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
