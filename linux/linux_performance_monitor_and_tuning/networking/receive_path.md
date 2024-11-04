# Linux networking receive path
* receive path overview

![image](https://github.com/user-attachments/assets/24fd6a47-97a0-401b-aefa-26c7c5c35b47)

* Linux receives frames with a four-step process:
    1. hardware receiption
        * the NIC receives the frame on the wire, and transfer to either NIC internal buffer memory or ring buffer.
    2. hard IRQ
        * the NIC interrupts the CPU and schedule the soft IRQ operation
    5. soft IRQ
        * running on the same CPU as the hard IRQ(minimize locking overhead), the kernel removes the frame from the ring buffer and processes it through the network stack, and is appended to the application socket.
        * this process is done iteratively until the ring buffer runs out of frames, or until the `dev_weight`
    7. application receive

## receive side scaling (RSS)
* RSS is introduced to enable NIC to forward different packets to different queues to distribute processing among CPUs.
    * the distribution to queues is based on a 4-tuple hash function.
* each receive queue has a separate IRQ associated with it, the NIC triggers this to notify a CPU when a new packet arrives on the given queue.
* the following shows `eth0` has four receive queues:
    * `eth0-rx-1` is served at `CPU2`
    * `eth0-rx-2` is served at `CPU3`
    * `eth0-rx-3` is served at `CPU0`
    * `eth0-rx-4` is served at `CPU1`
```
# cat /proc/interrupts
           CPU0       CPU1       CPU2       CPU3
...
 39:          0          0          0     688019   PCI-MSI 1048576-edge      eth0-tx-0
 40:          0          0    2440105          0   PCI-MSI 1048577-edge      eth0-rx-1
 41:          0          0          0      30203   PCI-MSI 1048578-edge      eth0-rx-2
 42:       7500          0          0          0   PCI-MSI 1048579-edge      eth0-rx-3
 43:          0     184666          0          0   PCI-MSI 1048580-edge      eth0-rx-4
...
```
* how to configure the distribution?
    * `ethtool --show-rxfh-indir`
    * `ethtool --set-rxfh-indir`
```
# ethtool --show-rxfh-indir eth0
RX flow hash indirection table for eth0 with 4 RX ring(s):
    0:      0     1     2     3     0     1     2     3
    8:      0     1     2     3     0     1     2     3
   16:      0     1     2     3     0     1     2     3
   24:      0     1     2     3     0     1     2     3
   32:      0     1     2     3     0     1     2     3
   40:      0     1     2     3     0     1     2     3
   48:      0     1     2     3     0     1     2     3
   56:      0     1     2     3     0     1     2     3
   64:      0     1     2     3     0     1     2     3
   72:      0     1     2     3     0     1     2     3
   80:      0     1     2     3     0     1     2     3
   88:      0     1     2     3     0     1     2     3
   96:      0     1     2     3     0     1     2     3
  104:      0     1     2     3     0     1     2     3
  112:      0     1     2     3     0     1     2     3
  120:      0     1     2     3     0     1     2     3
RSS hash key:
Operation not supported
RSS hash function:
    toeplitz: on
    xor: off
    crc32: off
```

## receive packet steering (RPS)
* RPS is a software implementation of RSS
    * RSS selects the queue and the CPU (via interrupt) to run the interrupt handler, RPS selects the CPU to perform frame processing above the interrupt handler by placing the packet on the desired CPU's backlog queue and waking up the CPU for processing.
    * without RPS, the frame are processed on the interrupting CPU.

* how to configure?
    * via `/sys/class/net/<dev>/queues/rx-<n>/rps_cpus`

## receive flow steering (RFS)
* the goal of RFS is to steer kernel processing of frames to the CPU where the application thread consuming the packet is running.
    * this improves the data cache hit rate.
* RFS replies on the same RPS mechanisms to enqueue packets to the backlog of another CPU and to wake up that CPU.
* TBD
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
# ip -s link show eth0
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether a4:3f:68:fb:38:f4 brd ff:ff:ff:ff:ff:ff
    RX: bytes  packets  errors  dropped missed  mcast
    288259484  2764112  0       0       0       239686
    TX: bytes  packets  errors  dropped carrier collsns
    203275234  898920   0       0       0       0
```

* how to fix?
1. increase the ring buffer length
```
# ethtool --set-ring devname value
```
2. increase device weight
    * `device_weight` is the maximum number of frames that the NIC can receive before the `softirq` context has to yield the CPU.
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
$ ss -mntp
State      Recv-Q      Send-Q           Local Address:Port               Peer Address:Port      Process
ESTAB      0           0                   10.52.6.14:22                 10.191.32.13:56148
         skmem:(r0,rb3196732,t0,tb2059776,f0,w0,o0,bl0,d22)
ESTAB      0           0                   10.52.6.14:50598             40.112.242.80:443
         skmem:(r0,rb131072,t0,tb87040,f0,w0,o0,bl0,d0)
ESTAB      0           0                    127.0.0.1:56978                 127.0.0.1:38677
         skmem:(r0,rb4323627,t0,tb2626560,f0,w0,o0,bl0,d0)
ESTAB      0           0                    127.0.0.1:37184                 127.0.0.1:43361
         skmem:(r0,rb131072,t0,tb2626560,f0,w0,o0,bl0,d0)
ESTAB      0           0                   10.52.6.14:42632           169.254.169.254:80
         skmem:(r0,rb131072,t0,tb87040,f0,w0,o0,bl0,d0)
ESTAB      0           0                    127.0.0.1:43361                 127.0.0.1:37184      users:(("code-384ff7382d",pid=4080475,fd=12))
         skmem:(r0,rb131072,t0,tb2626560,f0,w0,o0,bl0,d0)
ESTAB      0           0                   10.52.6.14:51826              20.209.103.4:443
         skmem:(r0,rb131072,t0,tb870400,f0,w0,o0,bl0,d1)
ESTAB      0           0                   10.52.6.14:39732              20.209.103.4:443
         skmem:(r0,rb131072,t0,tb870400,f0,w0,o0,bl0,d0)
ESTAB      0           0                   10.52.6.14:39714             140.82.112.22:443        users:(("node",pid=4080016,fd=56))
         skmem:(r0,rb131072,t0,tb87040,f0,w0,o0,bl0,d0)
ESTAB      0           0                    127.0.0.1:38677                 127.0.0.1:56978      users:(("node",pid=4080016,fd=20))
         skmem:(r0,rb6291456,t0,tb2626560,f4096,w0,o0,bl0,d6)
ESTAB      0           0                   10.52.6.14:22                 10.191.32.13:56176
         skmem:(r0,rb131072,t0,tb46080,f0,w0,o0,bl0,d0)
ESTAB      0           0                   10.52.6.14:40136             40.112.242.80:443
         skmem:(r0,rb131072,t0,tb87040,f0,w0,o0,bl0,d0)
ESTAB      0           0                    127.0.0.1:38677                 127.0.0.1:56976      users:(("node",pid=4079771,fd=23))
         skmem:(r0,rb131072,t0,tb2626560,f4096,w0,o0,bl0,d0)
ESTAB      0           0                    127.0.0.1:56976                 127.0.0.1:38677
         skmem:(r0,rb6291456,t0,tb2626560,f0,w0,o0,bl0,d0)
ESTAB      0           156                 10.52.6.14:22                 10.191.32.13:56048
         skmem:(r0,rb131072,t0,tb46080,f100,w3996,o0,bl0,d0)
```
* how to interpret the output from `ss -mt`:
    * `skmem:(r<rmem_alloc>,rb<rcv_buf>,t<wmem_alloc>,tb<snd_buf>,f<fwd_alloc>,w<wmem_queued>,o<opt_mem>,bl<back_log>,d<sock_drop>)`
        * <rmem_alloc>: the memory allocated for receiving packet
        * <rcv_buf>: the total memory can be allocated for receiving packet
        * <wmem_alloc>: the memory used for sending packet (which has been sent to layer 3)
        * <snd_buf>: the total memory can be allocated for sending packet
        * <fwd_alloc>: the memory allocated by the socket as cache, but not used for receiving/sending packet yet. If need memory to send/receive packet, the memory in this cache will be used before allocate additional memory.
        * <wmem_queued>: the memory allocated for sending packet (which has not been sent to layer 3)
        * <ropt_mem>: the memory used for storing socket option, e.g., the key for TCP MD5 signature
        * <back_log>: the memory used for the sk backlog queue. On a process context, if the process is receiving packet, and a new packet is received, it will be put into the sk backlog  queue, so it can be received by the process immediately
        * <sock_drop>: the number of packets dropped before they are de-multiplexed into the socket
* how to fix
1. slow down the input traffic by configuring the queue fill rate
    * decrease `dev_weight`
2. queue depth
    * Increase the value of `/proc/sys/net/core/rmem_default`
        * `sysctl -w net.core.rmem_default=N`
    * Use setsockopt to configure a larger `SO_RCVBUF` value
4. application call frequency
    * optimize the application read performance

#### busy polling
* `busy polling` allows socket layer code to poll the receive queue of a network device, and disabling network interrupts.
    * this removes delays caused by the interrupt and the context switch.
![image](https://github.com/user-attachments/assets/7af944a5-3298-4477-aa74-4b5d8be92abc)

* how to enable?
    * enable on a specific socket:
        * set `sysctl.net.core.busy_poll` to a value other than `0`
        * create socket with `SO_BUSY_POLL`
    * enable globally:
        * set `sysctl.net.core.busy_read` to a value other than `0`

```
# cat /proc/sys/net/core/busy_read
0
# cat /proc/sys/net/core/busy_poll
0
```
* check if a NIC supports:
```
# ethtool -k device | grep "busy-poll"
```

## references
* https://www.kernel.org/doc/Documentation/networking/scaling.txt
* https://docs.redhat.com/en/documentation/red_hat_enterprise_linux/7/html/performance_tuning_guide/
