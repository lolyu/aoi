# VPP Cli cheatsheet

## hardware
* `show pci`
```
vpp# show pci
Address      Sock VID:PID     Link Speed    Driver          Product Name                    Vital Product Data
0000:03:00.0   0  1af4:1041   unknown       virtio-pci
0000:04:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:05:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:06:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:07:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:08:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:09:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:0a:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:0b:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:0c:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:0d:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:0e:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:0f:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:10:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:11:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:12:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:13:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:14:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:15:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:16:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:17:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:18:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:19:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:1a:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:1b:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:1c:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:1d:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:1e:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:1f:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:20:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:21:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:22:00.0   0  1af4:1041   unknown       uio_pci_generic
0000:23:00.0   0  1af4:1041   unknown       uio_pci_generic
```

## runtime / threads

* `show threads` — list worker threads and CPU assignments
```
vpp# show threads
ID     Name                Type        LWP     Sched Policy (Priority)  lcore  Core   Socket State
0      vpp_main                        56      other (0)                2      2      0
1      vpp_wk_0            workers     58      other (0)                1      1      0
2      vpp_wk_1            workers     59      other (0)                3      3      0
```

* `show version` — VPP build info
```
vpp# show version
vpp v2510-0.1+b1sonic1 built by aronovic on 2d3694296f36 at 2025-11-07T17:02:58
```

* `show log` — recent VPP log messages (useful before `show error`)
```
vpp# show log
```

---

## L2
* `show interface`
```
vpp# show interface
              Name               Idx    State  MTU (L3/IP4/IP6/MPLS)     Counter          Count
BondEthernet101                   65     up          9100/0/0/0     rx packets                 12789
                                                                    rx bytes                 3256653
                                                                    tx packets                    18
                                                                    tx bytes                    1548
                                                                    drops                      10000
                                                                    ip4                        11146
                                                                    ip6                         1498
BondEthernet102                   66     up          9100/0/0/0     rx packets                  2212
                                                                    rx bytes                  212951
                                                                    tx packets                    18
                                                                    tx bytes                    1548
                                                                    ip4                          850
                                                                    ip6                         1217
BondEthernet103                   67     up          9100/0/0/0     rx packets                  3085
                                                                    rx bytes                  276130
                                                                    tx packets                    18
                                                                    tx bytes                    1548
                                                                    ip4                         1444
                                                                    ip6                         1496
BondEthernet104                   68     up          9100/0/0/0     rx packets                  2889
                                                                    rx bytes                  263194
                                                                    tx packets                    18
                                                                    tx bytes                    1548
                                                                    ip4                         1248
                                                                    ip6                         1496
```
```
vpp# show interface bobm11
              Name               Idx    State  MTU (L3/IP4/IP6/MPLS)     Counter          Count
bobm11                            12     up          9122/0/0/0     rx packets                    74
                                                                    rx bytes                    6844
                                                                    tx packets                   493
                                                                    tx bytes                   58218
```

* `show hard-interfaces`
```
vpp# show hardware-interfaces bobm11
              Name                Idx   Link  Hardware
bobm11                            12     up   bobm11
  Link speed: unknown
  RX Queues:
    queue thread         mode
    0     vpp_wk_1 (2)   polling
  TX Queues:
    TX Hash: [name: hash-eth-l34 priority: 50 description: Hash ethernet L34 headers]
    queue shared thread(s)
    0     yes    0-2
  Ethernet address 22:59:d0:d5:19:a9
  Red Hat Virtio
    carrier up full duplex max-frame-size 9144  promisc
    flags: admin-up promisc maybe-multiseg
    Devargs:
    rx: queues 1 (max 1), desc 256 (min 32 max 32768 align 1)
    tx: queues 1 (max 1), desc 256 (min 32 max 32768 align 1)
    pci: device 1af4:1041 subsystem 1af4:1100 address 0000:0f:00.00 numa 0
    max rx packet len: 9728
    promiscuous: unicast on all-multicast on
    vlan offload: strip off filter off qinq off
    rx offload avail:  vlan-strip vlan-filter scatter
    rx offload active: scatter
    tx offload avail:  vlan-insert multi-segs
    tx offload active: multi-segs
    rss avail:         none
    rss active:        none
    tx burst mode: Scalar
    tx burst function: virtio_xmit_pkts
    rx burst mode: Scalar mergeable
    rx burst function: virtio_recv_mergeable_pkts

    tx frames ok                                         825
    tx bytes ok                                       107885
    rx frames ok                                         113
    rx bytes ok                                        10454
    extended stats:
      rx_good_packets                                    113
      tx_good_packets                                    825
      rx_good_bytes                                    10454
      tx_good_bytes                                   107885
      rx_q0_good_packets                                 113
      rx_q0_good_bytes                                 10454
      rx_q0_multicast_packets                             21
      rx_q0_size_65_127_packets                          113
      tx_q0_good_packets                                 825
      tx_q0_good_bytes                                107885
      tx_q0_multicast_packets                            735
      tx_q0_undersize_packets                              1
      tx_q0_size_65_127_packets                          636
      tx_q0_size_128_255_packets                           9
      tx_q0_size_256_511_packets                         179
```

