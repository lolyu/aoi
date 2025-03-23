# buffer management

## SONiC buffer model
* the switch uses a MMU to manage the packet buffer.
* once a packet arrives at the switch, the packet is mapped to an ingress queue and an egress queue.
* admission control:
    * `ingress admission control`: acts over ingress queues.
    * `egress admission control`: acts over egress queues.
    * all ingress/egress queues are associated with a threshold (max queue size), if the queue size hits the threshold, the switch will drop the packet and send PFC PAUSE.

![image](https://github.com/user-attachments/assets/f2dae561-26f7-45bd-8ee4-40fed4ab96b2)

* ingress admission control:
    * lossy queue: if the queue hits the threshold -> packet is dropped.
    * lossless queue: if the queue hists the threshold ->
        1. queue in PAUSE state, send PFC PAUSE
        2. store the packet in headroom
* egress admission control

## configurations
* a buffer configuration includes:
    * buffer pool
        * size: buffer size
        * type: egress/ingress
        * mode: static/dynamic
        * xoff: shared headroom pool size, only for ingress lossless pool.
    * buffer profile
        * pool: name
        * size: guaranteed buffer size
        * xon: PFC resume size
        * xon_offset: PFC resume offset
            * **NOTE**: the PFC PAUSE resume is governed by `total buffer usage <= max(XON_TH, total buffer limit - XON_OFFSET_TH)`.
        * xoff: PG headroom size
            * if the buffer pool shared headroom pool size is zero, the PG headroom buffer size is xoff.
            * if the buffer pool shared headroom pool size > 0, xoff specifies the max amount of headroom buffer this PG can use from the buffer pool shared headroom pool size.
        * dynamic_th: dynamic threshold, represents the proportion of max share buffer in the pool this queue can occupy
            * alpha = 2 ^ dynamic_th, proportion = alpha / (1 + alpha)
    * buffer PG: specifies the profile for each ingress priority.
    * buffer queue: specifies the profile for each egress queue.

```
Pool: ingress_lossless_pool
----  --------
mode  dynamic
size  33169344
type  ingress
xoff  7827456
----  --------

Profile: pg_lossless_profile
----------  ---------------------
dynamic_th  0
pool        ingress_lossless_pool
size        1248
xoff        101088
xon         1248
xon_offset  2496
----------  ---------------------
```


## references
* https://www.usenix.org/system/files/nsdi24-addanki-reverie.pdf
