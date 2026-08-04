# network virtualization

 *EVPN is to connect L2 network segments over L3 network

 ## network virtualization

 * virtualization: sharing a resource to multiple users and affords each user the illution that they own the source

* VNI - virtual network identifier

### overlay virtual network

* An overlay virtual network is a logical network built on top of an existing physical network — where the logical topology is decoupled from, and independent of, the underlying physical topology.
* You run two networks at once:
    - The underlay — the real physical network (switches, routers, cables, IP fabric). Its only job is to move packets between endpoints.
    - The overlay — a virtual network riding on top, created by encapsulating tenant traffic inside packets that the underlay forwards as ordinary IP.
 
* overlay virtual network is achieved by tunneling:

```
[ Outer IP/UDP header ]  ← underlay routes on THIS (VTEP-to-VTEP)
[ Overlay header (VNI) ]  ← identifies which virtual network
[ Original tenant frame ] ← the actual payload, untouched
```

* An edge device (VTEP / NVE) wraps the tenant's original Ethernet frame in an outer IP/UDP header addressed to the far-end edge device. The underlay delivers it like any IP packet. The far edge strips the encapsulation and hands the original frame to the destination. To the tenant, it looks like one flat L2/L3 network; physically it crossed a routed fabric.

* NVE (network virtualization edge): tunnel endpoint
    * ingress NVE - tunnel start
    * egress NVE - tunnel end

### VXLAN

<img width="608" height="330" alt="image" src="https://github.com/user-attachments/assets/a1da0efc-e164-4332-ab4a-21c6dd15d476" />

* the control plane of VXLAN must provide the following:
    * map inner payload to egress NVE address
    * allow NVE to list the virtual networks it is registered to allow point-to-multipoint communication such as broadcast

 * VXLAN needs a protocol to provide the *mapping of (VNI, MAC) tuple to egress NVE address*.
 * **NOTE**: the VXLAN UDP source port is calculated based on the hash of 5-tuple of the underlying payload header
    * all packets belonging to the same L4 flow has the same VXLAN header
