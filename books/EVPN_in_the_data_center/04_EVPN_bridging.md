# EVPN bridging

* traditional bridging: flood and learn

## EVPN bridging

<img width="823" height="517" alt="image" src="https://github.com/user-attachments/assets/d003f8a0-0745-4537-a085-914dbc43480f" />

* setup:
    * red VNI: 10.1.1.0/24
        * A: 10.1.1.A
    * blue VNI: 10.1.2.0/24
        * D: 10.1.2.D
* the eBGP sessions between leaf/spine must enable EVPN, and the advertisement of each NVE's locally attached virtual networks.
* steps when `A` sends packet to `L1` destined to `E`:
    * `L1` receives the packet, learns `MAC_A` is via `PORT_A`.
        * `L1` advertises the reachability to `MAC_A` via BGP EVPN:
            * `(VNI_RED, MAC_A)` is reachable via VTEP `L1`.
            * `L2`/`L4` learns `(VNI_RED, MAC_A)` from VTEP `L1`
    * `L1` doesn't know `MAC_E` -> flood the packet via VXLAN
    * `L1` sends VXLAN encapsulated packets to `L2` and `L4`
        * the VXLAN packet source port is hashed on the inner packet header (load-balance purpose)
    * `L2`/`L4` decapsulate the VXLAN encapsulated packet
    * `L2`/`L4` send the packet out of locally attached port in the red virtual network.
        * `L4` forwards the packet to `E`
    * `E` sends packet to `L4` destined to `A`:
        * the dest MAC is set to `MAC_A`, source MAC is set to `MAC_E`
    * `L4` receives the packet, learns `MAC_E` is via `PORT_E`
        * `L4` advertises the reachability to `MAC_E` via BGP EVPN:
            * `(VNI_RED, MAC_E)` is reachable via VTEP `L4`.
            * `L1`/`L2` learns `(VNI_RED, MAC_E)` from VTEP `L4`
    * `L4` knows `MAC_A` is reachable via VTEP `L1`, encapsulated the packet (dest IP VTEP `L1`, source IP VTEP `L4`)
        * the VXLAN packet source port is hashed on the inner packet header (load-balance purpose)
    * `L1` receives/decapsulates the VXLAN encapsulated packet
    * `L1` sees dest MAC is `MAC_A`, which is reachable via `PORT_A`, sends the packet to A.

* what happens if L4 doesn't have `(VNI_RED, MAC_A)` reachability knowledge learnt from BGP?
    * `L4` behaves similar to `L1` and sends the packets to all NVEs interested in red virtual network.
* what happens if packet from `E` if, upon reaching `L1`, it doesn't know where `A` is?
    * `L1` floods the packet out all locally attached ports in the red virtual network.

## split horizon
* split horizon: the loop prevention rule for flooding
    * don't send a flooded/BUM frame back out toward where it could loop
    * VLAN:
        * flood in the VLAN but not the ingress port
    * VXLAN/overlay
        * not flood a VXLAN-encapsulated packet after decapsulation back into the VXLAN overlay

## BUM packets handling
* BUM (broadcast, unknown unicast, multicast)
* when the broadcast is flooded, how to decide the flooding target:
    * ingress replication
    * L3 underlay multicast
    * dropping BUM packets

### ingress replication

* The mechanism: Type-3 IMET advertisement
* IMET = Inclusive Multicast Ethernet Tag route (EVPN route type 3). Every VTEP that has a given VNI advertises a Type-3 route saying, in effect: "I have VNI X; my VTEP IP is Y; flood BUM for this VNI to me via ingress replication."

```
RD 10.0.0.1:10100
[3]:[EthTag 0]:[32]:[10.0.0.1]        ← originator's VTEP IP = 10.0.0.1
    PMSI Tunnel attribute: type=Ingress Replication, label=VNI 10100
    RT 65000:10100
```

