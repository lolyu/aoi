# Dual-ToR mux drop ACL applies to ALL ports: `SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS` is never read

Context: on a dual-ToR SONiC-VPP box, MuxOrch's drop ACL (`INGRESS_TABLE_DROP` / `mux_acl_rule`,
action DROP) is scoped to the **standby** mux ports via `SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS`.
The VPP SAI layer never reads that qualifier, so the entry renders as an unconditional
`ipv4 deny 0.0.0.0/0 -> 0.0.0.0/0` bound to **every** port of the table. A single standby port
blackholes the entire data plane. Filed as sonic-net/sonic-buildimage#29077.

Distinct from the sibling issue #28884 / sonic-sairedis#2027 (`gemini_acl_drop_icmp_replies.md`):
that one is "the drop ACL also eats the switch's own for-us traffic" and was fixed with the
`sonic_ext` ip2me receive-DPO node. This note is the *scoping* half -- the drop applies to ports
it was never meant to touch. Both stem from the same rule being over-applied, but the fixes are
independent and live in different layers.

Fix branch: `sonic-sairedis` @ `fix_vpp_acl_in_ports` (off `dev`). Not yet committed/deployed.

--------------------------------------------------------------------------------
## 1. The defect

`grep -rn "IN_PORTS" vslib/` returns **nothing**. In `SwitchVppAcl.cpp::acl_rule_field_update()`
the attribute falls through to `default:`:

    default:
        SWSS_LOG_ERROR("Unhandled ACL entry attribute ID: %u", attr_id);
        break;

`SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS = SAI_ACL_ENTRY_ATTR_FIELD_START + 0xa = 0x1000 + 0xa
= 0x100a = 4106` (`SAI/inc/saiacl.h:1828,2000`), which is the ID in the log line.

The silent part: `status` is initialised `SAI_STATUS_SUCCESS` at the top of the function and the
`default:` arm only `break`s. **The function returns success**, so the entry is programmed as if
the qualifier did not exist -- a `deny any -> any` with no port restriction.