* `show l2fib all` — L2 FIB (MAC address table) for all bridge domains

```
vpp# show l2fib all
    Mac-Address     BD-Idx If-Idx BSN-ISN Age(min) static filter bvi         Interface-Name
 fe:54:00:36:e0:0b    1      12     0/1      -       -      -     -              bobm11
 fe:54:00:e8:60:13    1      20     0/1      -       -      -     -              bobm19
 fe:54:00:e1:70:02    1      3      0/1      -       -      -     -               bobm2
 fe:54:00:56:f0:05    1      6      0/1      -       -      -     -               bobm5
 fe:54:00:11:20:11    1      18     0/1      -       -      -     -              bobm17
 fe:54:00:f1:00:0e    1      15     0/1      -       -      -     -              bobm14
 fe:54:00:9b:e0:0d    1      14     0/1      -       -      -     -              bobm13
 fe:54:00:6e:80:0f    1      16     0/1      -       -      -     -              bobm15
 fe:54:00:e2:30:01    1      2      0/1      -       -      -     -               bobm1
 fe:54:00:ce:00:09    1      10     0/1      -       -      -     -               bobm9
 fe:54:00:1d:60:14    1      21     0/1      -       -      -     -              bobm20
 fe:54:00:ef:80:03    1      4      0/1      -       -      -     -               bobm3
 fe:54:00:94:70:06    1      7      0/1      -       -      -     -               bobm6
 fe:54:00:89:f0:07    1      8      0/1      -       -      -     -               bobm7
 fe:54:00:e2:60:0a    1      11     0/1      -       -      -     -              bobm10
 fe:54:00:5a:d0:16    1      23     0/1      -       -      -     -              bobm22
 fe:54:00:9d:d0:12    1      19     0/1      -       -      -     -              bobm18
 22:59:d0:d5:19:a9    1      75     0/0      no      *      -     *              bvi1000
 fe:54:00:44:30:18    1      25     0/1      -       -      -     -              bobm24
 fe:54:00:0c:20:10    1      17     0/1      -       -      -     -              bobm16
 fe:54:00:4c:00:17    1      24     0/1      -       -      -     -              bobm23
 fe:54:00:1d:60:04    1      5      0/1      -       -      -     -               bobm4
 fe:54:00:c0:e0:15    1      22     0/1      -       -      -     -              bobm21
 fe:54:00:47:60:0c    1      13     0/1      -       -      -     -              bobm12
 fe:54:00:6a:00:08    1      9      0/1      -       -      -     -               bobm8
L2FIB total/learned entries: 25/23  Last scan time: 0.0000e0sec  Learn limit: 16777216
```

