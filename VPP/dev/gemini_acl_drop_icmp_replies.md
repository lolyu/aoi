# ICMP-to-Loopback punt on SONiC-VPP dual-ToR: current node path and the receive-DPO fix

Context: on a dual-ToR SONiC-VPP box the mux drop ACL (MuxOrch `INGRESS_TABLE_DROP` /
`mux_acl_rule`, scoped by `SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS`, action DROP) renders under
VPP as a port-wide `ipv4 deny 0.0.0.0/0 -> 0.0.0.0/0` at `l2-input`, and drops the switch's
own for-us/control traffic (the ICMP mux heartbeat to the SoC loopback) before it can be
punted. Related issue: sonic-net/sonic-buildimage#28884. PR ("add accept local rule before
drop ACL" approach): sonic-net/sonic-sairedis#2027.

This note captures (1) the current ICMP-to-loopback VPP node path, and (2) how the
"receive-DPO check before ACL" approach (a `sonic_ext` plugin feature node) would fix it.

--------------------------------------------------------------------------------
## 1. Current ICMP-to-Loopback VPP node path

An ICMP echo arrives on a **standby mux port** destined to a loopback (e.g. `10.1.0.36`,
which the FIB resolves to `dpo-receive on loop2`). The mux/server port is an **untagged
access port in bridge-domain 1000**, so the packet rides the **L2 path** and only becomes
"L3 for-us" after `l2-fwd` sends it to the BVI.

Full live node chain:

    dpdk-input
     -> sonic-ext-capture              (device-input arc: stamps magic cookie)
     -> ethernet-input
     -> l2-input                       (BD 1000)
        span-l2-input
        l2-input-classify              (untagged-LLDP punt session; miss -> continue)
        l2-input-feat-arc  ------->     l2-input-ip4 sub-arc:
                                          [0] acl-plugin-in-ip4-l2   <-- THE MUX DROP LIVES HERE
                                          [1] l2-input-feat-arc-end
        l2-policer-* / l2-input-acl / vpath / qos / l2-input-vtr
        l2-learn
        l2-fwd                         (dst MAC == BVI MAC -> send to BVI sw_if_index)
     -> ip4-input        (now on the BVI, L3)
     -> ip4-lookup       (hits 10.1.0.36/32 -> dpo-receive)
     -> ip4-receive
     -> ip4-icmp-input
     -> ip4-punt -> ip4-punt-redirect -> ip4-dvr-dpo -> ip4-dvr-reinject
     -> tap<bvi>-output -> sonic-ext-aggr-tap-redirect -> member-tap-tx -> kernel

Live confirmation of the destination being for-us:

    vppctl show ip fib 10.1.0.36
    10.1.0.36/32 fib:0 ... entry-flags:connected,local ...
        [@0]: dpo-receive: 10.1.0.36 on loop2

Live l2-input arc order and the ip4 sub-arc:

    span-l2-input l2-input-classify l2-input-feat-arc l2-policer-classify l2-policer-input
    l2-input-acl vpath-input-l2 l2-ip-qos-record l2-input-vtr sr-pl-rewrite-encaps-l2
    l2-learn l2-rw l2-fwd l2-flood l2-output

    [21] l2-input-ip4:
      [ 0]: acl-plugin-in-ip4-l2
      [ 1]: l2-input-feat-arc-end

### Two ordering facts that ARE the bug

1. The "for-us" decision (`dpo-receive`) happens at **`ip4-lookup`, which is AFTER
   `l2-fwd`** -- i.e. after the L2 feature arc where `acl-plugin-in-ip4-l2` (the mux drop)
   runs. So VPP evaluates the drop ACL BEFORE it knows the packet is for-us. Hardware
   CoPP-traps for-us traffic BEFORE the ACL engine; VPP cannot, because the receive-DPO is
   only discovered post-L2. This inversion is the whole problem.

