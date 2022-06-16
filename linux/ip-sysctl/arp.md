# arp
* rp_filter
    * strict reverse path: if a packet entering a port, the kernel will use the source IP of the packet against the FIB to see if the igress port is the best reverse path.
    * 1 - if the packet fails the strict reverse path, it will be discarded
    * 2 - the packet source IP will be checked against the FIB, if the source IP is not reachable, it will be discarded.
 * arp_filter
    * 1 - the ARP reply only be answered if the kernel would route a packet with source IP as the ARPed IP out that interface.
        * this needs source based routing towork
    * 2 - the kernel will respond to ARP request with addresses from other interfaces
 * arp_announce
    * determines which IP address to use to send the ARP request
    * 0 - use any local address, configured on any interface
    * 1 - try to use local addresses in the same subnet as the ARPed IP
    * 2 - try to use the best local address
 * arp_ignore
    * 0 - reply for any local target IP addresses
    * 1 - reply only if the target IP address is configured on the incoming interface
    * 2 - reply only if the target IP address is configured on the incoming interface and both the sender's IP address are part from same subnet on this interface
    * 2 -  do not reply for local addresses configured with scope host,
	only resolutions for global and link addresses are replied
