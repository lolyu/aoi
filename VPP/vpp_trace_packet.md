# VPP trace packet

```
vpp# trace add dpdk-input 10
vpp# show trace
vpp# show trace
------------------- Start of thread 0 vpp_main -------------------
No packets in trace buffer
------------------- Start of thread 1 vpp_wk_0 -------------------
Packet 10

03:21:53:937423: dpdk-input
bobm28 rx queue 0
buffer 0x149cf3: current data 0, length 62, buffer-pool 0, ref-count 1, trace handle 0x1000009
ext-hdr-valid
PKT MBUF: port 28, nb_segs 1, pkt_len 62
buf_len 2176, data_len 62, ol_flags 0x0, data_off 128, phys_addr 0x6b873d40
packet_type 0x0 l2_len 0 l3_len 0 outer_l2_len 0 outer_l3_len 0
rss 0x0 fdir.hi 0x0 fdir.lo 0x0
IP4: 3e:38:be:ea:b0:1c -> 22:59:d0:d5:19:a9
IP_IN_IP: 10.10.10.10 -> 192.168.0.1
tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
fragment id 0x0001
03:21:53:937456: bond-input
src 3e:38:be:ea:b0:1c, dst 22:59:d0:d5:19:a9, bobm28 -> BondEthernet101
03:21:53:937466: ethernet-input
IP4: 3e:38:be:ea:b0:1c -> 22:59:d0:d5:19:a9
03:21:53:937471: ip4-input
IP_IN_IP: 10.10.10.10 -> 192.168.0.1
tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
fragment id 0x0001
03:21:53:937475: acl-plugin-in-ip4-fa
acl-plugin: lc_index: 0, sw_if_index 65, next index 1, action: 1, match: acl 1 rule 0 trace_bits 00000000
pkt info 0000000000000000 0000000000000000 0000000000000000 0100a8c00a0a0a0a 0041030400000000 0200ffff00000000
lc_index 0 l3 ip4 10.10.10.10 -> 192.168.0.1 l4 lsb_of_sw_if_index 65 proto 4 l4_is_input 1 l4_slow_path 1 l4_flags 0x03 port 0 -> 0 tcp flags (invalid) 00 rsvd 0
03:21:53:937483: ip4-lookup
fib 0 dpo-idx 54 flow hash: 0x00000000
IP_IN_IP: 10.10.10.10 -> 192.168.0.1
tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
fragment id 0x0001
03:21:53:937492: ip4-receive
fib:0 adj:54 flow:0x00000000
IP_IN_IP: 10.10.10.10 -> 192.168.0.1
tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
fragment id 0x0001
03:21:53:937500: ipip4-input
IPIP: tunnel 76 len 48 src 10.10.10.10 dst 192.168.0.1
03:21:53:937510: ip4-input
ICMP: 20.20.20.20 -> 20.20.20.20
tos 0x00, ttl 64, length 28, checksum 0x2a91 dscp CS0 ecn NON_ECN
fragment id 0x0001
ICMP echo_request checksum 0xf7ff id 0
03:21:53:937510: ip4-not-enabled
fib:0 adj:1 flow:0x00000000
ICMP: 20.20.20.20 -> 20.20.20.20
tos 0x00, ttl 64, length 28, checksum 0x2a91 dscp CS0 ecn NON_ECN
fragment id 0x0001
ICMP echo_request checksum 0xf7ff id 0
03:21:53:937513: error-drop
rx:ipip0
03:21:53:937514: drop
ip4-local: unknown ip protocol
```

## P2MP tunnel miss — no TEIB entry (tunnel -1)

`ipip4-input: tunnel -1` means the P2P hash lookup `{src=10.10.10.10, dst=192.168.0.1}` found
no match.  This happens when the P2MP tunnel exists but no TEIB entry has been added for the
peer `10.10.10.10`, so the per-peer P2P key is absent from the tunnel DB.

