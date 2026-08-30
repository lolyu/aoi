# VPP debug ACL

How the VPP ACL plugin works, how to read every `show acl-plugin` command, and how
SONiC's SAI ACL model maps (and fails to map) onto it.

Source references are to `vpp/src/plugins/acl/`. Live captures are from
`vlab-vpp-03` (dualtor T0, `Force10-S6000`), 2026-08-19, taken while
`Ethernet4` was the only standby mux port.

## Quick reference

```
# vppctl show acl-plugin acl                 # logical view: ACLs, tags, rules
# vppctl show acl-plugin lookup context      # BEST: iface + direction + ORDERED acl list
# vppctl show acl-plugin interface           # per-interface bindings
# vppctl show acl-plugin tables              # compiled view: masks, hitcounts, collisions
# vppctl show acl-plugin sessions            # stateful sessions (action 2 = permit+reflect)
# vppctl show acl-plugin decode 5tuple ...   # decode 6 hex words into a 5-tuple
# vppctl show interface
```

---

## 1. The object model

VPP's model is **much flatter** than SONiC's or a classic ASIC's. There are exactly
three concepts:

```
   ACL  (a numbered list of rules)          <- the only "container" that exists
    |
    +-- rule / ACE  (5-tuple match + action)
    |
    +-- binding: (sw_if_index, direction) -> ordered list of ACL indices
                    |
                    +-- lookup context (lc_index)   <- the compiled artifact
```

**There is no "ACL table" object in VPP.** What SONiC calls an *ACL table* becomes a
VPP *ACL*; what SONiC calls an *ACL entry/rule* becomes a VPP *rule* inside that ACL.
There is no third level.

### 1.1 The ACL

A pool object with an index (`acl-index`), a free-form 64-char `tag`, and a vector of
rules.

| Property | Consequence |
|---|---|
| The index is a **pool index**, reused after deletion | Never cache an index across a delete |
| An ACL **cannot be empty** | Zero rules is rejected by the API. This is why SONiC's `emptyAclCreate()` injects a placeholder rule |
| The `tag` is the only identity slot | SONiC writes `sonic_acl_oid:0x700000000` — a **RID**, not the ASIC_DB VID |

Bridge tag RID to ASIC_DB VID with:

```bash
redis-cli -n 1 HGET VIDTORID oid:<vid>
```

### 1.2 The rule (ACE)

Printed by `acl_print_acl_x()` (`acl.c:143-183`):

```
      0: ipv4 permit src 0.0.0.0/0 dst 0.0.0.0/32 proto 0 sport 0 dport 0
      |   |     |                                  |         |
      |   |     |                                  |         +- L4 port ranges (first[-last])
      |   |     |                                  +- IP protocol
      |   |     +- action
      |   +- address family
      +- rule index within the ACL (9-wide, right-aligned)
```

Match fields, and **only** these fields:

| Field | Notes |
|---|---|
| `is_ipv6` | v4 and v6 **never** cross-match: `if (is_ip6 != r->is_ipv6) continue;` (`public_inlines.h:301`) |
| src prefix, dst prefix | |
| `proto` | **`0` means "any" AND silently disables the port fields** |
| sport / dport ranges | first-last, inclusive. A `-N` suffix prints only when first != last |
| TCP flags value + mask | printed only when mask or value is nonzero |

**The `proto 0` trap** is the #1 misreading of this output. Both engines gate L4
behind a nonzero proto:

```c
if (r->proto)          /* public_inlines.h:321  -- linear engine */
if (r->proto != 0)     /* hash_lookup.c:987     -- hash engine   */
```

So `proto 0 sport 80 dport 443` matches **everything**. The ports are dead text.

**Actions** (`format_acl_action()`, `acl.c:123-140`):

| Value | Meaning |
|---|---|
| `0` | deny |
| `1` | permit (stateless) |
| `2` | permit + **reflect** (stateful — creates a session for the return flow) |

