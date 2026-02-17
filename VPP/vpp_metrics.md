# VPP metrics


## runtime statistics

```
vpp# show run
Time 183141.7, 10 sec internal node vector rate 0.00 loops/sec 1578899.96
  vector rates in 1.8901e-3, out 1.6007e-3, drop 1.1147e-3, punt 0.0000e0
             Name                 State         Calls          Vectors        Suspends         Clocks       Vectors/Call
af-packet-input               interrupt wa             330             337               0          9.72e6            1.02
arp-input                        active                  4               4               0          3.51e3            1.00
arp-reply                        active                  4               4               0          1.13e5            1.00
host-vpp2vpp1-output             active                142             142               0          1.56e3            1.00
host-vpp2vpp1-tx                 active                142             142               0          3.54e4            1.00
ip4-icmp-echo-request            active                138             138               0          2.47e3            1.00
ip4-icmp-input                   active                138             138               0          1.19e3            1.00
ip4-input                        active                138             138               0          2.50e3            1.00
ip4-load-balance                 active                138             138               0          1.98e3            1.00
ip4-lookup                       active                138             138               0          3.27e3            1.00
ip4-neighbor-age-process      "event wait"               0               0               1          4.89e3            0.00
ip4-receive                      active                138             138               0          3.18e3            1.00
ip4-rewrite                      active                138             138               0          2.54e3            1.00
```

* `Time 183141.7`: total runtime
* `10 sec internal node vector rate 0.00`: average number of packets (vectors) processed per second across all data-plane nodes over the last 10-second window
* `loops/sec 1578899.96`: how many times the main VPP loops in one second
  * high loops/sec -> the VPP is idling
* columns:
  * state:
    * active: The node has found work (packets) and is currently processing them.
    * polling: This is the "high-performance" state. The node is constantly checking for work (like a DPDK or memif input node). Even if no packets are there, it stays "awake" to ensure zero latency.
    * event wait / any wait: This is for Process Nodes (background tasks). They are currently "sleeping" and waiting for a specific signal or timer to wake them up.
    * interrupt: The node is asleep but will be woken up by a hardware or software "kick" (common in power-saving modes).
  * calls: how many times the node is executed
  * vectors: packet count
  * suspends: how many times the background tasks went to sleep waiting for an event.
  * clocks: CPU cycles spend in the node
  * vectors/call: **batch size**