```
00:18:48:250203: dpdk-input
  bobm28 rx queue 0
  buffer 0x15b828: current data 0, length 62, buffer-pool 0, ref-count 1, trace handle 0x1000005
                   ext-hdr-valid
  PKT MBUF: port 28, nb_segs 1, pkt_len 62
    buf_len 2176, data_len 62, ol_flags 0x0, data_off 128, phys_addr 0x838e0a80
    packet_type 0x0 l2_len 0 l3_len 0 outer_l2_len 0 outer_l3_len 0
    rss 0x0 fdir.hi 0x0 fdir.lo 0x0
  IP4: 3e:38:be:ea:b0:1c -> 22:59:d0:d5:19:a9
  IP_IN_IP: 10.10.10.10 -> 192.168.0.1
    tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
    fragment id 0x0001
00:18:48:250208: bond-input
  src 3e:38:be:ea:b0:1c, dst 22:59:d0:d5:19:a9, bobm28 -> BondEthernet101
00:18:48:250208: ethernet-input
  IP4: 3e:38:be:ea:b0:1c -> 22:59:d0:d5:19:a9
00:18:48:250209: ip4-input
  IP_IN_IP: 10.10.10.10 -> 192.168.0.1
    tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
    fragment id 0x0001
00:18:48:250209: acl-plugin-in-ip4-fa
  acl-plugin: lc_index: 0, sw_if_index 65, next index 1, action: 1, match: acl 1 rule 0 trace_bits 00000000
  pkt info 0000000000000000 0000000000000000 0000000000000000 0100a8c00a0a0a0a 0041030400000000 0200ffff00000000
   lc_index 0 l3 ip4 10.10.10.10 -> 192.168.0.1 l4 lsb_of_sw_if_index 65 proto 4 l4_is_input 1 l4_slow_path 1 l4_flags 0x03 port 0 -> 0 tcp flags (invalid) 00 rsvd 0
00:18:48:250210: ip4-lookup
  fib 0 dpo-idx 54 flow hash: 0x00000000
  IP_IN_IP: 10.10.10.10 -> 192.168.0.1
    tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
    fragment id 0x0001
00:18:48:250210: ip4-receive
    fib:0 adj:54 flow:0x00000000
  IP_IN_IP: 10.10.10.10 -> 192.168.0.1
    tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
    fragment id 0x0001
00:18:48:250211: ipip4-input
  IPIP: tunnel -1 len 48 src 10.10.10.10 dst 192.168.0.1
00:18:48:250212: error-drop
  rx:BondEthernet101
00:18:48:250212: drop
  ipip4-input: no tunnel
```

## Inner-FIB drop after successful decap — class-E special drop

### Trace (excerpt of interest)

```
00:46:59:532738: ipip4-input
  IPIP: tunnel 85 len 120 src 1.1.1.1 dst 10.1.0.32
00:46:59:532754: ip4-input
  TCP: 1.1.1.1 -> 240.0.0.0
    tos 0x18, ttl 40, length 100, checksum 0xa079
00:46:59:532754: ip4-validate
  IP4-VALIDATE: sw_if_index 85 next 1 src 1.1.1.1 dst 240.0.0.0
00:46:59:532755: ip4-lookup
  fib 0 dpo-idx 0 flow hash: 0x00000000
00:46:59:532756: ip4-drop
    fib:0 adj:0 flow:0x00000000
00:46:59:532761: error-drop
  rx:ipip10
00:46:59:532765: drop
  ip4-local: unknown ip protocol
```

### Symptom
Outer packet `1.1.1.1 → 10.1.0.32` is decapsulated by IPIP tunnel 85; the inner packet
`1.1.1.1 → 240.0.0.0` is then dropped.