### 1.3 What is NOT a match field

**There is no interface / ingress-port field in a VPP ACL rule.** Not src port, not
port-group, nothing.

This is the single most important structural fact about the plugin. Ingress-port
scoping can only be expressed at the **binding** layer — one VPP ACL per distinct
port-set. See section 6.

### 1.4 The binding

```
sw_if_index + direction (input|output)  ->  ordered vector of acl-index
```

Two facts that bite:

1. **Order is significant and binding *appends*.** `acl_interface_add_del` does a
   `vec_add1`. Live on the DUT: `acl_indices: 2, 3, 1` — the default-permit ACL is
   **last**, so anything naively bound afterwards lands *after* permit-any and is dead
   code. To insert in the middle you must unbind and rebind the whole port.
2. Input and output are independent lists.

### 1.5 The lookup context (`lc_index`)

The compiled, flattened form of one binding. When you bind `[acl 2, acl 3, acl 1]` to
an interface, the plugin **concatenates all their rules in order** into a single flat
array and compiles that into the hash tables. The dataplane then does exactly one
lookup against one `lc_index`.

This is also a public API ("ACL-as-a-service") — other VPP features (`abf`, `nat`)
allocate lookup contexts too, which is why the plugin distinguishes lookup **users**
from lookup **contexts**.

---

## 2. Match semantics

```
for each ACL in the interface's ordered list:
    for each rule in that ACL, in order:
        if rule matches:  apply its action, STOP.
if nothing matched:  DENY
```

Two consequences:

- **First match wins, globally across the concatenated list.** A permit-any early in
  the list makes everything after it unreachable.
- **The implicit default is deny**, not permit:
  `action = 0; /* deny by default */` (`dataplane_node.c:463`).
  This is why SONiC has to inject a `sonic_acl_default_permit` ACL at all.

---

## 3. The two matching engines

`Use hash-based lookup for ACLs: 1` in the output tells you which is active.

**Linear** (`use_hash_acl_matching = 0`): walk the rules one at a time. Simple, O(n),
only used as a fallback — notably for **non-first IP fragments**, which tuple-merge
cannot handle.

**Hash / TupleMerge** (the default): the problem it solves is that ACL rules have
wildcards and hashes do not. TupleMerge's answer:

1. Group rules by their **mask** (which bits are actually significant).
2. For each distinct mask, hash `(lc_index, masked-5-tuple, mask_type_index)` into one
   shared bihash.
3. At match time, for each candidate mask: apply it to the packet, look up, done.
4. **Relax** masks (widen them) so similar rules share a mask — fewer masks means
   fewer lookups per packet. Rules that then collide under the relaxed mask are
   chained and re-checked exactly.

The `lc_index` is part of the hash key, which is how one global table serves every
interface.

### 3.1 The 48-byte key (`fa_5tuple_t`, `fa_node.h:68-90`)

Everything printed as six hex words is this struct:

| Word | Bytes | Contents |
|---|---|---|
| `key[0..3]` | 0-31 | L3. For **IPv6**: `ip6_addr[2]`. For **IPv4**: 24 bytes of zero pad, then `ip4_addr[2]` — so **v4 addresses live in `key[3]` only**: low half = src, high half = dst |
| `key[4]` | 32-39 | `fa_session_l4_key_t` — ports, proto, l4 flags |
| `value` | 40-47 | `fa_packet_info_t` — `lc_index`, `mask_type_index_lsb`, tcp_flags, and the `is_ip6` / `l4_valid` / `is_nonfirst_fragment` bits |

Worked decode of the three masks captured on the DUT:

```
0: ...  ffffffff00000000  0000000000000000  0800ffffffffffff   refcount 2
1: ...  c0ffffff00000000  0000000000000000  0800ffffffffffff   refcount 32
2: ...  0000000000000000  0000000000000000  0800ffffffffffff   refcount 107
        ^ key[3]: v4 addrs ^ key[4]: L4      ^ value: pkt info
```