* `show lcp` — linux-cp interface pairs (VPP ↔ Linux tap mappings)
```
vpp# show lcp
lcp default netns '<unset>'
lcp lcp-auto-subint on
lcp lcp-sync off
lcp del-static-on-link-down off
lcp del-dynamic-on-link-down off
itf-pair: [0] bobm6 tap4096 Ethernet24 221 type tap
itf-pair: [1] bobm7 tap4097 Ethernet28 222 type tap
itf-pair: [2] bobm9 tap4098 Ethernet36 223 type tap
itf-pair: [3] bobm8 tap4099 Ethernet32 224 type tap
itf-pair: [4] bobm10 tap4100 Ethernet40 225 type tap
itf-pair: [5] bobm11 tap4101 Ethernet44 226 type tap
itf-pair: [6] bobm0 tap4102 Ethernet0 227 type tap
itf-pair: [7] bobm1 tap4103 Ethernet4 228 type tap
itf-pair: [8] bobm2 tap4104 Ethernet8 229 type tap
itf-pair: [9] bobm3 tap4105 Ethernet12 230 type tap
itf-pair: [10] bobm5 tap4106 Ethernet20 231 type tap
itf-pair: [11] bobm4 tap4107 Ethernet16 232 type tap
itf-pair: [12] bobm13 tap4108 Ethernet52 233 type tap
itf-pair: [13] bobm12 tap4109 Ethernet48 234 type tap
itf-pair: [14] bobm14 tap4110 Ethernet56 235 type tap
itf-pair: [15] bobm15 tap4111 Ethernet60 236 type tap
itf-pair: [16] bobm17 tap4112 Ethernet68 237 type tap
itf-pair: [17] bobm16 tap4113 Ethernet64 238 type tap
itf-pair: [18] bobm18 tap4114 Ethernet72 239 type tap
itf-pair: [19] bobm19 tap4115 Ethernet76 240 type tap
itf-pair: [20] bobm26 tap4116 Ethernet104 241 type tap
itf-pair: [21] bobm27 tap4117 Ethernet108 242 type tap
itf-pair: [22] bobm29 tap4118 Ethernet116 243 type tap
itf-pair: [23] bobm28 tap4119 Ethernet112 244 type tap
itf-pair: [24] bobm31 tap4120 Ethernet124 245 type tap
itf-pair: [25] bobm30 tap4121 Ethernet120 246 type tap
itf-pair: [26] bobm21 tap4122 Ethernet84 247 type tap
itf-pair: [27] bobm20 tap4123 Ethernet80 248 type tap
itf-pair: [28] bobm22 tap4124 Ethernet88 249 type tap
itf-pair: [29] bobm23 tap4125 Ethernet92 250 type tap
itf-pair: [30] bobm25 tap4126 Ethernet100 251 type tap
itf-pair: [31] bobm24 tap4127 Ethernet96 252 type tap
itf-pair: [32] BondEthernet101 tap4128 be101 257 type tap
itf-pair: [33] BondEthernet102 tap4129 be102 258 type tap
itf-pair: [34] BondEthernet103 tap4130 be103 259 type tap
itf-pair: [35] BondEthernet104 tap4131 be104 260 type tap
itf-pair: [36] loop0 tap4132 tap_Loopback0 261 type tap
```


## L3
* `show int addr`
```
vpp# show interface addr loop0
loop0 (up):
  L3 10.1.0.32/32
  L3 fc00:1::32/128
```

* `show ip fib`
* `show ip fib <prefix>` — look up a specific prefix
* `show ip fib table <table-id>` — show a specific VRF
* `show ip fib tables` — list all existing VRF table IDs
* `show ip6 fib`
* `show ip neighbor` — ARP / ND table
```
vpp# show ip neighbor
     Age                       IP                    Flags      Ethernet              Interface
   3764.2008                10.0.0.57                  D    3e:90:50:e7:69:a1 BondEthernet101
```

* `show ip neighbor-stats` — ARP hit/miss counters per interface

---

## routing

* Add / delete a static route
```
vpp# ip route add 10.99.0.0/24 via 10.0.0.57 BondEthernet101
vpp# ip route del 10.99.0.0/24 via 10.0.0.57 BondEthernet101
```

* Add a blackhole (null) route
```
vpp# ip route add 10.99.0.0/24 via drop
```

* Assign interface to a VRF
```
vpp# set interface ip table GigabitEthernet0/0/0 100
```

---
```
vpp# show ip neighbor
     Age                       IP                    Flags      Ethernet              Interface
   3764.2008                10.0.0.57                  D    3e:90:50:e7:69:a1 BondEthernet101
   3647.8850        fe80::3c90:50ff:fee7:69a1          D    3e:90:50:e7:69:a1 BondEthernet101
     23.7235        fe80::3c38:beff:feea:b01c          D    3e:38:be:ea:b0:1c BondEthernet101
   3760.7637                fc00::72                   D    3e:90:50:e7:69:a1 BondEthernet101
   3764.1608                10.0.0.59                  D    06:b4:8b:a1:24:24 BondEthernet102
   3647.8747        fe80::4b4:8bff:fea1:2424           D    06:b4:8b:a1:24:24 BondEthernet102
     23.7235         fe80::cca:f1ff:fe98:1d            D    0e:ca:f1:98:00:1d BondEthernet102
   3760.7597                fc00::76                   D    06:b4:8b:a1:24:24 BondEthernet102
   3764.1194                10.0.0.61                  D    12:39:0a:c1:a9:0c BondEthernet103
   3647.8674        fe80::1039:aff:fec1:a90c           D    12:39:0a:c1:a9:0c BondEthernet103
     23.7235        fe80::c61:72ff:fe66:401e           D    0e:61:72:66:40:1e BondEthernet103
   3760.6424                fc00::7a                   D    12:39:0a:c1:a9:0c BondEthernet103
   3764.0786                10.0.0.63                  D    66:34:57:50:0c:62 BondEthernet104
   3647.8601        fe80::6434:57ff:fe50:c62           D    66:34:57:50:0c:62 BondEthernet104
     23.7235        fe80::885d:6bff:fe41:201f          D    8a:5d:6b:41:20:1f BondEthernet104
   3760.6274                fc00::7e                   D    66:34:57:50:0c:62 BondEthernet104
```

