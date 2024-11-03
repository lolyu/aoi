# Linux networking receive path
* receive path overview

![image](https://github.com/user-attachments/assets/24fd6a47-97a0-401b-aefa-26c7c5c35b47)

* Linux receives frames with a four-step process:
    1. hardware receiption
        * the NIC receives the frame on the wire, and transfer to either NIC internal buffer memory or ring buffer.
    2. hard IRQ
        * the NIC interrupts the CPU and schedule the soft IRQ operation
    5. soft IRQ
        * running on the same CPU as the hard IRQ(minimize locking overhead), the kernel removes the frame from the ring buffer and processes it through the network stack, and is appended to the applicatio nsocket.
        * this process is done iteratively until the ring buffer runs out of frames, or until the `dev_weight`
    7. application receive




![image](https://github.com/user-attachments/assets/c6facec9-320e-48be-988a-1732dd47b94c)


## performance tuning considerations

### NIC ring buffer overrun
* how to check?
```
# ethtool -S eth0
NIC statistics:
     rx_octets: 192616016
     rx_fragments: 0
     rx_ucast_packets: 155674
     rx_mcast_packets: 170984
     rx_bcast_packets: 1457027
     rx_fcs_errors: 0
     rx_align_errors: 0
     ...
     rx_discards: 0
     rx_errors: 0
     ...
```

* how to fix?
1. increase the ring buffer length
```
# ethtool --set-ring eth0
```
2. increase device weight
    * `device_weight` is the maximum number of frames that the NIC can receive before the `softirq` contex has to yield the CPU.
```
# cat /proc/sys/net/core/dev_weight
64
```

#### socket queue
* how to check?
```
# cat /proc/sys/net/ipv4/tcp_rmem
4096    131072  6291456
# cat /proc/sys/net/ipv4/tcp_wmem
4096    16384   4194304
# ss -tm
State                          Recv-Q                          Send-Q                                                     Local Address:Port                                                     Peer Address:Port                           Process
ESTAB                          0                               0                                                          10.150.22.234:ssh                                                       10.90.6.201:39912
         skmem:(r0,rb131072,t0,tb230400,f4096,w0,o0,bl0,d1)
```