- **`key[3] = ffffffff00000000`** -> LE bytes at offsets 24-31 are
  `00 00 00 00 | ff ff ff ff` -> **src /0, dst /32**. Exactly `dst 0.0.0.0/32` — the
  `emptyAclCreate()` placeholder in ACLs 0 and 2.
- **`key[3] = c0ffffff00000000`** -> `00 00 00 00 | ff ff ff c0` -> src /0, **dst /26**.
  This is mask 0 after **relaxation**: TupleMerge widened /32 to /26 to merge partitions.
- **`key[3] = 0`** -> full wildcard: ACL 1 (permit any) and ACL 3 (deny any).
- **`key[4] = 0` in all three** -> the L4 word does not participate. That is `proto 0`
  doing exactly what section 1.2 describes.
- **`value = 0800ffffffffffff`** -> LE bytes `ff ff ff ff | ff ff | 00 | 08` =
  `lc_index` fully masked (32 bits), `mask_type_index_lsb` fully masked (16 bits),
  tcp_flags not matched, flags byte `0x08` = bit 3 = **`is_ip6`**. That single bit is
  the mechanical reason v4 and v6 can never cross-match.

Note the built-in decoder renders these as IPv6 (`::ffff:ffc0`) because a bare mask
carries no family context — mentally read the trailing 32 bits as the v4 dst mask:

```
vpp# show acl-plugin decode 5tuple 0 0 0 c0ffffff00000000 0 0800ffffffffffff
5-tuple structure decode: lc_index -1 l3 ip6 :: -> ::ffff:ffc0 l4 lsb_of_sw_if_index 0
proto 0 l4_is_input 0 l4_slow_path 0 l4_flags 0x00 port 0 -> 0 tcp flags (invalid) 00 rsvd 0
```

### 3.2 Verifying `refcount`

`refcount` = base references + applied references. It checks out exactly on the DUT:

| Mask | Base refs | Applied refs | Total | Printed |
|---|---|---|---|---|
| 0 (dst /32) | ACL0 r0, ACL2 r0 = 2 | 0 (all relaxed to mask 1) | 2 | **2** |
| 1 (dst /26) | 0 | ACL0 x 4 lc + ACL2 x 28 lc = 32 | 32 | **32** |
| 2 (wildcard) | ACL1 r0/r1, ACL3 r0 = 3 | ACL1 x 36 lc x 2 + ACL3 x 32 lc = 104 | 107 | **107** |

---

## 4. The commands

### 4.1 Inspection

| Command | What it gives you |
|---|---|
| `show acl-plugin acl [index N]` | The **logical** view: every ACL, its tag, its rules in human-readable form, plus reverse indexes of where it is applied. Start here. |
| `show acl-plugin interface [sw_if_index N] [acl]` | Per-interface view: which ACLs are on which interface, in **correct order**. `acl` suffix adds rule dumps. |
| `show acl-plugin lookup context [index N]` | **The single most useful command.** One line per context: `index 4: interface ACL sw_if_index: 7 is_input: 1, acl_indices: 2, 3, 1`. Owner + interface + direction + **ordered** ACL list, all in one place. |
| `show acl-plugin lookup user [index N]` | Which subsystems registered as lookup-context users (`interface ACL`, `abf`, ...). Rarely needed. |
| `show acl-plugin tables [acl [index N] \| applied [lc_index N] \| mask \| hash [verbose N]]` | The **compiled** view: mask pool, per-ACL bitmask-ready layout, per-context applied entries with **hitcounts**, and the raw bihash. |
| `show acl-plugin decode 5tuple W W W W W W` | Decodes six hex words into a readable 5-tuple. The tool for reading mask rows. |
| `show acl-plugin sessions` | Stateful sessions created by `permit+reflect` (action 2). Empty if only stateless rules are used. |
| `show acl-plugin memory` | Heap usage of the plugin's private mheap. |
| `show acl-plugin macip acl` / `macip interface` | The separate MAC+IP ACL feature (L2 ingress anti-spoof). Unrelated to IP ACLs; SONiC does not use it. |

