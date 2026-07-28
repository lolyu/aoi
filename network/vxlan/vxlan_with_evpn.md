# vxlan with EVPN

## vxlan basics

* What VXLAN actually does
    * VXLAN's core job is to extend an L2 broadcast domain (a VLAN) across an L3 (routed) network — stretch one VLAN so hosts on different physical switches/subnets, connected only by an IP/routed underlay, behave as if they're on the same LAN.

* The full picture:
    * Known-unicast (the common case): host A's frame to host B's MAC gets VXLAN-encapped and sent as a single targeted packet to the VTEP behind B. No flooding. This is most traffic.
    * BUM/flood (broadcast, unknown-unicast, multicast): replicated to all remote VTEPs in the VNI. This is the part you're describing — and it's the exception, needed mainly for ARP, initial learning, etc.

### vxlan vs vlan
* vlan
    * l2 segmentation with 12-bit tag -> 4096 vlans
    * doesn't across L3 boundaries
* vxlan
    * l2-over-l3 overlay: MAC-in-UDP encapsulation (UDP dst port 4789)
    * 24-bt VNI -> 16M+ segments

### underlay vs overlay
* overlay is the virtual network
    * vxlan overlay is Ethernet
* underlay is the physical network
    * vxlan underlay is IP
 
---

|terms|detail|
|-|-|
|VNI(vxlan network identifier)|vxlan ID, the overlay segment|
|VTEP(vxlan tunnel endpoint)|vxlan encapsulation/decapsulation device|
|VTEP IP interface|the VTEP ip address in the underlay network, used to encap/decap Ethernet frames|
|VNI interface|defined in each overlay segment, local handle for the overlay segment; one VTEP can have multiple VNI interfaces|

* vxlan sonic config example:

```
{
    "VXLAN_TUNNEL": {
        "vtep1": {
            "src_ip": "1.2.3.4"
        }
    },
    "VXLAN_TUNNEL_MAP": {
        "vtep1|map_100_Vlan100": {
            "vni": "100",
            "vlan": "Vlan100"
        }
    }
}
```

### vxlan packet format

<img width="1291" height="324" alt="image" src="https://github.com/user-attachments/assets/aa9c682a-d6ec-476c-ae16-66693367d365" />

### vxlan frame forwarding

<img width="2048" height="1295" alt="image" src="https://github.com/user-attachments/assets/2f93bfea-3332-44da-82ec-42f8e9431e98" />

* the key is to find the mapping between dst MAC -> remote VTEP


## vxlan static ingress replication

* CONFIG_DB

```
{
    "VXLAN_TUNNEL": {
        "vtep1": {
            "src_ip": "1.2.3.4"
        }
    },
    "VXLAN_TUNNEL_MAP": {
        "vtep1|map_100_Vlan100": {
            "vni": "100",
            "vlan": "Vlan100"
        }
    }
}
```


* APPL_DB

```
VXLAN_REMOTE_VNI_TABLE:Vlan100:2.3.4.5   →  { vni: 100 }
```

* BUM (broadcast, unknown-unicast, and L2-multicast) frames get VXLAN-encapped (VNI 100, outer src 1.2.3.4, outer dst 2.3.4.5) and ingress-replicated to 2.3.4.5. With only one remote VTEP listed, there's exactly one flood copy.
* Add more VXLAN_REMOTE_VNI_TABLE:Vlan100:<ip> entries → one encapped copy per remote VTEP (head-end replication).

### example

* Setup
    * VNI 100 ↔ Vlan100, subnet 10.0.0.0/24
    * Switch-1 (VTEP-1): src_ip 1.1.1.1. Host A behind it: IP 10.0.0.1, MAC-A, on port Ethernet0.
    * Switch-2 (VTEP-2): src_ip 2.2.2.2. Host B behind it: IP 10.0.0.2, MAC-B, on port Ethernet0.
    * Underlay: 1.1.1.1 and 2.2.2.2 are IP-reachable (routed fabric, loopbacks advertised).
    * Each VTEP has VXLAN_REMOTE_VNI_TABLE:Vlan100:<other_vtep> → flood member built. (Way A — no EVPN; flooding via that list.)

```mermaid
sequenceDiagram
    participant A as Host A<br/>10.0.0.1 / MAC-A
    participant V1 as VTEP-1<br/>1.1.1.1
    participant UL as Underlay<br/>(routed fabric)
    participant V2 as VTEP-2<br/>2.2.2.2
    participant B as Host B<br/>10.0.0.2 / MAC-B

    Note over A,B: Phase 1 — ARP flood (BUM), IP→MAC unknown
    A->>V1: ARP req "who has 10.0.0.2?"<br/>dst=ff:ff:ff:ff:ff:ff
    Note over V1: broadcast → L2MC flood group<br/>(member: tunnel-bp to 2.2.2.2)
    V1->>UL: VXLAN encap VNI 100<br/>1.1.1.1 → 2.2.2.2
    UL->>V2: routed IP/UDP packet
    Note over V2: tunnel term match →<br/>decap, VNI 100 → Vlan100<br/>LEARN MAC-A → VTEP 1.1.1.1
    V2->>B: flood ARP req into local Vlan100
    Note over B: it's me → cache<br/>10.0.0.1 → MAC-A

    Note over A,B: Phase 2 — ARP reply (known-unicast back)
    B->>V2: ARP reply dst=MAC-A (unicast)
    Note over V2: FDB: MAC-A → 2.2.2.2? no —<br/>MAC-A → VTEP 1.1.1.1 → targeted encap
    V2->>UL: VXLAN encap VNI 100<br/>2.2.2.2 → 1.1.1.1
    UL->>V1: routed IP/UDP packet
    Note over V1: decap → Vlan100<br/>LEARN MAC-B → VTEP 2.2.2.2
    V1->>A: deliver ARP reply out Ethernet0
    Note over A: cache 10.0.0.2 → MAC-B

    Note over A,B: Phase 3 — data traffic, all known-unicast (no flood)
    A->>V1: data frame dst=MAC-B
    Note over V1: FDB: MAC-B → VTEP 2.2.2.2
    V1->>UL: VXLAN encap VNI 100<br/>1.1.1.1 → 2.2.2.2
    UL->>V2: routed IP/UDP packet
    Note over V2: decap → Vlan100, dst MAC-B
    V2->>B: deliver data out Ethernet0
```