### Root cause
Inner `ip4-lookup` in fib 0 returns **`dpo-idx 0`** — the universal drop DPO. The default
route `0.0.0.0/0` is healthy in fib 0 (`dpo-load-balance index 1`, 4 ECMP buckets) and is
NOT what was matched. A more-specific prefix wins: VPP installs `240.0.0.0/4 → drop` as
a built-in special entry in every IPv4 FIB.

### Evidence

1. **`show ip fib` confirms a healthy default route**

   ```
   0.0.0.0/0
     [@0]: dpo-load-balance: [proto:ip4 index:1 buckets:4 ...]
       [0] ipv4 via 10.0.0.57 BondEthernet101
       [1] ipv4 via 10.0.0.59 BondEthernet102
       [2] ipv4 via 10.0.0.61 BondEthernet103
       [3] ipv4 via 10.0.0.63 BondEthernet104
   ```
   If the default route were being used, the trace would show `dpo-idx 1`, not `dpo-idx 0`.

2. **Built-in special drops in the FIB source** — `vpp/src/vnet/fib/ip4_fib.c`:

   ```c
   {
       /* 240.0.0.0/4 — drop class E */
       .ift_prefix = { .ip4.data_u32 = 0xf0000000, .fp_len = 4, ... },
       .ift_source = FIB_SOURCE_SPECIAL,
       .ift_flag   = FIB_ENTRY_FLAG_DROP,
   },
   {
       /* 224.0.0.0/4 — drop all mcast */
       .ift_prefix = { .ip4.data_u32 = 0xe0000000, .fp_len = 4, ... },
       .ift_source = FIB_SOURCE_SPECIAL,
       .ift_flag   = FIB_ENTRY_FLAG_DROP,
   },
   ```

   Installed into every IPv4 FIB at table creation by `ip4_fib_hash_load_specials()`.

3. **Longest-prefix match** — `240.0.0.0/4` (len 4) beats `0.0.0.0/0` (len 0), so the
   lookup resolves to `dpo-drop` (the global drop DPO at index 0).

### How the trace pointed to it (method)

1. **Read bottom-up.** Terminal nodes: `ip4-drop` → `error-drop rx:ipip10` → `drop "ip4-local: unknown ip protocol"`.
2. **Trust the structural fields, not the error string.** `ip4-drop` with `adj:0` means
   the FIB lookup matched a drop DPO. The error string `"ip4-local: unknown ip protocol"`
   is just a counter-slot label and is misleading here.
3. **Find the decision point.** Walk up to the preceding `ip4-lookup`: `fib 0 dpo-idx 0`.
   `dpo-idx 0` ≡ global `dpo-drop`. Compare with the earlier outer-header lookup which
   showed `dpo-idx 39` (a real adjacency) — the difference confirms the FIB miss/drop.
4. **Confirm in live VPP** with `show ip fib 240.0.0.0`. The matching prefix and source
   (`special`) pinpoint the install path.
5. **Locate in source.** Grep for `0xf0000000` in `vnet/fib/` → `ip4_fib.c` table of
   `ip4_specials[]`.

### Key facts about FIB sources (`vpp/src/vnet/fib/fib_source.h`)

- **Lower enum value = higher priority.**
- `FIB_SOURCE_SPECIAL` = highest priority. Comment: *"should never be over-ridden"*.
  Used for `224.0.0.0/4` and `240.0.0.0/4`. A normal `ip route` will NOT replace these.
- `FIB_SOURCE_DEFAULT_ROUTE` = lowest meaningful priority. Comment: *"we need to be able
  to over-ride it with 'ip route' sources"*. Used for `0.0.0.0/0`, `0.0.0.0/32`,
  `255.255.255.255/32`. These ARE overridable.

### Remediation options