### 4.2 Mutation (debug/lab only — SONiC drives all of this over the binary API)

| Command | Effect |
|---|---|
| `set acl-plugin acl <permit\|deny\|...> ...` | Create/replace an ACL from the CLI |
| `delete acl-plugin acl index <idx>` | Delete an ACL — **fails if still bound** |
| `set acl-plugin interface <intf> <input\|output> acl <IDX> [del]` | Bind / unbind. **Appends** on add. |
| `set acl-plugin session timeout ...` | Stateful session timers (udp idle, tcp idle/transient) |
| `clear acl-plugin sessions` | Flush the stateful session table |

### 4.3 `show acl-plugin tables`, section by section

```
Stats counters enabled for interface ACLs: 1     <- acl.c:3646
Use hash-based lookup for ACLs: 1                <- acl.c:3648
```

**Section 1 — `Mask-type entries:`** (`hash_lookup.c:1130`) — the global mask pool,
decoded in section 3.1 above.

**Section 2 — `Mask-ready ACL representations`** (`hash_lookup.c:1147`) — per ACL, the
compiled rules:

```
acl-index 1 bitmask-ready layout
  applied lc_index list: 32, 0, 1, 2, 10, ...
       0: ... 0000000200000000 base mask index 2 acl 1 rule 0 action 1
```

`base_mask_type_index` is the mask **before** relaxation. Note the match word
`0000000200000000`: LE bytes `00 00 00 00 | 02 00 | 00 | 00` -> `mask_type_index_lsb = 2`.
The mask index is literally baked into the key.

> **`applied lc_index list` is unordered.** It is maintained with `vec_add1` /
> `vec_del1`, and `_vec_del1` (`vppinfra/vec.h:990-999`) is a **swap-remove**:
> `clib_memcpy_fast (v + index * elt_sz, v + len * elt_sz, elt_sz)` moves the *last*
> element into the freed slot. Treat it as a set; sort before diffing. The same
> artifact affects `applied inbound on sw_if_index:` in `show acl-plugin acl`.

**Section 3 — `Applied lookup entries for lookup contexts`** (`hash_lookup.c:1205`) —
per context:

```
lc_index 10:
  applied acls: 2, 3, 1                              <- ORDERED (unlike the above)
  applied mask info entries:
       0: mask type index 1 first rule index 0 num_entries 1 max_collisions 1
       1: mask type index 2 first rule index 1 num_entries 3 max_collisions 2
  lookup applied entries:
       0: acl 2 rule 0 action 1 ... mask type index: 1 colliding_rules: 1 ... hitcount 0 acl_pos: 0
           0: acl 2 ace 0 acl pos 0 pae index: 0
       1: acl 3 rule 0 action 0 ... mask type index: 2 colliding_rules: 2 ... hitcount 0 acl_pos: 1
           0: acl 3 ace 0 acl pos 1 pae index: 1
           1: acl 1 ace 0 acl pos 2 pae index: 2
```

- `applied mask info entries` = the per-context list of masks the dataplane must try,
  each with how many rules use it and the worst collision-chain depth.
  **This is the ACL-scale health metric**: growing `max_collisions` = degrading lookup
  performance.
- `lookup applied entries` = the flat, priority-ordered rule array. `acl_pos` is the
  position of the owning ACL in the binding list.
- The **indented lines are the collision chain** — rules that share a relaxed mask and
  must be re-checked exactly. Entry 1 above shows `acl 3 ace 0` and `acl 1 ace 0`
  colliding: deny-any and permit-any both reduce to the wildcard mask, so the engine
  disambiguates by position. Deny is at `acl_pos 1`, permit at `acl_pos 2` —
  **deny wins**.

