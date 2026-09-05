<img width="877" height="270" alt="image" src="https://github.com/user-attachments/assets/dfdef25b-0e39-4737-9aff-0ea59d1f9138" /># EVPN basics

## EVPN in DC

* eBGP carries both overlay + underlay advertisements:

<img width="877" height="270" alt="image" src="https://github.com/user-attachments/assets/f681a447-c1a4-41dc-9ba1-42f7553bc766" />

## BGP requirements for virtual network

* for a routing protocol to exchange information about virtual informations:
    * a way to identify the network address being exchanged
        * BGP AFI(address family indicator)/SAFI(subsequent address family indicator)
    * a way to idenity the mapping between network address advertised and the virtual network
    * a way to identify the tunnel used to build the network overlay
 
### BGP AFI/SAFI
* AFI (Address Family Identifier): A 16-bit field that identifies the network layer protocol.
    * 1 = IPv4
    * 2 = IPv6
    * 25 = L2VPN / EVPN
* SAFI (Subsequent Address Family Identifier): An 8-bit field that gives more detail about the type of Network Layer Reachability Information (NLRI) being carried.
    * 1 = Unicast
    * 2 = Multicast
    * 4 = NLRI with MPLS labels (BGP-LU)
    * 70 = EVPN (Ethernet VPN)
    * 128 = MPLS-labeled VPN (L3VPN)

## route distinguisher
* route distinguisher is to make the route unique - as addressed might not be unique across multiple virtual networks

<img width="786" height="337" alt="image" src="https://github.com/user-attachments/assets/6ef89163-1738-4b86-af47-611ec284e57d" />

## route target
* route target is used to decide which local virtual network should add the routes received

<img width="836" height="352" alt="image" src="https://github.com/user-attachments/assets/07d6a6e5-daf6-4973-97d0-08bc29762a90" />


## BGP route types
* NLRI (network layer reachability information)
* EVPN NLRI is further classified by route types:

| Type | Name | Carries | Primary use | Plane |
|---|---|---|---|---|
| Type 1 | Ethernet Segment Auto-Discovery (A-D) | ESI, EthTag, ESI-label, Single-Active flag | Multihoming: aliasing (ECMP to a MAC across both leaves) + fast mass-withdraw | Control (MH) |
| Type 2 | MAC/IP Advertisement | MAC, VNI, (optional) IP, ESI, L2VNI+L3VNI, router-MAC | Host reachability: advertise a specific MAC (and optionally its IP) | L2 (+L3 for IRB) |
| Type 3 | Inclusive Multicast Ethernet Tag (IMET) | VNI/VTEP association (PMSI) | BUM flood list: who's in a VNI, builds ingress-replication list | L2 (BUM) |
| Type 4 | Ethernet Segment (ES) | ESI, originating VTEP IP | Multihoming: ES discovery → DF election + split-horizon | Control (MH) |
| Type 5 | IP Prefix | IP prefix (not /32,/128), L3 VNI, router-MAC | Routed reachability: subnets/summaries/silent hosts | L3 |
| Type 6 | Selective Multicast (SMET) | Multicast group membership (from IGMP) | Multicast optimization: only forward groups someone wants | L2 (mcast) |

## EVPN EBGP: keep the nexthop unchanged

* The EVPN next-hop is the destination `VTEP` for the `VXLAN` tunnel, so it must stay pointed at the originating T0 (A) that actually owns the MAC; if a T1 (B) applies eBGP's default and rewrites it to itself, remote leaves would try to tunnel to a box that isn't a VTEP and can't decapsulate — black-holing the overlay.
* set ip next-hop unchanged (needed on non-FRR T1s in an eBGP-underlay CloS) keeps the T1 transparent for overlay routes while it still does normal next-hop rewriting for the underlay.

## EVPN EBGP: keep route targets
* Normally, a BGP speaker receiving VPN routes (EVPN/VPNv4) applies RT filtering on import: if a received route carries an RT that no local VRF imports, the router discards it. Why keep a route you'll never install into any local VRF? It's a sensible memory optimization — don't store VPN routes for tenants you don't serve.
* The default eBGP optimization drops any VPN prefix whose RT no local VRF imports — but a Clos T1 imports no RTs at all (it's underlay-only, not a VTEP), so it would drop every EVPN route it's supposed to relay to the other leaves, black-holing the overlay. `retain-route-target-all` tells the T1 to retain and re-advertise all EVPN routes regardless of RT, keeping it a transparent transit relay.

## FRR support for EVPN
* FRR sets the next hop to itself for IPv4/IPv6 unicast AFI/SAFI advertisements, but does not do so for EVPN routes (specifically type 2 and 3 Route Types).
