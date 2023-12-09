# linux routing
![image](https://github.com/lolyu/aoi/assets/35479537/52bffb64-6468-4627-a63a-7cd4b8ec8ceb)
* the routing result is attached to the `sk_buff` struct
* the routing decision is made at two stages:
    * prerouting
        * decide if a packet is locally-forwarded
    * postrouting
        * decide the out packet routing

## references
* https://thermalcircle.de/doku.php?id=blog:linux:routing_decisions_in_the_linux_kernel_1_lookup_packet_flow
* https://thermalcircle.de/doku.php?id=blog:linux:routing_decisions_in_the_linux_kernel_2_caching