**Section 4** — the raw bihash dump (`hash` / `verbose N`).

---

## 5. `hitcount` — per-rule hit counters

There are two independent counter mechanisms, and they are easy to confuse:

| | Stat-segment counters | `hitcount` |
|---|---|---|
| Where | `/acl/<index>/matches`, per-rule per-thread | inside each applied-hash-entry |
| Read via | `vpp_get_stats` / `show stats` | **`show acl-plugin tables applied`** |
| Gated on | `interface_acl_counters_enabled` (`dataplane_node.c:472`) | **nothing** — `pae->hitcount++` is unconditional (`public_inlines.h:630`) |
| Available when | flag is set (SONiC sets it via `acl_stats_intf_counters_enable`) | always, but **hash engine only** |
| Reset when | — | the entry is recompiled (`hash_lookup.c:688`) — i.e. **any rebind zeroes it** |

That last row matters: hitcounts are *since last recompile*, not since boot. Use
**deltas**, not absolutes.

### 5.1 Worked example — the dualtor IN_PORTS bug

Sweeping every context on the DUT, per-ACL totals:

```
acl 0 total hits 0          <- emptyAclCreate() placeholder, never matches
acl 2 total hits 0          <- the other placeholder, never matches
acl 1 total hits 521538     <- sonic_acl_default_permit, carrying everything
acl 3 total hits 0          <- the mux drop ACL (ipv4 deny any->any)
```

The front-panel context for the standby port (`lc_index 10` = sw_if_index 2 =
**Ethernet4**):

```
  applied acls: 2, 3, 1
       0: acl 2 rule 0 action 1  hitcount 0      acl_pos: 0    <- placeholder, unmatchable
       1: acl 3 rule 0 action 0  hitcount 0      acl_pos: 1    <- ipv4 deny any->any
       3: acl 1 rule 1 action 1  hitcount 24     acl_pos: 2    <- only the IPv6 permit fires
```

Read that carefully — it is the whole bug in four lines:

- ACL 2 (placeholder, `dst /32`) **can never match**, so it protects nothing.
- ACL 3 is therefore **the first matchable rule** on this port.
- The only nonzero counter is `acl 1 rule 1` — the **IPv6** permit. `acl 1 rule 0`
  (the IPv4 permit) reads **0** on every front-panel port while reading 64k+ on the
  uplink LAGs. IPv4 never reaches it because the deny is in front of it.
- `acl 3` reading 0 is **not** "the deny is idle" — it is
  **"there is no IPv4 left to count"**. The traffic was already gone; hitcounts were
  zeroed by the last rebind and nothing has been offered since.

Why BGP survived: **`acl 3` is absent from `lc_index 0-3`** (sw_if_index 65-68, the
`BondEthernet` uplinks). Those are `Port::LAG`, and MuxOrch's `bindAllPorts()` only
walks `Port::PHY`. Hence 64537 IPv4 permit hits on the uplinks and zero on the front
panel.

---

## 6. How SONiC maps onto this

| SONiC / SAI | VPP | Fit? |
|---|---|---|
| ACL table | **one VPP ACL** | a level gets collapsed |
| ACL entry | one VPP rule | ok |
| Table binding to ports | binding to each `sw_if_index` | ok |
| Entry priority | position in the rule vector | ok |
| **`SAI_ACL_ENTRY_ATTR_FIELD_IN_PORTS`** | **nothing** | **broken — see below** |

`SwitchVppAcl.cpp`'s `acl_rule_field_update()` hits `default:` for attribute `4106`
(`SAI_ACL_ENTRY_ATTR_FIELD_START + 0xa`), logs an error, and **still returns success**
because `status` was pre-initialised to `SAI_STATUS_SUCCESS`.
`grep -rn IN_PORTS vslib/` returns nothing at all.