| # | Approach | Notes |
|---|----------|-------|
| 1 | `ip route add 240.0.0.0/4 via <nh>` | **Does NOT work** for the class-E/D specials — they use `FIB_SOURCE_SPECIAL` (higher priority than API/CLI). Works only for the `DEFAULT_ROUTE`-sourced entries (`0.0.0.0/0`, `0.0.0.0/32`, `255.255.255.255/32`). |
| 2 | Call `fib_table_entry_special_remove(fib_index, &pfx, FIB_SOURCE_SPECIAL)` at startup per FIB | Removes the special at runtime. Must be done for every FIB and at every new table creation. Brittle without a central hook. |
| 3 | Patch `ip4_specials[]` in `vnet/fib/ip4_fib.c` to drop the unwanted entries | Cleanest. Requires custom VPP build. Loses the default protection for unwanted mcast / class-E. |

### Related corollary — `0.0.0.0/32` drop and MP2P encap safety

- `0.0.0.0/32` is also a built-in drop entry (source `FIB_SOURCE_DEFAULT_ROUTE`, so
  overridable). Any packet with dst `0.0.0.0` is dropped by default.
- For an MP2P (decap-only) tunnel, `t->tunnel_dst == 0.0.0.0`. If a user tried to forward
  through the tunnel, building an encap rewrite would produce an outer header with
  `dst = 0.0.0.0`; the resulting FIB lookup would hit the `0.0.0.0/32` drop and the
  packet would still be discarded — so the FIB acts as a safety net.
- BUT the explicit early-return in `ipip_update_adj()` (sets `NULL` midchain rewrite for
  MP2P) is still worth keeping because:
  1. `0.0.0.0/32` is user-overridable; if anyone installs a real route for it, the
     midchain would restack onto a real adjacency and emit packets with dst `0.0.0.0`.
     The `NULL` rewrite makes the drop unconditional.
  2. No bogus packet is built/encapped/checksum'd; no spurious tx counter ticks.
  3. No spurious FIB child relationship on `0.0.0.0/32` (no walk noise on changes).
  4. Drop appears tied to the tunnel adjacency, not as a generic `0.0.0.0/32` drop —
     easier to diagnose.
  5. Self-documenting: makes the decap-only contract explicit in code.

### Generic VPP drop-debug checklist

1. Read the trace bottom-up; identify the terminal node(s).
2. Look for `adj:0` / `dpo-idx 0` in lookup nodes → FIB drop. Look for `next 0` /
   `next:drop` in feature/arc nodes → feature drop.
3. Treat the textual drop "reason" as a label, not ground truth — many sites reuse
   generic error strings.
4. Use `show ip fib <addr>` / `show ip fib index <N> <addr>` to identify the matching
   prefix and the source that installed it.
5. Use `show errors` and `show node counters` while reproducing to pin the exact counter.
6. For hard cases: `pcap trace rx tx drop max 1000 intfc any file foo.pcap`,
   `trace add <node> N`, `show adj <idx>`, `show dpo-load-balance index <N>`.
7. Map node names → source files (e.g. `ip4-drop` → `vnet/ip/ip4_forward.c`); grep the
   literal error string to find the emitter.

### Using `show errors` to pin a drop reason

`show errors` (alias: `show error`) prints the per-node error-counter array. Each VPP
graph node declares a list of error strings (e.g. `ip4_error.def`, `ip6_error.def`,
node-local `error_strings[]`); every time a node calls `vlib_node_increment_counter()`
or steers a packet to its drop next-edge with an error index, the corresponding bucket
ticks. This is the most reliable way to identify the **exact** reason for a drop —
the trace's free-text label can be misleading, but the counter that increments cannot.

#### Workflow

```
vpp# clear errors           ; zero all node error counters
vpp# clear runtime          ; (optional) reset per-node call/clocks stats
... reproduce traffic ...
vpp# show errors            ; see which buckets ticked
```

Typical output (columns: Count, Node, Reason, Severity):