2. `acl-plugin-in-ip4-l2` sits at index [0] of the `l2-input-ip4` sub-arc, reached via
   `l2-input-feat-arc`. That is exactly the hook point: a feature node registered on
   `l2-input-ip4` with `.runs_before = "acl-plugin-in-ip4-l2"` executes immediately before
   the drop ACL, still on the L2 path.

The "add accept local rule before drop ACL" approach (PR #2027) sidesteps the inversion by
pre-permitting the known for-us /32s and /128s inside the ACL (first-match-wins). The
"receive-DPO check before ACL" approach instead does the receive check directly at the hook,
needing zero ACL rules.

### 1a. The sonic_ext punt plumbing: magic cookie + aggr-tap-redirect

Two `sonic_ext` nodes in the path above (`sonic-ext-capture` at ingress, `sonic-ext-aggr-tap-redirect`
at egress) are two halves of one mechanism that preserves the *original ingress member port*
identity across the bridge -> BVI -> punt -> DVR-reinject journey, so a punted for-us packet
surfaces on the correct Linux netdev (e.g. Ethernet4), not the aggregate BVI tap.

Why it is needed: VPP's normal punt loses the ingress port. Once a for-us packet is bridged to
the BVI, punted at ip4-local, and DVR-reinjected toward the tap, VLIB_RX/VLIB_TX point at the
BVI / aggregate tap, not the mux member the packet actually arrived on. But SONiC needs it to
appear on the member's kernel netdev.

The magic cookie (`SONIC_EXT_BUFFER_MAGIC = 0x534e4358u`, ascii 'SNCX', in sonic_ext.h) is a
validity marker for a small per-buffer metadata stash overlaid on `vnet_buffer2(b)->unused[]`:

    typedef struct {
      u32 magic;                // 'SNCX' -- "these fields are really ours"
      u32 orig_rx_sw_if_index;  // the physical/sub-if the packet actually arrived on
      u32 orig_vlan_tag;        // outermost 802.1Q/802.1ad tag snapshot (TPID|TCI), 0 = untagged
    } sonic_ext_buffer_opaque_t;

Lifecycle:

- `sonic-ext-capture` runs first on the device-input arc (before ethernet-input), where VLIB_RX
  still equals the true ingress port and current_data still points at the raw wire L2 header. It
  writes orig_rx_sw_if_index, snapshots the outermost VLAN tag straight off the wire bytes
  [12..16], and stamps magic = 'SNCX' (capture_node.c).
- `sonic-ext-aggr-tap-redirect` runs on the interface-output arc. It reads the stash ONLY if
  magic == SONIC_EXT_BUFFER_MAGIC, then immediately clears magic = 0 (so it cannot fire twice or
  leak onto a recycled buffer), recovers orig_rx, resolves the member host tap via the linux-cp
  pair (lcp_itf_pair_find_by_phy -> lip_host_sw_if_index), rewinds to the original wire L2 and
  re-pushes the saved VLAN tag, sets VLIB_TX = member_tap, and sends to interface-output.

Why a magic number at all -- three defenses from the header comment:

1. Cross-plugin safety: opaque2->unused[] is shared scratch; a non-matching cookie makes the
   redirect node ignore the slot rather than redirect to a garbage sw_if_index.
2. Stale-buffer safety: buffers are pooled/recycled; the cookie (plus the clear-after-read)
   prevents a leftover value from a previous packet being mis-attributed.
3. Clone-survival: opaque2 is memcpy'd wholesale into every vlib_buffer_clone, so the metadata
   follows the packet through l2-flood / replication for free -- an external sidecar keyed by
   buffer index could not, without a core-VPP patch on every clone caller.

So in the trace `tap<bvi>-output -> sonic-ext-aggr-tap-redirect -> member-tap-tx -> kernel`:
`tap<bvi>-output` is the packet heading for the aggregate BVI tap; `sonic-ext-aggr-tap-redirect`
rewrites TX to the real member tap (using the cookie-validated orig_rx and re-pushing the saved
VLAN); `member-tap-tx -> kernel` delivers it on the correct SONiC interface. The cookie is what
lets the egress node trust the ingress-port identity that survived the whole punt journey.

--------------------------------------------------------------------------------
## 2. Approach: receive-DPO check before ACL (sonic_ext feature node)

Add a new internal node `sonic-ext-ip2me` in the out-of-tree `sonic_ext` plugin
(`sonic-platform-vpp/vppbld/plugins/sonic_ext/ip2me_node.c`), registered on the
`l2-input-ip4` arc (plus a twin on `l2-input-ip6`) to run right before the ACL plugin. This
mirrors the existing plugin nodes (`capture`/`host-xc` on `device-input`, `aggr-tap-redirect`
on `interface-output`), which all register with `VNET_FEATURE_INIT` + `.runs_before` and
steer with `vnet_feature_next()`.

### Placement (feature registration)

    VNET_FEATURE_INIT (sonic_ext_ip2me_ip4_feat, static) = {
      .arc_name    = "l2-input-ip4",
      .node_name   = "sonic-ext-ip2me-ip4",
      .runs_before = VNET_FEATURES ("acl-plugin-in-ip4-l2"),
    };
    /* + sonic_ext_ip2me_ip6_feat on "l2-input-ip6" runs_before "acl-plugin-in-ip6-l2" */

### Per-packet logic (FIB receive-lookup on inner dst IP)

    /* L3 header is at current_data + ethernet_buffer_header_size(b) on the
     * l2-input-ip4 arc; that helper already accounts for any 802.1Q/802.1ad tags,
     * and l2-input-vtr (tag pop) runs AFTER the ACL, i.e. after us. */
    u8 *l3 = vlib_buffer_get_current(b[0]) + ethernet_buffer_header_size(b[0]);
    ip4_header_t *ip = (ip4_header_t *) l3;
    u32 fib_index = vec_elt(ip4_main.fib_index_by_sw_if_index, sw_if_index);
    u32 lbi       = ip4_fib_forwarding_lookup(fib_index, &ip->dst_address);
    const load_balance_t *lb = load_balance_get(lbi);
    const dpo_id_t *dpo = load_balance_get_bucket_i(lb, 0);

    if (dpo->dpoi_type == DPO_RECEIVE) {
        /* ip2me: skip ONLY the mux drop ACL by jumping to the arc end node, so the
         * packet still flows l2-fwd -> BVI -> ip4-local for the normal punt. */
        next[0] = SONIC_EXT_IP2ME_NEXT_ARC_END;  /* static next -> "l2-input-feat-arc-end" */
        n_ip2me++;
    } else {
        /* Transit/data: let the ACL run exactly as today. */
        vnet_feature_next(&next0, b[0]);
        next[0] = (u16) next0;
    }

Key move: on an ip2me hit, steer `next` to `l2-input-feat-arc-end` (wired as a static
`.next_nodes` entry, not a per-frame `vlib_node_add_named_next`). This bypasses ONLY
`acl-plugin-in-ip4-l2`, leaving the rest of the L2 chain (`l2-input-vtr`, `l2-learn`,
`l2-fwd`) intact, so the packet still reaches the BVI and is punted through the normal path.
On a miss, `vnet_feature_next()` continues into the ACL and transit data is dropped exactly
as the mux intends.

### Per-interface gating (mirror the scoping)

Enable only where the mux drop binds -- driven from the same `SwitchVppAcl.cpp` bind path
that today populates `m_local_deny_tables`. The plugin exposes a per-port helper (and a
matching `sonic_ext_ip2me_enable_disable` binary API for sonic-sairedis) that toggles both
address families via the L2-specific `vnet_l2_feature_enable_disable`, which also flips the
port's `L2INPUT_FEAT_INPUT_FEAT_ARC` bitmap so the ip4/ip6 sub-arc is actually dispatched:

    void sonic_ext_ip2me_enable_disable(u32 sw_if_index, int enable) {
      vnet_l2_feature_enable_disable("l2-input-ip4", "sonic-ext-ip2me-ip4", sw_if_index, enable, 0, 0);
      vnet_l2_feature_enable_disable("l2-input-ip6", "sonic-ext-ip2me-ip6", sw_if_index, enable, 0, 0);
    }

So fabric/T1 ports never pay the cost, and the FIB lookup runs only on standby-mux-bound
ports. A `sonic-ext ip2me <interface> [on|off]` CLI is provided for manual bring-up/testing,
and `show sonic-ext` reports the `ip2me hits` counter.

### Trace after the fix

ip2me:

    ... l2-input-feat-arc
     -> sonic-ext-ip2me-ip4        (FIB lookup: 10.1.0.36 -> DPO_RECEIVE -> IP2ME)
     -> l2-input-feat-arc-end      (acl-plugin-in-ip4-l2 SKIPPED)
     -> l2-learn -> l2-fwd -> BVI -> ip4-lookup -> ip4-receive -> ip4-punt -> ... -> tap

Transit:

    ... sonic-ext-ip2me-ip4 (miss) -> acl-plugin-in-ip4-l2 (deny) -> drop


--------------------------------------------------------------------------------
## 3. receive-DPO check before ACL vs add accept local rule before drop ACL

| Aspect                     | Add accept local rule before drop ACL (#2027) | receive-DPO check before ACL             |
|----------------------------|-----------------------------------------------|------------------------------------------|
| ACL rules added            | O(loopbacks+SVIs), ~10 (bounded)              | 0                                        |
| Per-packet cost (gated)    | ~0 (reuses existing ACL tuple)                | +1 FIB lookup (~1.4x the ACL node, measured) |
| Address tracking           | maintain m_local_ips + refresh ACL            | none -- FIB is the source of truth       |
| Rule-budget / TupleMerge   | consumes rules; scales w/ addr count          | immune -- nothing in the ACL             |
| Where the change lives     | sairedis only (libsaivs)                      | sonic-platform-vpp plugin + small sairedis hook |
| "for-us" correctness       | enumerated set (could miss a class)           | exact -- DPO_RECEIVE is ground truth     |

Net: the receive-DPO check before ACL is the architecturally correct "CoPP-before-ACL" --
zero ACL rules, unbounded address support, no set to track, FIB as source of truth -- at the
cost of a per-packet FIB lookup on gated ports and a heavier two-repo (plugin) change plus
image rebuild. The add-accept-local-rule approach is the lighter, sairedis-only fix that,
after scoping to the mux ACL + loopback/SVI addresses only, keeps the rule count small and
bounded.

### Caveats to design for in the receive-DPO check before ACL

1. Tagged members: `l2-input-vtr` (tag pop) runs AFTER this hook, so for tagged access
   members the 802.1Q tag is still on -- the parser must skip 0x8100 before reading the IP
   header. Dual-ToR mux/server ports are untagged, so the common case is trivial, but handle
   the tag for correctness.
2. v4 + v6: two nodes (l2-input-ip4 / l2-input-ip6). ARP rides l2-input-nonip and is not
   dropped by the ipv4 deny, so no nonip node is needed.
3. Cost: the FIB lookup is the single most expensive node in the graph (~1.4x the ACL node),
   but it is gated to standby-mux ports only and amortizes under vector load.
4. Still needs the separate BVI-MAC fix: the receive-DPO check only stops the DROP; the packet still must
   reach the BVI via l2-fwd, which depends on the BVI answering to the dual-ToR gateway MAC
   (00:aa:bb:cc:dd:ee). That is the separate VLAN-RIF SRC_MAC bug (swss #4816 + sairedis
   VLAN-RIF work), not part of the ACL fix.

--------------------------------------------------------------------------------
## 4. Background: why the ACL rule count matters (the "~80" / TupleMerge note)

There is no hard 80-rule cap. The SONiC-VPP SAI layer advertises 1000 ACL entries per table
(`m_maxAclTableEntries = 1000` in `vslib/SwitchStateBase.h`; live CRM shows acl_entry
Available 999). "~80" is a practical/performance ceiling driven by:

- TupleMerge split threshold: `vpp/src/plugins/acl/acl.h` has `#define TM_SPLIT_THRESHOLD 39`.
  Rules sharing the SAME mask hash into ONE tuple; when that tuple's collision chain exceeds
  39, TupleMerge splits it -> more mask-types probed per packet -> slower lookup + more
  memory. 39 ~= "half of 80" and is the number behind the reviewer's instinct.
- Per-lookup-context replication: an ACL builds hash state per bound interface. The mux drop
  binds to ~32 ports, so each rule is instantiated x32. VPP's client/API heap is bounded
  (api-size 64M, main-heap-size 2G in startup.conf); we hit os_panic in
  clib_mem_heap_realloc_aligned earlier from this pressure.
- Rebuild cost: every ACL change does a full acl_add_replace and rebuilds per-context hash.

Why the unscoped change added 40+ rules: it injected one /32 (or /128) permit per local
address, counted twice (v4 + v6). On production the UPLINK routed-RIF class dominates and
scales with fabric (e.g. 16 uplinks x (v4+v6) = 32) plus loopbacks/SVIs -> 40+. Worse, every
permit is `src 0/0, dst /32, proto 0` -- IDENTICAL mask -> they all collapse into ONE
TupleMerge tuple, so 40+ same-mask permits pile a single tuple past the 39 split threshold.

The scoping in PR #2027 fixes both: Lever A (only the IN_PORTS-scoped mux drop table gets
permits) and Lever B (only mux-reachable loopback+SVI addresses; drop uplink routed RIFs)
remove the fabric-proportional term, keeping the count bounded (~10) and the shared-mask
tuple well under 39. The receive-DPO check before ACL removes the rule cost entirely.

--------------------------------------------------------------------------------
## 5. Field-confirmed dual-ToR blockers (2026-08-13)

Sections 1-4 fix the *steady-state* inversion: the mux drop ACL runs before VPP knows
a packet is for-us. In the field, the same symptom -- linkmgrd "no ICMP replies
received", mux ports `unhealthy`, cycling `Peer_Heartbeat_Missing` -- has **two more
independent causes** on the exact same link-prober echo-reply path. All three must be
fixed for a dual-ToR mux port to punt its heartbeat; fixing only one still leaves the
port unhealthy. §5.3 then covers a **separate downstream data-plane** blocker surfaced in
the same session (server-facing traffic on the standby ToR), not part of the punt path.

| # | Drop point (node)              | Trigger                          | Fix                                    |
|---|--------------------------------|----------------------------------|----------------------------------------|
| 1 | `acl-plugin-in-ip4-l2` (drop)  | steady state (ACL before for-us) | ip2me receive-DPO bypass (§2) / #2027  |
| 2 | `acl-plugin-in-ip4-l2` (drop)  | standby->active / bind churn     | stale-deny guard (§5.1)                |
| 3 | `l2-fwd` (flood, never to BVI) | BVI not owning the gateway MAC   | VLAN-RIF SRC_MAC fix, swss #4816 (§5.2)|

### 5.1 Stale drop ACL issue (a sibling of the §1 ACL drop)

Symptom: for-us / ip2me punts silently dropped, but the trigger is a **mux standby->active
transition (or bind churn)**, not steady state.

Mechanism: MuxOrch deletes the ingress drop entry when a port leaves standby, so the ACL
table's rule set goes empty and sairedis `AclTblConfig()` computes `acl == NULL`. The old
guard `if (acl != NULL)` then **skipped `acl_add_replace()` entirely**, so the
previously-programmed VPP ACL (`deny 0.0.0.0/0 -> 0.0.0.0/0`) stayed bound to the mux
ports and kept matching -- SONiC believes the drop is gone, VPP is still dropping. This
is the same `acl-plugin-in-ip4-l2` node as §1, just *stale* rather than *pre-for-us*.

Fix -- the "stale-deny guard" (sonic-sairedis `dev`, `vslib/vpp/SwitchVppAcl.cpp`,
`AclTblConfig()` ~line 1258):

    // acl is NULL when the table has no regular rules left - e.g. the mux drop
    // entry is deleted when a port transitions standby -> active. Route the NULL
    // acl through acl_add_replace() ONLY when the table was already programmed,
    // so its acl==NULL branch (emptyAclCreate) replaces the stale rule in-place.
    if (acl != NULL || m_acl_swindex_map.find(tbl_oid) != m_acl_swindex_map.end()) {
        status = acl_add_replace(acl, tbl_oid, aces, ordered_aces);

On the NULL path `acl_add_replace()` calls `emptyAclCreate()`, which replaces the same
`acl_swindex` in place with a harmless `permit dst 0.0.0.0/32` -- binds are preserved and
the stale `deny` stops matching. The `|| find(...)` half is load-bearing: it keeps a NULL
acl from reaching `vpp_acl_add_replace()` on a *never-programmed* table (that branch would
pass NULL straight through); `status` is pre-initialized `SAI_STATUS_SUCCESS` (line 1172)
so the never-programmed skip path is safe. Commit `[vpp] Clear stale VPP ACL when a
table's last rule is removed`; built into `docker-syncd-vpp` (dev @`56de6edb`) and
binary-verified in the shipped `libsaivs.so`.

### 5.2 VLAN BVI MAC issue (the downstream §3-caveat-4 blocker, now field-confirmed)

Symptom: identical linkmgrd "no ICMP replies received" -- but it survives *even with the
ACL correctly bypassed*. This is the separate "still needs the BVI-MAC fix" caveat from
§3, reproduced live.

Mechanism: the link-prober echo-reply comes back with **dst-mac = the shared dual-ToR
gateway MAC `00:aa:bb:cc:dd:ee`**. `bvi1000` (the Vlan1000 RIF), however, is programmed
with the **per-device switch base MAC** (vpp-03 `22:ff:ab:76:97:c0`, vpp-04
`22:ad:a2:34:08:76`), not the gateway MAC. So `l2-fwd` looks up the gateway MAC in BD1000,
**misses** (`result [0xffffffffffffffff, -1]`), UU-floods to every bridge member and never
sends the packet up to the BVI -- so it never reaches `ip4-input` / `ip4-local` and is
never punted to linkmgrd.

Live trace (vpp-04, ip2me already enabled -- proves this is downstream of the ACL fix):

    dpdk-input: bobm10  IP4: <server> -> 00:aa:bb:cc:dd:ee  ICMP 192.168.0.11 -> 10.1.0.36 echo_reply
     -> sonic-ext-ip2me-ip4:  ... -> IP2ME (bypass ACL)        <-- ACL correctly skipped
     -> l2-fwd: dst 00:aa:bb:cc:dd:ee ... result [0xffffffffffffffff, -1]   <-- MISS
     -> l2-flood -> l2-output x(all other mux members)         <-- never reaches bvi1000
     (never: ip4-input -> ip4-local -> ip4-punt -> tap -> linkmgrd)

So the BVI-MAC bug **masks a working ACL fix**: on vpp-04 `sonic-ext-ip2me` was correctly
bypassing the ACL and the port was still `unhealthy`, because the reply died one node
later at `l2-fwd`.

Root cause: IntfsOrch overwrites the shared Vlan1000 RIF `SAI_ROUTER_INTERFACE_ATTR_SRC_MAC_ADDRESS`
with the per-device switch MAC (`gMacAddress`) on any INTF_TABLE update -- a regression
from EVPN-MH PR #4615 (`f0c53b94`). VPP's BVI inherits the RIF MAC, so it stops answering
to the gateway MAC. Fix: swss **#4816** (preserve port MAC when updating router-interface
MAC) + the sairedis VLAN-RIF work; tracked by swss issue **#4823**.

#### How to change the Vlan1000 BVI MAC (runtime fix)

Runtime workaround, applied **per DUT** (wiped on any `syncd`/image restart until the fix
ships in the image). Run each step from the SONiC host shell:

1. **Confirm the bug** -- the BVI is on the per-device base MAC, not the gateway MAC. The
   `Idx`/sw_if_index is `89` and the `Ethernet address` is what matters:

       docker exec syncd vppctl show hardware-interfaces bvi1000

       # BEFORE (bad) -- per-device base MAC:
       #   bvi1000                            89     up   bvi1000
       #     Ethernet address 22:ad:a2:34:08:76        <-- vpp-04 base (vpp-03: 22:ff:ab:76:97:c0)

2. **Set the BVI MAC to the shared dual-ToR gateway MAC** `00:aa:bb:cc:dd:ee`:

       docker exec syncd vppctl set interface mac address bvi1000 00:aa:bb:cc:dd:ee

3. **Verify the MAC took** (re-run the show; the `Ethernet address` must now be the gw MAC):

       docker exec syncd vppctl show hardware-interfaces bvi1000
       #     Ethernet address 00:aa:bb:cc:dd:ee        <-- AFTER (good)

4. **Verify forwarding is restored** -- the reply to the gw MAC now hits the BVI in BD1000
   instead of UU-flooding, and rises to `ip4-local` -> punt -> linkmgrd. Confirm with a live
   trace (send/receive one link-prober heartbeat, then):

       docker exec syncd vppctl show trace
       # expect: l2-fwd dst 00:aa:bb:cc:dd:ee ... result [<oif>, 97]   <-- HIT bvi1000 (not 0xffff..ffff)
       #      -> ip4-receive -> ip4-local/ip4-punt -> tap -> linkmgrd

   Then confirm the mux ports report healthy:

       show mux status        # expect all mux ports 'healthy' (0/24 -> 24/24)

5. **Repeat on the peer DUT.** Do steps 1-4 on both `vpp-03` (10.250.0.133) and
   `vpp-04` (10.250.0.134); each carries its own per-device base MAC and must be set
   independently.

Note: this is `vppctl`-only and does not survive a `syncd`/container/image restart -- it is a
stopgap until swss **#4816** + the sairedis VLAN-RIF work land in the deployed image (issue
**#4823**).

Applying it flipped **both DUTs from 0/24 to 24/24 mux ports healthy**. (vpp-03 also had
`sonic-ext-ip2me` missing from its `l2-input-ip4` arc -- i.e. it was hitting drop point #1
as well -- so it additionally needed `vppctl sonic-ext ip2me bobm<N> on` on every mux
member before it went healthy; vpp-04 already had ip2me and only needed the BVI-MAC set.)

### 5.3 Standby tunnel-route clobber issue (downstream data-plane, not the punt path)

Distinct from the three punt-path drop points above, the same 2026-08-13 session surfaced a
**downstream data-plane** blocker: traffic destined to a server on the **standby** ToR is
black-holed because the server's IPinIP tunnel-encap route is missing from the VPP FIB. This
is a server-facing forwarding failure, not a heartbeat-punt failure -- it shows up as
`test_normal_op_downstream_lower_tor` delivering 0/24 packets after a bulk mux toggle to
standby.

Symptom (standby vpp-04): every server `192.168.0.X/32` should resolve to an IPinIP tunnel to
the peer ToR, but the VPP FIB has **0/24** of them; all fall through to the `192.168.0.0/21`
glean on `bvi1000` -> ARP on Vlan1000 (the server is not directly reachable on the standby) ->
dropped. ASIC_DB is correct: all 25 server `/32` `ROUTE_ENTRY` point at NH
`oid:0x4000000000646` = `SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP` -> `10.1.0.32`, tunnel
`oid:0x2a000000000607`; the encap tunnel exists in VPP (`ipip16 src 10.1.0.33 dst 10.1.0.32`,
sw_if 89) and the underlay `10.1.0.32/32` resolves via BGP ECMP over BondEthernet101-104. So
the route add is *accepted* (syncd logs status 0) yet the prefix ends up absent. (Neighbor
side is correct: active vpp-03 = 24/24 static neighbors + attached `/32` on `bvi1000`, no
tunnel; standby vpp-04 = 0/24 neighbors -- only the tunnel-route half fails.)

Mechanism -- a same-prefix clobber between two independent libsaivs writers of the *same*
server `/32`:

- neighbor host route: `vslib/vpp/SwitchVppNbr.cpp::addRemoveIpNbr()` programs a per-neighbor
  `/32` (or `/128`) via `bvi1000`.
- mux tunnel route: `vslib/vpp/SwitchVppRoute.cpp::IpRouteAddRemove()` programs the same `/32`
  as a single-NH tunnel-encap route.

Both went through VPP `ip_route_add_del()` with `is_multipath = false`. VPP's four flag
combinations for a prefix:

| is_add | is_multipath | VPP action on the prefix                                    |
|--------|--------------|-------------------------------------------------------------|
| 1      | 0            | REPLACE the whole entry with the supplied path              |
| 1      | 1            | ADD the supplied path (ECMP)                                |
| 0      | 1            | remove ONLY the supplied path (entry survives if others)    |
| 0      | 0            | **DELETE the ENTIRE prefix by key (supplied path IGNORED)** |

MuxOrch always issues **add-new-then-remove-old** on a transition (`MuxNbrHandler::disable`:
`addRoutes()` then `disableNeighbors()`; `enable`: `enableNeighbors()` then `removeRoutes()`).
So on active->standby the order is (1) CREATE `X/32` -> tunnel NH (VPP programs it, status 0),
then (2) REMOVE neighbor `X` -> the neighbor host-route removal runs with `is_add=0,
is_multipath=0` and **deletes the entire `X/32`**, clobbering the tunnel route from step 1.
Live proof (vpp-04, 08:11:26): `Add ip route 192.168.0.2/32 status 0` @315ms, then
`Remove neighbor host route 192.168.0.2 status 0` @332ms -> `X/32` ends absent.

Why standby broke but active stayed healthy: the standby tunnel route is a one-shot MuxOrch
creation with no refresh, so once clobbered it stays gone; the active `/32` is continuously
re-established by live neighbor resolution (real ARP re-runs `addRemoveIpNbr(add)`), so it
self-heals. The symmetric standby->active clobber (`removeRoutes()` after `enableNeighbors()`)
was latent -- masked only because the first bug had already removed the tunnel route, making
the trailing `removeRoutes()` a no-op. Fixing only the neighbor side would unmask it and break
the active path, so both writers must change together.

Fix -- make every remove **path-specific** instead of whole-prefix (last-add-wins), so a
trailing remove targets a path already replaced away (harmless no-op) and any coexisting route
on the same prefix survives (sonic-sairedis `dev`, commit `b3d2afdf`):

    // SwitchVppNbr.cpp::addRemoveIpNbr(), host-route block (~line 222)
    ip_route->is_multipath = !is_add;   // add = REPLACE (authoritative); remove = path-specific

    // SwitchVppRoute.cpp::IpRouteAddRemove() (~line 243)
    ip_route->is_multipath = (nxthop_group->nmembers > 1) || !is_add;
    // ECMP (nmembers>1) already path-based; single-NH: REPLACE on add, path-specific on remove

A path-specific remove is identical to a whole-delete when only one path exists, so non-dualtor
single-NH and existing ECMP are unaffected; it also fixes the v6 `/128` case.
`IpRoutePathAddRemove()` already used `is_multipath=true`, corroborating the intended pattern.
Built into `docker-syncd-vpp` (dev @`b3d2afdf`) and binary-verified in the shipped
`libsaivs.so`: both stores now write a computed `!is_add` (`xor $0x1` -> the `is_multipath`
field at struct offset `0x28`), not the old constant `false`.

Expected after the fix (single active<->standby toggle): standby shows **24/24** server `/32`
via `ipip16`, and after toggling back active shows **24/24** `/32` via `bvi1000` with no
tunnel. (Image built + binary-verified; not yet deployed/live-verified on the running testbed.)