So the mux drop rule — which SONiC authors as *"drop IPv4 arriving on Ethernet4
specifically"* — loses its port qualifier and becomes a bare `ipv4 deny any->any`,
bound to all 32 PHY ports by `bindAllPorts()`. That produces the **exact inverse of
intent**: one standby port's rule blackholes IPv4 on the entire box.

Because VPP rules have no interface field (section 1.3), the fix must express the
scoping at the **binding** layer: partition ports by their applicable-entry set and
program one VPP ACL per distinct set.

---

## 7. Practical recipes

```bash
# 1. What is configured, logically?
vppctl show acl-plugin acl

# 2. What is actually bound where, in what ORDER?  (best single command)
vppctl show acl-plugin lookup context

# 3. Which rules are firing?  (deltas, not absolutes)
vppctl show acl-plugin tables applied | grep -E '^lc_index|hitcount'

# 4. One context in detail
vppctl show acl-plugin tables applied lc_index 10

# 5. Per-ACL hit totals across every context
vppctl show acl-plugin tables applied | awk '
  /acl [0-9]+ rule [0-9]+ action/ {
    for (i=1;i<=NF;i++) if ($i=="hitcount") h=$(i+1); t[$3]+=h
  } END { for (a in t) print "acl", a, "hits", t[a] }'

# 6. Decode a mask row
vppctl show acl-plugin decode 5tuple 0 0 0 c0ffffff00000000 0 0800ffffffffffff

# 7. Lookup-table health
vppctl show acl-plugin tables applied | grep max_collisions
```

Inside SONiC, prefix with `docker exec syncd`:

```bash
docker exec syncd vppctl show acl-plugin lookup context
```

---

## 8. Gotchas

- `proto 0` means the port fields are **ignored entirely**.
- The implicit default action is **deny**.
- Binding **appends**; to reorder you must unbind and rebind the whole port.
- `applied inbound on sw_if_index` and `applied lc_index list` are **sets**, not
  sequences (swap-remove artifact). `applied acls:` inside a context **is** ordered.
- ACL indices are **reused** after delete.
- Hitcounts **reset on every recompile** (any rebind).
- v4 and v6 rules never cross-match — it is a single bit in the hash key.
- Packet trace is **unusable** on SONiC VPP's `bobm*` interfaces: `bond-input` errors
  with *"doesn't support per-node tracing"*, and `af-packet-input` / `virtio-input`
  accept `trace add` but capture nothing. Use hitcounts instead. See
  `vpp_trace_packet.md`.

---

## 9. Source map

| Item | Location |
|---|---|
| `acl_print_acl_x()` — rule formatting | `acl.c:143-183` |
| `format_acl_action()` — 0/1/2 | `acl.c:123-140` |
| Per-interface ordered ACL vector (`vec_dup`) | `acl.c:691-692` |
| Reverse index `input_sw_if_index_vec_by_acl` | `acl.c:3241` |
| `Stats counters enabled ...` header | `acl.c:3646` |
| `Use hash-based lookup ...` header | `acl.c:3648` |
| `acl_stats_intf_counters_enable_disable()` | `acl.c:584-591` |
| v4/v6 gate | `public_inlines.h:301` |
| linear-engine proto gate | `public_inlines.h:321` |
| `pae->hitcount++` | `public_inlines.h:630` |
| hash-engine proto gate | `hash_lookup.c:987` |
| `relax_tuple()` — mask widening | `hash_lookup.c:191` |
| `hitcount` reset on recompile | `hash_lookup.c:688` |
| Mask-type dump | `hash_lookup.c:1123-1138` |
| Bitmask-ready ACL dump | `hash_lookup.c:1140-1169` |
| Applied-entry / collision-chain dump | `hash_lookup.c:1171-1247` |
| `fa_5tuple_t` key layout | `fa_node.h:68-90` |
| Default-deny | `dataplane_node.c:463` |
| Stat-segment counter gate | `dataplane_node.c:472` |
| TupleMerge design doc | `acl_hash_lookup_doc.rst` |