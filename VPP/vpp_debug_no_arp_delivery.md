# ARP from PTF not responded by SONiC VPP

## Problem

PTF host `eth1` (192.168.0.100) sends ARP who-has for 192.168.0.1 (Vlan1000 SVI).
No ARP reply is received. `tcpdump` on DUT's `Ethernet4` tap sees nothing.

## Summary

Two questions, two answers:

| Question | Answer |
|---|---|
| Why doesn't VPP respond to the ARP? | `arp-term-l2bd` uses a per-BD `mac_by_ip4` hash table that is **empty** — not the BVI's interface IP |
| Why isn't the ARP punted to Linux (Ethernet4/Vlan1000)? | `bobm1` is in L2 bridge mode → packets go through `l2-input`, not the L3 linux-cp punt path. The BVI has no LCP tap pair (by design), so the flood copy to bvi1000 is blackholed. |

---

## 1. Why ARP is NOT punted to Ethernet4 / Vlan1000

When a port is placed in an L2 bridge domain (`set interface l2 bridge bobm1 1000`),
ingress packets take the **L2 path** (`l2-input`), not the L3 path (`ip4-input`).

The linux-cp cross-connect (`lip-xc: bobm1 → tap4103/Ethernet4`) only applies on
the **L3 input path**. Once in L2 mode, packets are processed by the bridge domain
feature chain: `l2-input-vtr → l2-learn → arp-term-l2bd → l2-flood`.

The L2 flood sends copies to all BD members including bvi1000 (sw_if_index 75).
When VPP tries to punt the bvi1000 copy to Linux via linux-cp, it finds:
```
linux-cp-punt-xc: lip-xc: 75 -> 4294967295
```
`0xFFFFFFFF` = invalid — **bvi1000 has no LCP tap pair** (this is by design in SONiC VPP).
The flood copy to the BVI is blackholed. Linux never sees the ARP.

**This is expected.** The design intent is for `arp-term-l2bd` to answer ARP directly
in the VPP dataplane, without involving Linux.

---

## 2. Why `arp-term-l2bd` does NOT respond

### State check

```
vpp# show interface addr bvi1000
bvi1000 (up):
  L2 bridge bd-id 1000 idx 1 shg 0 bvi
  L3 192.168.0.1/21
  L3 fc02:1000::1/64

vpp# show bridge-domain 1000 detail
  ...
  ARP-Term: on
  ...
  IP4/IP6 to MAC table for ARP Termination
  ← EMPTY
```

bvi1000 has 192.168.0.1 and ARP termination is enabled, but the **ARP term table is empty**.

### Root cause: `arp-term-l2bd` uses `mac_by_ip4` hash, NOT the BVI interface IP

From `l2_arp_term.c`:
```c
macp0 = (u8 *) hash_get (last_bd_config->mac_by_ip4, ip0);

if (PREDICT_FALSE (!macp0))
    goto next_l2_feature;    /* MAC not found → fall through to l2-flood */
```

`arp-term-l2bd` does NOT look at the BVI's configured IP addresses. It uses a
**separate per-BD hash table** (`bd_config->mac_by_ip4`) that must be explicitly
populated via the `bd_ip_mac_add_del` VPP API.

VPP's own test (`test_l2bd_arp_term.py`) confirms this:
```python
self.vapi.bd_ip_mac_add_del(is_add=1, entry={"bd_id": bd_id, "ip": ip, "mac": mac})
```

SONiC's `vpp_create_bvi_interface()` in `SwitchVppFdb.cpp` enables ARP termination
(`set_bridge_domain_flags(bd_id, VPP_BD_FLAG_ARP_TERM, true)`) but **never populates
the ARP term table** with the BVI's IP+MAC.

---

## Packet trace (annotated)

```
dpdk-input (bobm1/sw_if_index 2)
  → ethernet-input (ARP: fe:54:00:19:d0:01 → ff:ff:ff:ff:ff:ff)
  → l2-input (bd_index 1)                          ← L2 path, NOT L3 punt
    → l2-input-vtr (strips VLAN 1000 tag)
    → l2-learn (learns src MAC on sw_if_index 2)
    → arp-term-l2bd                                 ← looks up mac_by_ip4 for 192.168.0.1
                                                      hash_get returns NULL → fall through
    → l2-flood (broadcasts to all 24 BD members)
      → l2-output to bobm3..bobm24                  ← flooded out all physical ports
      → linux-cp-punt-xc: lip-xc: 75 → 0xFFFFFFFF  ← bvi1000 has no LCP pair
      → error-drop rx:bvi1000
      → drop: null-node: blackholed packets
```

---

## Fix

### Manual VPP CLI

```shell
set bridge-domain arp entry 1000 192.168.0.1 22:59:d0:d5:19:a9
set bridge-domain arp entry 1000 fc02:1000::1 22:59:d0:d5:19:a9
```

Validate with:
```shell
show bridge-domain 1000 detail
# Check "IP4/IP6 to MAC table for ARP Termination" section is now populated
```

### Code fix

In `SwitchVppRif.cpp`, when an IP address is added to a BVI interface, also call
`bd_ip_mac_add_del(bd_id, ip, bvi_mac, is_add=1)` to populate the ARP termination
table. Similarly, remove the entry when the IP is deleted.

---

## Debug commands cheatsheet

```shell
show interface addr bvi1000       # BVI IP config
show bridge-domain 1000 detail    # BD members + ARP term table
show l2fib all                    # L2 MAC table
show lcp                          # linux-cp tap pairs
trace add dpdk-input 20           # capture packets
show trace                        # view captured trace
```