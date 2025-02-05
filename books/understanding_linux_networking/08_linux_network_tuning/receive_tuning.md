# receive tuning

## NIC ring buffer resize
* check current NIC ring buffer size
```
# ethtool -g eth0
Ring parameters for eth0:
Pre-set maximums:
RX:             511                            // the max limit is 511
RX Mini:        n/a
RX Jumbo:       n/a
TX:             511
Current hardware settings:
RX:             200                            // current setting is 200
RX Mini:        n/a
RX Jumbo:       n/a
TX:             511
RX Buf Len:             n/a
CQE Size:               n/a
TX Push:        off
TCP data split: n/a
```
* check for NIC ring buffer overrun
```
# ip -s link show eth0
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc mq state UP mode DEFAULT group default qlen 1000
    link/ether a4:3f:68:fb:3c:00 brd ff:ff:ff:ff:ff:ff
    RX:  bytes packets errors dropped  missed   mcast
     113431262  191167      0       2       0    2480
    TX:  bytes packets errors dropped carrier collsns
      36436555  201298      0       0       0       0
# ifconfig eth0
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.150.22.35  netmask 255.255.254.0  broadcast 10.150.23.255
        inet6 2404:f801:10:2200::a96:1623  prefixlen 52  scopeid 0x0<global>
        inet6 fe80::a63f:68ff:fefb:3c00  prefixlen 64  scopeid 0x20<link>
        ether a4:3f:68:fb:3c:00  txqueuelen 1000  (Ethernet)
        RX packets 190688  bytes 113127295 (107.8 MiB)
        RX errors 0  dropped 2  overruns 0  frame 0
        TX packets 201169  bytes 36406621 (34.7 MiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
        device interrupt 16
# cat /proc/net/dev                                                                                                                                                                                                                                                     1 ↵
Inter-|   Receive                                                |  Transmit
 face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed
  eno1: 550513785095 1787182647   21   17    0    21          0  16271162 610592092381 1816591493    0    0    0     0       0          0
```
* The statistics:
    * `errors`: transmit/receive errors detected by driver
    * `dropped`: total number of packets dropped by driver
    * `overrun`: the ring buffer overrun errors
    * `frame`: misaligned frames (length not divisible by 8)
* increase the ring buffer size
```
# ethtool -G eth0 rx 511
# ethtool -g eth0
Ring parameters for eth0:
Pre-set maximums:
RX:             511
RX Mini:        n/a
RX Jumbo:       n/a
TX:             511
Current hardware settings:
RX:             511
RX Mini:        n/a
RX Jumbo:       n/a
TX:             511
RX Buf Len:             n/a
CQE Size:               n/a
TX Push:        off
TCP data split: n/a
```

## RSS (receive side scaling)
```
# cat /proc/interrupts
           CPU0       CPU1       CPU2       CPU3
 39:          0     234737          0          0   PCI-MSI 1048576-edge      eth0-tx-0
 40:          0          0    1020423          0   PCI-MSI 1048577-edge      eth0-rx-1
 41:          0          0          0      99851   PCI-MSI 1048578-edge      eth0-rx-2
root@bjw2-can-7260-5:~# ethtool -l eth0
Channel parameters for eth0:
Pre-set maximums:
RX:             4                                        // support up to 4 rx queues
TX:             1
Other:          n/a
Combined:       n/a
Current hardware settings:
RX:             2                                        // current setting is 2
TX:             1
Other:          n/a
Combined:       n/a
root@bjw2-can-7260-5:~# ethtool -L eth0 rx 4
root@bjw2-can-7260-5:~# ethtool -l eth0
Channel parameters for eth0:
Pre-set maximums:
RX:             4
TX:             1
Other:          n/a
Combined:       n/a
Current hardware settings:
RX:             4
TX:             1
Other:          n/a
Combined:       n/a
root@bjw2-can-7260-5:~# cat /proc/interrupts
           CPU0       CPU1       CPU2       CPU3
 39:          0        177          0          0   PCI-MSI 1048576-edge      eth0-tx-0
 40:          0          0        197          0   PCI-MSI 1048577-edge      eth0-rx-1
 41:          0          0          0         13   PCI-MSI 1048578-edge      eth0-rx-2
 42:          1          0          0          0   PCI-MSI 1048579-edge      eth0-rx-3
 43:          0          1          0          0   PCI-MSI 1048580-edge      eth0-rx-4
root@bjw2-can-7260-5:~# cat /proc/irq/40/effective_affinity                                    // 4th cpu
4
root@bjw2-can-7260-5:~# cat /proc/irq/41/effective_affinity                                    // 3th cpu
8
root@bjw2-can-7260-5:~# cat /proc/irq/42/effective_affinity                                    // 1st cpu
1
root@bjw2-can-7260-5:~# cat /proc/irq/43/effective_affinity                                    // 2nd cpu
2
```
* set the `RSS` indirection hash key mapping:

```
# ethtool --show-rxfh-indir eth0
RX flow hash indirection table for eth0 with 4 RX ring(s):
    0:      0     1     0     1     0     1     0     1
    8:      0     1     0     1     0     1     0     1
   16:      0     1     0     1     0     1     0     1
   24:      0     1     0     1     0     1     0     1
   32:      0     1     0     1     0     1     0     1
   40:      0     1     0     1     0     1     0     1
   48:      0     1     0     1     0     1     0     1
   56:      0     1     0     1     0     1     0     1
   64:      0     1     0     1     0     1     0     1
   72:      0     1     0     1     0     1     0     1
   80:      0     1     0     1     0     1     0     1
   88:      0     1     0     1     0     1     0     1
   96:      0     1     0     1     0     1     0     1
  104:      0     1     0     1     0     1     0     1
  112:      0     1     0     1     0     1     0     1
  120:      0     1     0     1     0     1     0     1
RSS hash key:
Operation not supported
RSS hash function:
    toeplitz: on
    xor: off
    crc32: off
# ethtool --set-rxfh-indir eth0 equal 4
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

## hardware IRQ coalesce
```
# ethtool -c eth0
Coalesce parameters for eth0:
Adaptive RX: n/a  TX: n/a
stats-block-usecs: 0
sample-interval: n/a
pkt-rate-low: n/a
pkt-rate-high: n/a

rx-usecs: 20
rx-frames: 5
rx-usecs-irq: 0
rx-frames-irq: 5

tx-usecs: 72
tx-frames: 53
tx-usecs-irq: 0
tx-frames-irq: 5

rx-usecs-low: n/a
rx-frame-low: n/a
tx-usecs-low: n/a
tx-frame-low: n/a

rx-usecs-high: n/a
rx-frame-high: n/a
tx-usecs-high: n/a
tx-frame-high: n/a

CQE mode RX: n/a  TX: n/a
```

## NAPI budget
* `netdev_budget` controls how many packets `ksoftirqd` can process in one soft IRQ handling.
```
# sysctl net.core.netdev_budget
net.core.netdev_budget = 300
# sysctl -w net.core.netdev_budget=600
net.core.netdev_budget = 600
# sysctl net.core.netdev_budget
net.core.netdev_budget = 600
```

## receive offload
* receive offload reassembles incoming packets into large buffers and transfers the larger but fewer packets to the Linux network stack, which saves the overall CPU packet processing time.
```
# ethtool -k eth0
tcp-segmentation-offload: on
generic-receive-offload: on
large-receive-offload: off [fixed]
```

### LRO (large receive offload)
* `LRO` is implemented in the NIC.

### GRO (generic receive offload)
* `GRO` is implemented in the kernel.