```
vpp# show error
   Count                  Node                              Reason               Severity
         9            ip6-validate                      valid packets              error
     20009            ip4-validate                      valid packets              error
         9        acl-plugin-in-ip4-fa                ACL permit packets           error
         9        acl-plugin-in-ip4-fa                 checked packets             error
         9        acl-plugin-in-ip6-fa                ACL permit packets           error
         9        acl-plugin-in-ip6-fa                 checked packets             error
     10000        acl-plugin-in-ip4-l2                ACL permit packets           error
     10000        acl-plugin-in-ip4-l2                 checked packets             error
     10000            ipip4-input                    packets decapsulated          error
     10000             ip4-local                     unknown ip protocol           error
        18             bond-input                          no error                error
     10000              l2-learn                       L2 learn packets            error
     10000              l2-input                       L2 input packets            error
        12            ip6-validate                      valid packets              error
         9            ip4-validate                      valid packets              error
         9        acl-plugin-in-ip4-fa                ACL permit packets           error
         9        acl-plugin-in-ip4-fa                 checked packets             error
        12        acl-plugin-in-ip6-fa                ACL permit packets           error
        12        acl-plugin-in-ip6-fa                 checked packets             error
        21             bond-input                          no error                error
```

Map the ticking counter back to source:

- `node` column → the graph node that emitted the counter.
- `reason` column → the literal string from that node's `error_strings[]` /
  `*_error.def` file.
- Grep the reason text in the VPP source to find the exact emit site, e.g.:

  ```
  $ grep -rn "no tunnel" vpp/src/vnet/ipip/
  vpp/src/vnet/ipip/ipip.c: ... IPIP_ERROR_NO_TUNNEL ...
  vpp/src/vnet/ipip/ipip_api.c: ...
  vpp/src/vnet/ipip/node.c: ... b->error = node->errors[IPIP_ERROR_NO_TUNNEL];
  ```

#### Useful variants

| Command | Purpose |
|---|---|
| `show errors` | All non-zero error counters across all nodes/workers. |
| `clear errors` | Zero counters before a focused reproduction. |
| `show errors verbose` | Include zero counters (useful to enumerate all possible reasons for a node). |
| `show node counters` | Like `show errors` but also includes informational (non-error) counters. |
| `show runtime` / `clear runtime` | Per-node call counts, vectors, clocks — combine to confirm packets actually traversed the suspected node. |
| `show node <name>` | Lists a node's error strings and next-nodes. |

#### Tips

- **Always `clear errors` before reproducing.** Counters accumulate since boot and the
  signal is otherwise drowned out.
- **Send a known-small burst** (e.g. one packet) and confirm the bucket increments
  by exactly that amount. Anything else means a different code path is also being hit.
- The "error-drop" node bucket and the **originating node's** bucket usually both tick
  for the same packet (the originating node tags the error, `error-drop` is the sink).
  The originating-node bucket is the one to trust for "why".
- The reason string is just an array entry — *do not* infer semantics from it alone.
  Grep the source to confirm what triggers it.
- For per-worker breakdown on multi-thread setups, run `show errors` from the CLI;
  VPP aggregates across threads. Use `show node counters` for the same with extras.
- `show interface` and `show hardware-interfaces` show `rx-error` / `tx-error` and
  `rx-miss` at the device level — useful to distinguish NIC/DPDK-level drops from
  graph-node drops.

#### Worked example (this trace)

For the class-E decap drop, `show errors` after reproducing would show counters like:

```
   Count   Node           Reason
       N   ip4-drop       Misc. ip4 drops
       N   error-drop     ip4-local: unknown ip protocol
```

Both tick once per dropped packet. The `ip4-drop` counter is the one emitted at the
FIB-drop site (matching `dpo-idx 0`). The string `"ip4-local: unknown ip protocol"` is
the generic error slot reused by `ip4-drop` — grepping it in `vnet/ip/` shows it lives
in `ip4_error.def`, used by multiple sites. So the string alone is *not* enough to
identify the root cause; the combination with `dpo-idx 0` in the trace and
`show ip fib 240.0.0.0` (matching the class-E special) is what pins it.