## VLAN

* `show bridge-domain`
```
vpp# show bridge-domain
  BD-ID   Index   BSN  Age(min)  Learning  U-Forwrd   UU-Flood   Flooding  ARP-Term  arp-ufwd Learn-co Learn-li   BVI-Intf
  1000      1      0     off        on        on       flood        on        on       off       23    16777216   bvi1000
vpp# show bridge-domain 1000 detail
  BD-ID   Index   BSN  Age(min)  Learning  U-Forwrd   UU-Flood   Flooding  ARP-Term  arp-ufwd Learn-co Learn-li   BVI-Intf
  1000      1      0     off        on        on       flood        on        on       off       23    16777216   bvi1000
span-l2-input l2-input-classify l2-input-feat-arc l2-policer-classify l2-input-acl vpath-input-l2 l2-ip-qos-record l2-input-vtr l2-learn l2-rw l2-fwd l2-flood arp-term-l2bd l2-flood l2-output

           Interface           If-idx ISN  SHG  BVI  TxFlood        VLAN-Tag-Rewrite
            bvi1000              87    1    0    *      *           push-1 dot1q 1000
             bobm3               4     1    0    -      *           push-1 dot1q 1000
             bobm4               5     1    0    -      *           push-1 dot1q 1000
             bobm5               6     1    0    -      *           push-1 dot1q 1000
             bobm6               7     1    0    -      *           push-1 dot1q 1000
             bobm7               8     1    0    -      *           push-1 dot1q 1000
             bobm8               9     1    0    -      *           push-1 dot1q 1000
             bobm9               10    1    0    -      *           push-1 dot1q 1000
             bobm1               2     1    0    -      *           push-1 dot1q 1000
            bobm10               11    1    0    -      *           push-1 dot1q 1000
            bobm11               12    1    0    -      *           push-1 dot1q 1000
            bobm12               13    1    0    -      *           push-1 dot1q 1000
            bobm13               14    1    0    -      *           push-1 dot1q 1000
            bobm14               15    1    0    -      *           push-1 dot1q 1000
            bobm15               16    1    0    -      *           push-1 dot1q 1000
            bobm16               17    1    0    -      *           push-1 dot1q 1000
            bobm17               18    1    0    -      *           push-1 dot1q 1000
            bobm18               19    1    0    -      *           push-1 dot1q 1000
            bobm19               20    1    0    -      *           push-1 dot1q 1000
             bobm2               3     1    0    -      *           push-1 dot1q 1000
            bobm20               21    1    0    -      *           push-1 dot1q 1000
            bobm21               22    1    0    -      *           push-1 dot1q 1000
            bobm22               23    1    0    -      *           push-1 dot1q 1000
            bobm23               24    1    0    -      *           push-1 dot1q 1000
            bobm24               25    1    0    -      *           push-1 dot1q 1000

  IP4/IP6 to MAC table for ARP Termination
               192.168.0.1               =>   22:59:d0:d5:19:a9
              fc02:1000::1               =>   22:59:d0:d5:19:a9
```

## counter

### clear counters

```
vpp# clear error          # reset error counters
vpp# clear interfaces     # reset rx/tx packet counters on all interfaces
vpp# clear ip neighbor    # flush ARP/ND table
```

### interface counters (per-interface detail)

```
vpp# show interface bobm11
vpp# show interface counters          # rx/tx summary for all interfaces
```

### error counter