* The two load-bearing pieces:
    * The originating VTEP IP (10.0.0.1) — encoded both in the route and in the PMSI tunnel attribute. This is the address other VTEPs will unicast-encapsulate BUM copies to.
The PMSI Tunnel attribute — carries tunnel type = Ingress Replication (type 6) and the VNI/label. This is what says "replicate to me by unicast," as opposed to "join this multicast group."

* How a VTEP builds its flood list
    * The membership discovery is symmetric — every VTEP does the same thing:
        * Advertise: when leaf A comes up with VNI 10100 (via advertise-all-vni), it originates a Type-3 IMET for VNI 10100 carrying its VTEP IP 10.0.0.1 and RT 65000:10100.
Import by RT: leaf B, C, D each receive A's Type-3 (reflected by the spines). They import it because its RT (65000:10100) matches their VNI 10100's import-RT — the same RT machinery that governs everything else. If a VTEP doesn't have VNI 10100, the RT doesn't match, it ignores the route → it's correctly not in the flood list.
Install into the replication list: each importing VTEP adds 10.0.0.1 to its per-VNI ingress-replication list for VNI 10100. (1/3)
    * Every VTEP does this for every other, so they all converge on the same membership: VNI 10100 flood list = {10.0.0.1, 10.0.0.2, 10.0.0.3, …} minus self.
    * Now when leaf B needs to flood a broadcast for VNI 10100, it looks up its flood list {A, C, D} and sends one unicast VXLAN copy to each.

```
A floods →  A makes copies →  unicast to B
                              unicast to C   (3 separate packets on the wire)
                              unicast to D
```

### l3 underlay multicast

```
A floods →  A sends ONE packet to group 239.1.1.1
              spine replicates along the tree → B, C, D  (one copy per link)
```

## MAC moves
* situation:
    * A moves from behind L1 to behind L2
    * A sends out GARP after move
    * L2 learns MAC_A is local, update its FDB table, announce `(VNI_RED, MAC_A)`
    * L1 learns BGP UPDATE **`(VNI_RED, MAC_A)` is from L2, but its FDB table says MAC_A is local**
        * BGP always prefer local

* The mechanism: the MAC Mobility extended community (a sequence number)
    * EVPN attaches a MAC Mobility extended community to the Type-2 route, carrying a sequence number. This is the tiebreaker:
        * When a MAC is first advertised, seq = 0 (or the community is absent).
        * When a VTEP learns a MAC that it sees is already advertised by another VTEP (i.e. a move), it advertises its Type-2 with seq = (highest seq seen) + 1.
        * Highest sequence number wins. Remote VTEPs installing the route pick the Type-2 with the greatest MAC-mobility seq as the current location.

* so the walk for the mac move situation:
    * L1 announces `(VNI_RED, MAC_A)` with mac mobility (seq 0)
    * A moves from bebind L1 to behind L2
    * L2 announces `(VNI_RED, MAC_A)` with mac mobility (seq 1)
    * L1 learns `(VNI_RED, MAC_A)` is from L2 with mac mobility (seq 1), withdraw its local one (seq 0)

## ARP suppression
* ARP suppression: answer locally, don't flood
* ARP suppression (and ND suppression for IPv6) means:
    * The ingress VTEP intercepts the host's ARP request, looks up the target IP in its local EVPN-learned ARP/ND table (populated from Type-2 routes), and if it has the binding, replies directly to the host itself (proxy-ARP) — without flooding the request into the fabric.

* The flow:
    * Host X (behind VTEP A) sends ARP request for 10.1.1.20. (1/3)
    * VTEP A intercepts it (doesn't flood immediately). It checks its local table, which was populated by B's Type-2 route → 10.1.1.20 = BB:BB:....
    * Match → VTEP A generates the ARP reply itself ("10.1.1.20 is at BB:BB:...") and sends it straight back to X. The ARP request never leaves VTEP A.
    * Miss (A doesn't know the IP yet — e.g. a silent host that hasn't been advertised) → A falls back to flooding the ARP normally, so correctness is preserved.


# references