Day-1, not a regression. The file entered sonic-sairedis at `552cc66c` (2025-04-14, "Merge vector
packet processing (vpp) from sonic-platform-vpp with virtual switch #1473"), re-homed from
sonic-platform-vpp `platform/saivpp/` which was deleted there in `0023aba` (2024-10-28). The
original introduction is 2023-07-04 (`f351116`); `IN_PORTS` was absent from the switch from the
first commit.

### Live confirmation of the ASIC_DB intent vs what VPP got

Set `Ethernet4` + `Ethernet8` standby on vlab-vpp-03. ASIC_DB carries the scoping correctly:

    ASIC_STATE:SAI_OBJECT_TYPE_ACL_ENTRY:oid:0x8000000000724
      SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS = 2:oid:0x1000000000033,oid:0x1000000000034

but `vppctl show acl-plugin acl` showed the same 4 ACLs as baseline (indices 0-3) with the deny
rule in the table ACL bound to every front-panel port (32 of them -- see section 1b, which
measures this precisely).

Note the entry count: MuxOrch keeps **one shared table + one shared rule** for all mux ports
(`orchagent/muxorch.cpp:50` `MUX_ACL_RULE_NAME`, created once at :1501, then appended to via
`RULE_OPER_ADD` :1505 / `RULE_OPER_DELETE` :1530). Each switchover just ADDs/DELETEs a port OID
on that single rule's `IN_PORTS` list. So "32 standby mux ports" does **not** mean 32 ACL
entries -- it means one entry whose `IN_PORTS` has 32 members.

--------------------------------------------------------------------------------
## 1b. Live reproduction, single standby port (vlab-vpp-03, 2026-08-19 08:54)

```
root@vlab-vpp-03:/# vppctl show acl-plugin acl
acl-index 0 count 1 tag {sonic_acl_oid:0x700000000}
          0: ipv4 permit src 0.0.0.0/0 dst 0.0.0.0/32 proto 0 sport 0 dport 0
  applied inbound on sw_if_index: 67, 65, 66, 68
  used in lookup context index: 2, 0, 1, 3
acl-index 1 count 2 tag {sonic_acl_default_permit}
          0: ipv4 permit src 0.0.0.0/0 dst 0.0.0.0/0 proto 0 sport 0 dport 0
          1: ipv6 permit src ::/0 dst ::/0 proto 0 sport 0 dport 0
  applied inbound on sw_if_index: 24, 65, 66, 67, 6, 7, 9, 8, 2, 11, 10, 20, 30, 5, 32, 13, 12, 14, 15, 17, 16, 18, 19, 21, 3, 22, 23, 68, 1, 26, 27, 28, 29, 4, 31, 25
  used in lookup context index: 26, 0, 1, 2, 13, 4, 7, 5, 10, 8, 6, 22, 33, 14, 35, 16, 9, 15, 17, 20, 18, 19, 21, 24, 11, 23, 25, 3, 28, 29, 30, 31, 32, 12, 34, 27
acl-index 2 count 1 tag {sonic_acl_oid:0x700000001}
          0: ipv4 permit src 0.0.0.0/0 dst 0.0.0.0/32 proto 0 sport 0 dport 0
  applied inbound on sw_if_index: 6, 7, 9, 8, 2, 11, 10, 20, 68, 5, 4, 13, 12, 14, 15, 17, 16, 18, 19, 21, 3, 22, 23, 24, 65, 66, 67, 25
  used in lookup context index: 13, 4, 7, 5, 10, 8, 6, 22, 3, 14, 12, 16, 9, 15, 17, 20, 18, 19, 21, 24, 11, 23, 25, 26, 0, 1, 2, 27
acl-index 3 count 1 tag {sonic_acl_oid:0x700000002}
          0: ipv4 deny src 0.0.0.0/0 dst 0.0.0.0/0 proto 0 sport 0 dport 0
  applied inbound on sw_if_index: 1, 26, 27, 28, 29, 30, 4, 31, 32, 5, 6, 7, 8, 9, 10, 2, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 3, 21, 22, 23, 24, 25
  used in lookup context index: 28, 29, 30, 31, 32, 33, 12, 34, 35, 14, 13, 4, 5, 7, 6, 10, 8, 9, 16, 15, 17, 18, 20, 19, 21, 22, 11, 24, 23, 25, 26, 27
```

The worst-case demonstration: **one** port standby, blast radius **all 32**.

    show mux status
      Ethernet4   standby   ...  2026-Aug-19 08:52:49
      Ethernet8 .. Ethernet96   active     (23 ports)

sw_if_index map (`port_config.ini` `index` column == bobm number, sw_if_index = index+1):

    sw_if_index  1      = bobm0        = Ethernet0            (not a mux port)
    sw_if_index  2      = bobm1        = Ethernet4            <-- THE ONLY STANDBY
    sw_if_index  3..25  = bobm2..24    = Ethernet8..96        (all ACTIVE)
    sw_if_index 26..32  = bobm25..31   = Ethernet100..124     (not mux ports)
    sw_if_index 65..68  = BondEthernet101..104                (uplinks to T1)

The four ACLs, resolved through `VIDTORID` (the VPP tag carries the **RID**, ASIC_DB keys
are **VIDs**):

| acl-index | tag RID      | ASIC_DB VID       | table fields          | rule                    | bound to |
|-----------|--------------|-------------------|-----------------------|-------------------------|----------|
| 0         | 0x700000000  | 0x70000000005f8   | full, no IN_PORTS     | placeholder `dst /32`   | 65-68    |
| 1         | -            | -                 | `sonic_acl_default_permit` | permit v4 + v6     | all 36   |
| 2         | 0x700000001  | 0x7000000000601   | full + IN_PORTS + DSCP| placeholder `dst /32`   | 2-25,65-68 |
| 3         | 0x700000002  | 0x7000000000667   | **IN_PORTS + TC only**| **`ipv4 deny any->any`**| **1-32** |

`acl-index 3` is unambiguously MuxOrch's `INGRESS_TABLE_DROP`: `VIDTORID[oid:0x7000000000667]
= oid:0x700000002`, and its table has only `SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS` + `FIELD_TC`.
The `dst 0.0.0.0/32` rules in ACLs 0 and 2 are `emptyAclCreate()` placeholders -- they can never
match (dst must literally equal 0.0.0.0); VPP forbids a zero-rule ACL, hence the filler.

**Intent, from ASIC_DB -- exactly one port:**

    ASIC_STATE:SAI_OBJECT_TYPE_ACL_ENTRY:oid:0x8000000000729
      SAI_ACL_ENTRY_ATTR_TABLE_ID            = oid:0x7000000000667
      SAI_ACL_ENTRY_ATTR_ACTION_PACKET_ACTION= SAI_PACKET_ACTION_DROP
      SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS      = 1:oid:0x1000000000033   <-- Ethernet4, count 1

**Reality, from VPP -- exactly 32 ports** (verified as the complete set {1..32}):

    acl-index 3 ... applied inbound on sw_if_index: 1 2 3 ... 32     (32 entries)

**And the bind order makes it lethal.** Standby and active are byte-for-byte identical:

    sw_if_index  2 (Ethernet4,   STANDBY):  input acl(s): 2, 3, 1
    sw_if_index  3 (Ethernet8,   ACTIVE ):  input acl(s): 2, 3, 1     <-- same!
    sw_if_index 25 (Ethernet96,  ACTIVE ):  input acl(s): 2, 3, 1     <-- same!
    sw_if_index 32 (Ethernet124, NOT MUX):  input acl(s): 3, 1        <-- deny still there
    sw_if_index 65 (BondEthernet101      ):  input acl(s): 0, 2, 1    <-- spared

ACL 2 never matches, so on every front-panel port the *first* rule that can match is
`acl-index 3`'s `ipv4 deny any -> any`, and `sonic_acl_default_permit` sits behind it,
unreachable.

### Why the table is bound to all 32 -- and why that is CORRECT

`MuxAclHandler::createMuxAclTable()` ends in `bindAllPorts()`
(`orchagent/muxorch.cpp:1534,1553,1577`), which links **every** `Port::PHY`:

    void MuxAclHandler::bindAllPorts(AclTable &acl_table) {
        for (auto &it: gPortsOrch->getAllPorts()) {
            Port port = it.second;
            if (port.m_type == Port::PHY) { acl_table.link(port.m_port_id); ... }

This is deliberate. MuxOrch's contract is: **bind the table once, to everything, forever; do all
per-port scoping in the rule's `IN_PORTS`** -- so a switchover is a cheap rule edit
(`createMuxAclRule` sets `MATCH_IN_PORTS = alias_`, priority 999, action DROP;
`muxorch.cpp:1505/1530` then `RULE_OPER_ADD`/`RULE_OPER_DELETE` a port on that one shared rule)
and never a table re-bind.

The two mechanisms are therefore **not redundant** -- `IN_PORTS` is the *only* thing narrowing
the drop. A SAI backend that ignores it does not get an approximately-right scope; it gets the
**exact inverse of the intent**: deny on 32 ports instead of 1. This is why the bug is
catastrophic rather than cosmetic, and why the fix has to live in the binding layer.

### Measured blast radius

    IPv4 192.168.0.3  -> 100% LOSS      (server behind an ACTIVE mux port)
    IPv4 192.168.0.7  -> 100% LOSS
    IPv4 192.168.0.16 -> 100% LOSS

Everything that is **not IPv4** still works, which is the signature of a single `ipv4 deny`:

- **ARP is fine** (ethertype 0x0806, unmatched). All 24 neighbors `REACHABLE`; flushing
  `192.168.0.3` and re-resolving succeeded within seconds -- while ICMP to that same address
  stayed at 100% loss.
- **Uplinks are fine.** All 8 BGP sessions (4x v4, 4x v6) up `1d02h57m` with 6401 prefixes --
  they live on `BondEthernet101-104` / sw_if_index 65-68, which `bindAllPorts()` skips because
  those are `Port::LAG`, not `Port::PHY`.

Which explains the `show mux status` output exactly: `HWSTATUS consistent` (the mux/L2 state
machine is fine) but `HEALTH unhealthy` on **all 24 ports** -- linkmgrd's ICMP heartbeats are
IPv4 and every one of them is dropped inbound.

### What this run should look like after the fix

    acl-index 3 (deny)  applied inbound on sw_if_index: 2          <-- Ethernet4 ONLY
    sw_if_index  2:  input acl(s): 2, 3, 1        (deny present, permit last)
    sw_if_index  3:  input acl(s): 2, <placeholder>, 1   (no deny)
    sw_if_index 32:  input acl(s): <placeholder>, 1      (no deny)

Note this run exercises **bug (a)** from section 6 head-on: the mux table's *only* entry is the
scoped one, so `base_aces` is empty and the default ACL must come from `emptyAclCreate()`. Without
that branch the 31 non-standby ports would fail to resolve a swindex and binding would abort for
the whole port -- so the placeholder path is on the critical path for the real-world case, not
an edge case.

--------------------------------------------------------------------------------
## 2. Why this is not a one-line switch-case addition

A VPP ACL **rule** has no interface field. `vpp_acl_rule_t` (`vppxlate/SaiVppXlate.h:86-99`) is
action + prefixes + proto + ports + TCP flags -- nothing else. Upstream
`src/plugins/acl/acl_types.api` agrees. VPP expresses ingress-port scoping **only** by which
interfaces an ACL is bound to (`vpp_acl_interface_bind/unbind`, `SaiVppXlate.c:3639-3699`).

So there is nowhere for `IN_PORTS` to land in the rule struct, and "add a case to
`acl_rule_field_update()`" is not implementable. The fix must live in the **binding layer**.

### The attribute is also not readable where you'd expect

`get_sorted_aces()` fills `acl_tbl_entries_t::attrs` via `get_max()` into a `calloc`'ed buffer.
`meta/SaiSerialize.cpp::transfer_list()` (L111-122):

    if (countOnly || dst_element.count == 0)
    {
        transfer_primitive(src_element.count, dst_element.count);
        return SAI_STATUS_SUCCESS;
    }

Zeroed buffer => `count == 0` => **the count is copied, `.list` stays NULL, and the call
succeeds**. So `IN_PORTS` is present in the ACE attrs array with a plausible count and a NULL
list. Any consumer must issue its own two-step `get()` (count first, then with a caller-supplied
buffer). This is exactly the workaround the existing range code already uses
(`p_ace->attr_range.value.aclfield.data.objlist.list = p_ace->range_objid_list` before a separate
`get()`).

--------------------------------------------------------------------------------
## 3. Design of the fix

**The default ACL** = the entries **without** `IN_PORTS`. Keeps `m_acl_swindex_map[tbl_oid]`, stays
bound to every port of the table, and degrades to the `emptyAclCreate()` placeholder
(`permit dst 0.0.0.0/32`, never matches -- VPP forbids a 0-rule ACL) when there are no unscoped
entries. Chosen so the default ACL's identity **never depends on data**, which is what lets every
existing path (tunterm, `AclTblRemove`, `emptyAclCreate`) keep working untouched.

**Scoped ACLs** for interfaces named by some `IN_PORTS`, grouped by *applicable-ACE signature* =
the ordered `ace_oid` vector of (unscoped entries + scoped entries naming that interface). One
VPP ACL per distinct signature; interfaces sharing a signature share one ACL. The signature is
also the reuse key, so an unchanged scoped ACL keeps its swindex and only bindings move.

### Partition PORTS by applicable-entry set, not the ENTRIES

The naive reading ("partition the entries by their `IN_PORTS` set") **breaks priority ordering**.
Given A(prio 100, scoped), B(prio 50, unscoped), C(prio 10, scoped), a scoped port needs A,B,C in
that order, but ACL-level binding could only produce `[A,C],[B]` or `[B],[A,C]`. Grouping *ports*
instead means each scoped ACL contains the full priority-ordered applicable set, so ordering is
preserved by construction.

### Scaling for the mux case

All-standby => all ports share one signature => **1 scoped ACL** bound to N interfaces, plus the
default-ACL placeholder. Not N ACLs. Single-standby => 1 scoped ACL on 1 port, the rest sit on the
placeholder, which is exactly today's known-good all-active state.

### Rejected alternatives

- *One ACL per interface always*: stable swindexes, zero rebinding, but N-times rule duplication.
- *Union-binding only* (bind the table ACL to the union of all `IN_PORTS`): trivially small and
  fully fixes mux, but wrong when entries carry *different* `IN_PORTS` sets. **Was** kept as a
  fallback if the full design proved too big for review -- **now definitively rejected**, see
  section 3b.
- *`acl_interface_set_acl_list`*: the theoretically right primitive (atomic ordered list
  replacement, removes the unbind/rebind window entirely), but needs a new VPP API binding.

--------------------------------------------------------------------------------
## 3b. Option A (union) vs Option B (partition) -- and why B is required (2026-08-19)

Revisited after the question *"this should be easy to fix, right? simple allow VPP to honor in
port list change"*. It is not, and the cheap option is **actively unsafe** on a normal SONiC
config. Recording the reasoning because the conclusion reverses an earlier working assumption.

### Why "just add a case for IN_PORTS" is impossible, not merely hard

`acl_rule_field_update()`'s entire output is a `vpp_acl_rule_t`
(`vslib/vpp/vppxlate/SaiVppXlate.h:86-99`) -- action, src/dst prefix + masks, proto, L4 port
ranges, tcp flags. **No interface member exists to write to.** And it is not a SONiC omission:
the struct is a 1:1 mirror of upstream VPP's own wire format (`acl_types.api`, `typedef
acl_rule`), which has exactly those fields. Adding one means changing VPP's binary API.

Structurally: VPP resolves *which ACLs apply to this port* **before** rule matching -- the
interface's lookup-context index is part of the match key itself (`fa_node.h`,
`fa_packet_info_t.lc_index`). By the time a rule is evaluated the port question is already
settled. So `IN_PORTS={Ethernet4}` is expressible only as *"bind this ACL to Ethernet4"*, never
as *"match if in-port == Ethernet4"*. Same semantics, one layer up.

The irreducible difficulty is a **cardinality mismatch**: SAI carries scope per *entry*, VPP
carries it per *ACL* (all rules in an ACL share one binding). A SAI table whose entries have
heterogeneous `IN_PORTS` cannot be one VPP ACL.

### The two options, on a worked example

Table bound to 32 ports, two entries:

    E1 (mux):    IN_PORTS={Ethernet4}   match any      action DROP
    E2 (pfcwd):  IN_PORTS={Ethernet8}   match TC=3     action DROP

| | VPP ACLs created | Bindings | Result |
|---|---|---|---|
| **today (broken)** | one, `[E1,E2]` | all 32 | every port gets `E1` deny-any -> box-wide blackhole |
| **A -- union** | one, `[E1,E2]` | `{Eth4,Eth8}` only | Eth4 correct; **Eth8 also gets `E1` deny-any** |
| **B -- partition** | `acl_a=[E1]`, `acl_b=[E2]` | `a`->Eth4, `b`->Eth8 | both correct; rest unbound -> `sonic_acl_default_permit` |

Option A ~150 lines, Option B 868 (on `fix_vpp_acl_in_ports`). Note ~90 lines are **common to
both** -- reading the attribute at all needs the two-step `get()` (section 2), and both need
the unbind/rebind-whole-port gotcha of section 4. The delta is really
`acl_port_scope_update()` (337 lines): partition diffing, slot reuse, ordering.

### Revisited 2026-08-20: is heterogeneous IN_PORTS a real SONiC config?

Challenged again with *"does SONiC really have multiple ACL rules with IN_PORTS in one table?"*
Answer is yes, and there is harder evidence than the mux/pfcwd interaction below -- a **shipping
sonic-mgmt test fixture**, i.e. static config rather than a race between two orchagents:

    sonic-mgmt/tests/generic_config_updater/templates/create_three_drop_rules.j2
        DYNAMIC_ACL_TABLE|RULE_3  pri 898  DROP  IN_PORTS={blocked_port_1}
        DYNAMIC_ACL_TABLE|RULE_4  pri 897  DROP  IN_PORTS={blocked_port_2}
        DYNAMIC_ACL_TABLE|RULE_5  pri 896  DROP  IN_PORTS={blocked_port_3}

Same table also holds **unscoped** FORWARD rules (`create_forward_rules.j2` RULE_1/RULE_2,
`create_arp_forward_rule.json`), and is bound to *all* downstream ports
(`test_dynamic_acl.py:285`, `bind_ports = downstream_ports`) while naming only 3. Consumed by
`test_gcu_acl_drop_rule_removal` (`:1265`), which removes one rule and asserts that port
forwards *while the other two stay blocked* -- a direct assertion of per-port differentiation.

Second independent case, **PFC watchdog alone, no dualtor**: the rule name is keyed on queue id
(`pfcactionhandler.cpp:318` `m_strRule = "Rule_PfcWdAclHandler_" + queuestr`). More ports
storming the *same* queue append to one rule's IN_PORTS (`:341`), but a *different* queue
creates a **new** rule (`:336-337`) in the same `IngressTableDrop`, matching `TC=<qid>`
(`:506-508`). Two lossless queues storming different ports => two rules, disjoint IN_PORTS.

**Option A has two failure modes, not one** (the second was missed in the original analysis):

  (a) *over-application* -- a scoped rule reaches ports it does not name. Requires the scoped
      rules to differ. Reachable via mux (`deny any`) + pfcwd (`deny TC=3`), or two pfcwd queues.
  (b) *under-application* -- union binding also confines the **unscoped** rules to the union,
      silently dropping them from every other bound port. `DYNAMIC_ACL_TABLE` is exactly this
      shape. Benign there only because its unscoped rules are FORWARD (~= default); an unscoped
      **DROP** plus any scoped rule becomes silent loss of enforcement, with no error and no log.

Caveat kept for honesty: on that particular fixture Option A would coincidentally pass, since
all three drop rules are identical `drop any` and the unscoped ones are FORWARD. Coincidence of
the fixture, not a property of the design.

**Cost, measured rather than estimated.** The A/B delta is smaller than the raw 868 suggests:
`acl_port_scope_update()` 337 lines is B-only; `acl_entry_in_ports_get()` 90,
`acl_tbl_port_bindings_get()` 54 and the counter helpers 49 are common to both. Within those 337
the partition itself is ~40 lines -- the bulk is slot reuse, old/new diffing, dirty-port tracking
and error recovery. If B ever needs slimming, cut *there* (tear down and rebuild every scoped ACL
each reprogram: shorter, still correct, costs unbind/rebind churn on every mux toggle). Do not
cut the partition.

Decision unchanged: **keep Option B**, now resting on static shipping config rather than on an
orchagent race.

### The decisive evidence: MuxOrch and PFC watchdog SHARE one ACL table

Heterogeneous `IN_PORTS` was assumed hypothetical. It is not -- it is the shipping config:

    aclorch.h:117              #define INGRESS_TABLE_DROP "IngressTableDrop"
    muxorch.cpp:49             #define MUX_ACL_TABLE_NAME INGRESS_TABLE_DROP
    pfcactionhandler.cpp:322   m_strIngressTable = INGRESS_TABLE_DROP;

Same table, different rules, different `IN_PORTS`, different match fields:

| Rule | `IN_PORTS` | Match | Action |
|---|---|---|---|
| `mux_acl_rule` (`muxorch.cpp:1556`) | standby mux ports | **any** | DROP |
| `Rule_PfcWdAclHandler_<qid>` (`pfcactionhandler.cpp:318`) | storming ports | `TC = qid` | DROP |

The sharing is deliberate -- the builtin table type declares both fields:

    aclorch.cpp:4081   builder.withName(TABLE_TYPE_DROP)
            4084       .withMatch(SAI_ACL_TABLE_ATTR_FIELD_TC)        // PFC watchdog
            4085       .withMatch(SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS)  // both

This retroactively explains an oddity noted in section 1b: the live mux table carries `FIELD_TC`
even though the mux rule never matches TC.

    ASIC_STATE:SAI_OBJECT_TYPE_ACL_TABLE:oid:0x7000000000667
      SAI_ACL_TABLE_ATTR_FIELD_IN_PORTS   true
      SAI_ACL_TABLE_ATTR_FIELD_TC         true    <- only PFC watchdog uses this

### Decision: Option B

On any dualtor ToR running PFC watchdog -- a normal configuration -- Option A converts a PFC
storm on one port into a **full IPv4 blackhole on that port**: the storming port gets bound to
an ACL that also contains the mux `deny any`. It would fix the reported bug and introduce a
smaller bug of the same class. The 868 lines are not over-engineering; the shared
`IngressTableDrop` is precisely the case that requires them.

**Corollary -- the current bug is worse than reported.** A single PFC watchdog trigger, on a box
with **no dualtor at all**, installs a rule into `IngressTableDrop` which VPP then binds to every
port. That is a second, independent path to a box-wide outage and it needs no mux. *(Not yet
verified on the DUT -- candidate follow-up.)*

### Related, but not a substitute: the `default:` case

Making `acl_rule_field_update()`'s `default:` return failure for unhandled **match-field**
attributes is genuinely ~3 lines, and is what would have made this loud in 2023 instead of
silent. But it must ship *with* a real fix, not instead of one: alone it turns a silent
dataplane outage into a failed ACL-entry create, and orchagent's `handleSaiCreateStatus` may
abort on that -- a crash loop is not obviously better than a blackhole.


--------------------------------------------------------------------------------
## 4. THE CRITICAL GOTCHA -- rebinding must unbind/rebind the WHOLE port

`vpp_acl_interface_bind` -> VPP `acl_interface_add_del`, which **appends** (`vec_add1`). Live
proof on the DUT:

    vppctl show acl-plugin interface sw_if_index 25
      input acl(s): 2, 3, 1

`1` is `sonic_acl_default_permit` (permit any v4+v6), bound **last**. Anything bound *after* it
is dead code.

So a naive "unbind just this table's ACL, bind the new one" would land the new ACL **behind** the
permit-any and the mux drop would silently stop working -- a worse failure than the bug being
fixed. `acl_port_scope_update()` therefore unbinds and rebinds the **whole port** via the
existing, already-tested `aclBindUnbindPort(port, grp, is_input, false)` then `(..., true)`,
which re-establishes group members in priority order with the default permit last.

Cost: a sub-millisecond window where the port has no ACLs bound (traffic permitted). Benign
versus today's total blackhole.

Related: `vpp_acl_interface_bind` maps `VNET_API_ERROR_ACL_IN_USE_INBOUND` to success, so
re-binding an already-bound ACL is a no-op and does **not** move it in the order.

--------------------------------------------------------------------------------
## 5. Order of operations in `acl_port_scope_update()` (must not be reordered)

    1. program the default ACL                      (done earlier, in AclTblConfig)
    2. program/replace the scoped ACLs
    3. compute the new per-interface assignment
    4. diff vs the old assignment
    5. UNBIND changed ports  <-- while m_acl_port_scope still holds the OLD scope
    6. store new_scope
    7. BIND changed ports
    8. delete now-unused old scoped ACLs           <-- must be after unbind:
                                                  vpp_acl_del fails on a bound ACL

--------------------------------------------------------------------------------
## 6. Bugs found during review (2 rounds: self + rubber-duck subagent)

Seven issues surfaced. The three genuinely functional ones:

**(a) Missing the default ACL when EVERY entry is scoped.** This is exactly the mux table -- one entry,
and it is the scoped one. So `base_aces` is empty, `acl == NULL`, and with the table not yet in
`m_acl_swindex_map`, `acl_add_replace()` was skipped entirely. The default ACL never existed,
`acl_port_swindex_get()` then failed for all the *unscoped* ports, and `aclBindUnbindPort()`
returned `SAI_STATUS_FAILURE`, aborting binding for the whole port. Now routed to
`emptyAclCreate()`.

**(b) ip2me deny detection regressed.** I had changed `has_deny` from scanning generated rules to
scanning ACE attributes for `SAI_PACKET_ACTION_DROP`. But `VPP_ACL_ACTION_API_DENY == 0`
(`SaiVppXlate.h:80-84`) and rules come from `calloc` -- so an ACE with **no explicit packet
action still generates a deny rule**. The attribute scan would miss it, silently changing which
ports get the `sonic_ext` ip2me bypass. Reverted to the generated-rule scan and threaded
`_Inout_ bool &has_deny` through `acl_port_scope_update()` so scoped ACLs OR in. Semantics
now exactly match `dev`, extended to scoped ACLs.

**(c) Intent-diff masked failed binds.** `changed` was computed by diffing old vs new *intent*.
If a rebind failed, the scope was still committed -- so on retry old and new both said
"port A -> ACL Y", `changed` came back empty, no rebind happened, and **the retry returned
success with the port left unbound**. Fixed with a new `m_acl_tbl_dirty_ports`: any port whose
bind/unbind failed, or that could not be resolved, is recorded and forced through unbind/rebind
on the next reprogram even when its assignment looks unchanged. A port whose *unbind* failed is
additionally skipped in the bind loop, since binding onto an uncleared list would append out of
order.

Also fixed: swindex leaks on the `acl_rules.empty()` and error paths; failed `vpp_acl_del()` now
retains ownership (tracked under an **empty signature**, which the grouping loop can never
produce, so it can never be reused or rebound -- only retried); stale counter placements cleared
on the error path; `AclTblRemove()` retains undeleted scoped ACLs and propagates failure instead of
erasing tracking.

--------------------------------------------------------------------------------
## 7. Code changes (`fix_vpp_acl_in_ports`, +868 / -60)

`vslib/vpp/SwitchVpp.h` (+160):

    vpp_ace_placement_t                 // an ACE can now live in several VPP ACLs
    vpp_ace_cntr_info_t::placements     // was flat {acl_index, base_index, num_rules}
    typedef struct _vpp_acl_port_scope_t { port_swindex; scoped_acls; }
    m_acl_port_scope                    // tbl_oid -> AclPortScope
    m_acl_tbl_dirty_ports               // tbl_oid -> ports needing forced reconciliation

`vslib/vpp/SwitchVppAcl.cpp` (+708), new methods:

    acl_entry_in_ports_get()      two-step get(); ITEM_NOT_FOUND == absent, all other
                                  statuses propagate (a failed read must NOT silently
                                  widen a scoped DROP to every port)
    acl_port_swindex_get()        per-interface swindex, falling back to the default ACL
    acl_tbl_port_bindings_get()   enumerate (port_oid, tbl_grp_oid, is_input)
    acl_port_scope_update()       the core: group, program, diff, unbind/rebind, reap
    acl_ace_cntr_info_update()    accumulates placements (factored out of acl_add_replace)
    acl_ace_cntr_info_clear()

changed:

    acl_rule_field_update()   explicit IN_PORTS case -> break with a pointer to the
                              binding layer (stops the bogus error log)
    AclTblConfig()            splits ACEs into base_aces vs ace_hwifs; emptyAclCreate()
                              when all entries are scoped; calls acl_port_scope_update()
    AclTblRemove()            clears counters + dirty ports, deletes scoped ACLs, retains
                              undeleted ones and propagates failure
    aclBindUnbindPort()       resolves swindex PER INTERFACE
    aclBindUnbindPorts()      same
    aclGetVppIndices()        placements.front(), guards on empty
    getAclEntryStats()        sums counters across ALL placements

`vslib/vpp/SwitchVppAcl.h` is **unchanged** -- the design deliberately avoided needing a new
field in `acl_tbl_entries_t` / `ordered_ace_list_t`.

--------------------------------------------------------------------------------
## 8. Deliberately deferred

- **Full transactional staging** under fresh swindices. Would double peak ACL usage and is a much
  larger redesign. Consequence: if a scoped ACL fails after the default ACL succeeded, some ports enforce
  new rules and others old until the next reprogram.
- **`acl_interface_set_acl_list`** wrapper -- removes the unbind/rebind window, needs a new VPP
  API binding.
- **Per-interface ip2me state.** `m_ip2me_drop_tables` is table-keyed in `dev`; re-keying it is a
  separate change. A standby-only DROP therefore marks the whole table as a drop table and
  enables the ip2me bypass on every bound port. Fails safe (ip2me only lets *for-us* traffic
  bypass ACLs), but it is a real over-approximation.

--------------------------------------------------------------------------------
## 9. Side findings

- `count_tunterm_acl_rules()` is **dead code** -- never called, so `ordered_ace_list_t::is_tunterm`
  is always false and `fill_acl_rules` always takes the regular branch. Pre-existing; left alone.
  Consequence for this change: the ACE filtering cannot affect tunterm.
- `m_acl_tbl_hw_ports_map` is maintained by `aclBindUnbindPorts` but **not** by
  `aclBindUnbindPort`. The rescope path uses the latter, so that map is untouched -- consistent
  with existing behavior.
- `SAI/meta` generation needs `doxygen`, `perl` **and `dot`** on PATH (`SAI/meta/Makefile:68`);
  graphviz was missing in the dev container.

--------------------------------------------------------------------------------
## 10. Status

Compiles clean under `-Wall -Wextra`:

    g++ -fsyntax-only -std=c++14 -Wall -Wextra -Wno-unused-parameter \
        -I . -I vslib -I vslib/vpp -I lib -I syncd \
        -I SAI/inc -I SAI/experimental -I SAI/meta -I SAI/custom -I <swss-common> \
        vslib/vpp/SwitchVppAcl.cpp

Every file in `vslib/vpp/` passes except the pre-existing, unrelated `libnl` include failure in
`SwitchVppHostif.cpp` (`netlink/route/link.h` not installed).

The **bug** is fully reproduced and measured on the DUT (section 1b: 1 standby port -> deny on
32/32 front-panel ports, all server IPv4 blackholed, ARP and uplink BGP unaffected).

The **fix** is **not yet committed and not yet runtime-tested** -- no verification on the DUT
that a mux switchover produces the expected `acl-index` layout. Design settled on **Option B
(partition)** as of 2026-08-19; the cheaper union approach is rejected, see section 3b.
Expected after the fix, on the section 1b run (Ethernet4 standby, 23 active):

    vppctl show acl-plugin acl                     -> one scoped ACL beyond baseline,
                                                      deny applied inbound on sw_if_index: 2 only
    vppctl show acl-plugin interface sw_if_index 2  -> deny present, default permit last
    vppctl show acl-plugin interface sw_if_index 3  -> placeholder only, no deny
    ping 192.168.0.3 from the DUT                   -> succeeds

Testbed restore procedure (a plain `auto` did not reconverge -- 22 active / 2 standby with a
residual deny): force `active`, then `auto`, then verify 24 active / 24 `auto` / **0** residual
deny ACLs.