* `show error`
```
vpp# show error
   Count                  Node                              Reason               Severity
     13610        acl-plugin-in-ip4-fa                ACL permit packets           error
     13610        acl-plugin-in-ip4-fa                 checked packets             error
      4058        acl-plugin-in-ip6-fa                ACL permit packets           error
      4058        acl-plugin-in-ip6-fa                 checked packets             error
      1176        acl-plugin-in-ip6-l2                ACL permit packets           error
      1176        acl-plugin-in-ip6-l2                 checked packets             error
     10000            ipip4-input                    packets decapsulated          error
     10000            ipip4-input                         no tunnel                error
       252           ip6-icmp-input                      unknown type              error
       974           ip6-icmp-input              neighbor advertisements sent      info
         4           ip6-icmp-input           neighbor discovery not configured    error
     18060             bond-input                          no error                error
       390             bond-input            pass through (CDP, LLDP, slow proto   error
      4788             l2-output                      L2 output packets            error
      1176              l2-learn                       L2 learn packets            error
      2100              l2-input                       L2 input packets            error
       168              l2-flood                       L2 flood packets            error
      2616        acl-plugin-in-ip4-fa                ACL permit packets           error
      2616        acl-plugin-in-ip4-fa                 checked packets             error
      3524        acl-plugin-in-ip6-fa                ACL permit packets           error
      3524        acl-plugin-in-ip6-fa                 checked packets             error
      1176        acl-plugin-in-ip6-l2                ACL permit packets           error
      1176        acl-plugin-in-ip6-l2                 checked packets             error
       252           ip6-icmp-input                      unknown type              error
       974           ip6-icmp-input              neighbor advertisements sent      info
         2           ip6-icmp-input           neighbor discovery not configured    error
      6531             bond-input                          no error                error
       390             bond-input            pass through (CDP, LLDP, slow proto   error
      4788             l2-output                      L2 output packets            error
      1176              l2-learn                       L2 learn packets            error
      2100              l2-input                       L2 input packets            error
       168              l2-flood                       L2 flood packets            error
vpp# clear error
```

---

## packet tracing

Capture live packets through the graph nodes — essential for debugging.

* Enable tracing (capture next 100 packets from a dpdk interface)
```
vpp# trace add dpdk-input 100
```

* For af_packet / tap / memif / bond interfaces use the matching input node
```
vpp# trace add af-packet-input 100
vpp# trace add virtio-input 100
vpp# trace add bond-process 100
```

* Show captured traces
```
vpp# show trace
```

* Clear trace buffer
```
vpp# clear trace
```

A trace shows each graph node the packet visits with timestamps, e.g.:
```
00:00:01:234567: dpdk-input
  GigabitEthernet0/0/0 rx queue 0
00:00:01:234569: ip4-lookup
  fib 0 dpo-idx 5 flow hash: 0x00000000
00:00:01:234571: ip4-rewrite
  tx_sw_if_index 2 ...
```

---

## bond

* `show bond` — list bond interfaces and their members
```
vpp# show bond
 ID   sw_if_index  Mode        Load balance  Active members  Interface
  0        65      LACP        L34               2           BondEthernet101
```

* `show bond details` — include member state and LACP PDU counters
```
vpp# show bond details
```

* `show lacp` — per-member LACP state machine
```
vpp# show lacp
```

---

## ACL

* `show acl-plugin acl` — list all ACLs and their rules
```
vpp# show acl-plugin acl
```

* `show acl-plugin interface` — show which ACLs are applied to which interfaces
```
vpp# show acl-plugin interface
```

* `show acl-plugin tables` — internal classifier table details
```
vpp# show acl-plugin tables
```

---

## tunnel

### IPinIP tunnel

```
vpp# create ipip tunnel src 192.168.0.1 p2mp outer-table-id 0
ipip0
vpp# set interface state ipip0 up
vpp# set interface ip table ipip0 0
vpp# set interface unnumbered ipip0 use bvi1000
vpp# show ipip tunnel
[0] instance 0 p2mp src 192.168.0.1 table-ID 0 sw-if-idx 76 flags [none] dscp CS0
vpp# delete ipip tunnel sw_if_index 76
vpp# show ipip tunnel
No IPIP tunnels configured...
```

### VxLAN tunnel

```
vpp# create vxlan tunnel src 10.0.0.1 dst 10.0.0.2 vni 100
vpp# show vxlan tunnel
vpp# delete vxlan tunnel src 10.0.0.1 dst 10.0.0.2 vni 100
```

---

## misc / useful one-liners

```
vpp# show api histogram          # API call latency histogram (slow API detection)
vpp# show runtime                # per-node packet/cycle statistics (CPU profiling)
vpp# show event-logger           # internal VPP event log (timing issues)
vpp# show memory                 # heap memory usage per thread
vpp# show memory verbose          # detailed per-allocator breakdown
vpp# show cpu                    # CPU frequency and NUMA topology
vpp# show ip punt redirect       # ip punt redirect rules
vpp# show classify tables verbose  # packet classifier tables + sessions
vpp# show inacl interface GigabitEthernet0/0/0  # ACL tables attached to interface
```
