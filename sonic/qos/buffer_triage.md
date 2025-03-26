# buffer triage

* steps:
    * disable tx on the VLAN port on t0
    * send downstream traffic to hit the off threshold, see the behaviors of ingress port.

## MMU config
```
# mmuconfig -l
Pool: egress_lossless_pool
----  --------
mode  static
size  32599040
type  egress
----  --------

Pool: egress_lossy_pool
----  --------
mode  dynamic
size  24709632
type  egress
----  --------

Pool: ingress_lossless_pool
----  --------
mode  dynamic
size  32732160
type  ingress
xoff  1622016
----  --------

Profile: egress_lossless_profile
---------  --------------------
pool       egress_lossless_pool
size       0
static_th  32599040
---------  --------------------

Profile: egress_lossy_profile
----------  -----------------
dynamic_th  3
pool        egress_lossy_pool
size        1792
----------  -----------------

Profile: ingress_lossy_profile
---------  ---------------------
pool       ingress_lossless_pool
size       0
static_th  32732160
---------  ---------------------

Profile: pg_lossless_100000_300m_profile
----------  ---------------------
dynamic_th  0
pool        ingress_lossless_pool
size        4608
xoff        160000
xon         4608
xon_offset  4608
----------  ---------------------
```


## topo
```
pkt ingress -> Ethernet112(uplink) -> DUT -> Ethernet4 (Vlan downlink) -> ptf
```

## lossy PG triage
* the lossy PG at most buffers 32732160 bytes
* the lossy queue at most buffers 24709632 * 2 ^ 3 / (1 + 2 ^ 3) = 21,964,117

1. disable `Ethernet4` tx
    * use [`set_port_tx`](https://github.com/lolyu/aoi/tree/master/scripts) to disable port tx on a vlan port.

2. send 40000 packets from ptf to `Ethernet112`
```
>>> pkt
<Ether  dst=00:01:02:03:04:05 src=00:06:07:08:09:0a type=0x800 |<IP  ihl=None tos=0x0 id=1 ttl=64 proto=hopopt src=192.168.0.1 dst=192.168.0.2 |<Raw  load='\x00\x01\x02\x03\x04\x05\x06\x07\x08\t\n\x0b\x0c\r\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !"#$%&\'()*+,-./0123456789:;<=>?@A' |>>>
>>> len(pkt)
100
>>> sendp(pkt, iface="eth29", count=40000)
```
3. check PG && queue watermark, drop counter, no noticeable drops.
    * **each cell is of 10240512 / 40000 ~= 256 bytes**
```
# portstat -i Ethernet112; show priority-group drop counters | grep Ethernet112; show priority-group watermark shared | grep Ethernet112
      IFACE    STATE    RX_OK    RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK    TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
-----------  -------  -------  --------  ---------  --------  --------  --------  -------  --------  ---------  --------  --------  --------
Ethernet112        U   40,297  2.53 B/s      0.00%         0         9         0      264  3.43 B/s      0.00%         0         0         0
Ethernet112    297      0      0      0      0      0      0      0
Ethernet112  10240512      0      0      0      0      0      0      0
# portstat -i Ethernet4; show queue counters | grep -w Ethernet4 ; show queue watermark unicast | grep -w Ethernet4
    IFACE    STATE    RX_OK     RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK    TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
---------  -------  -------  ---------  ---------  --------  --------  --------  -------  --------  ---------  --------  --------  --------
Ethernet4        U      306  24.02 B/s      0.00%         0         0         0      313  0.00 B/s      0.00%         0         0         0
Ethernet4    UC0               0                0            1             0
Ethernet4    UC1               0                0            0             0
Ethernet4    UC2               0                0            0             0
Ethernet4    UC3               0                0            0             0
Ethernet4    UC4               0                0            0             0
Ethernet4    UC5               0                0            0             0
Ethernet4    UC6               0                0            0             0
Ethernet4    UC7             296            47427            0             0
Ethernet4    UC8               0                0            0             0
Ethernet4    UC9               0                0            0             0
Ethernet4   MC10              17             1241            0             0
Ethernet4   MC11               0                0            0             0
Ethernet4   MC12               0                0            0             0
Ethernet4   MC13               0                0            0             0
Ethernet4   MC14               0                0            0             0
Ethernet4   MC15               0                0            0             0
Ethernet4   MC16               0                0            0             0
Ethernet4   MC17               0                0            0             0
Ethernet4   MC18               0                0            0             0
Ethernet4   MC19               0                0            0             0
  Ethernet4      0  10238208      0      0      0      0      0  79616      0      0
```

4. send another 40000 packets
    * `Ethernet112:PG0` only takes `10240512`, why they are dropped?
```
# portstat -i Ethernet112; show priority-group drop counters | grep Ethernet112; show priority-group watermark shared | grep Ethernet112
      IFACE    STATE    RX_OK      RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK    TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
-----------  -------  -------  ----------  ---------  --------  --------  --------  -------  --------  ---------  --------  --------  --------
Ethernet112        U   80,617  128.14 B/s      0.00%         0         9         0      353  0.00 B/s      0.00%         0         0         0
Ethernet112    615      0      0      0      0      0      0      0
Ethernet112  20480512      0      0      0      0      0      0      0
# portstat -i Ethernet4; show queue counters | grep -w Ethernet4 ; show queue watermark unicast | grep -w Ethernet4
    IFACE    STATE    RX_OK     RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK    TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
---------  -------  -------  ---------  ---------  --------  --------  --------  -------  --------  ---------  --------  --------  --------
Ethernet4        U      657  31.21 B/s      0.00%         0         0         0      313  0.00 B/s      0.00%         0         0         0
Ethernet4    UC0               0                0            1             0
Ethernet4    UC1               0                0            0             0
Ethernet4    UC2               0                0            0             0
Ethernet4    UC3               0                0            0             0
Ethernet4    UC4               0                0            0             0
Ethernet4    UC5               0                0            0             0
Ethernet4    UC6               0                0            0             0
Ethernet4    UC7             296            47427            0             0
Ethernet4    UC8               0                0            0             0
Ethernet4    UC9               0                0            0             0
Ethernet4   MC10              17             1241            0             0
Ethernet4   MC11               0                0            0             0
Ethernet4   MC12               0                0            0             0
Ethernet4   MC13               0                0            0             0
Ethernet4   MC14               0                0            0             0
Ethernet4   MC15               0                0            0             0
Ethernet4   MC16               0                0            0             0
Ethernet4   MC17               0                0            0             0
Ethernet4   MC18               0                0            0             0
Ethernet4   MC19               0                0            0             0
  Ethernet4      0  20478208      0      0      0      0      0  200192      0      0
```

5. send another 10000 packets, all dropped
```
# portstat -i Ethernet112; show priority-group drop counters | grep Ethernet112; show priority-group watermark shared | grep Ethernet112
      IFACE    STATE    RX_OK      RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK    TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
-----------  -------  -------  ----------  ---------  --------  --------  --------  -------  --------  ---------  --------  --------  --------
Ethernet112        U   90,750  140.14 B/s      0.00%         0    10,009         0      353  0.00 B/s      0.00%         0         0         0
Ethernet112  10750      0      0      0      0      0      0      0
Ethernet112  20480512      0      0      0      0      0      0      0
# portstat -i Ethernet4; show queue counters | grep -w Ethernet4 ; show queue watermark unicast | grep -w Ethernet4
    IFACE    STATE    RX_OK     RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK    TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
---------  -------  -------  ---------  ---------  --------  --------  --------  -------  --------  ---------  --------  --------  --------
Ethernet4        U      694  19.69 B/s      0.00%         0         0         0      313  0.00 B/s      0.00%         0         0         0
Ethernet4    UC0               0                0            1             0
Ethernet4    UC1               0                0            0             0
Ethernet4    UC2               0                0            0             0
Ethernet4    UC3               0                0            0             0
Ethernet4    UC4               0                0            0             0
Ethernet4    UC5               0                0            0             0
Ethernet4    UC6               0                0            0             0
Ethernet4    UC7             296            47427            0             0
Ethernet4    UC8               0                0            0             0
Ethernet4    UC9               0                0            0             0
Ethernet4   MC10              17             1241            0             0
Ethernet4   MC11               0                0            0             0
Ethernet4   MC12               0                0            0             0
Ethernet4   MC13               0                0            0             0
Ethernet4   MC14               0                0            0             0
Ethernet4   MC15               0                0            0             0
Ethernet4   MC16               0                0            0             0
Ethernet4   MC17               0                0            0             0
Ethernet4   MC18               0                0            0             0
Ethernet4   MC19               0                0            0             0
  Ethernet4      0  20478208      0      0      0      0      0  200192      0      0
# show queue watermark unicast
Egress shared pool occupancy per unicast queue:
       Port    UC0       UC1    UC2    UC3    UC4    UC5    UC6     UC7    UC8    UC9
-----------  -----  --------  -----  -----  -----  -----  -----  ------  -----  -----
  Ethernet0      0         0      0      0      0      0      0       0      0      0
  Ethernet4      0  20478208      0      0      0      0      0  200192      0      0
  Ethernet8      0         0      0      0      0      0      0       0      0      0
 Ethernet12      0         0      0      0      0      0      0       0      0      0
 Ethernet16      0         0      0      0      0      0      0       0      0      0
 Ethernet20      0         0      0      0      0      0      0       0      0      0
 Ethernet24      0         0      0      0      0      0      0       0      0      0
 Ethernet28      0         0      0      0      0      0      0       0      0      0
 Ethernet32      0         0      0      0      0      0      0       0      0      0
 Ethernet36      0         0      0      0      0      0      0       0      0      0
 Ethernet40      0         0      0      0      0      0      0       0      0      0
 Ethernet44      0         0      0      0      0      0      0       0      0      0
 Ethernet48      0         0      0      0      0      0      0       0      0      0
 Ethernet52      0         0      0      0      0      0      0       0      0      0
 Ethernet56      0         0      0      0      0      0      0       0      0      0
 Ethernet60      0         0      0      0      0      0      0       0      0      0
 Ethernet64      0         0      0      0      0      0      0       0      0      0
 Ethernet68      0         0      0      0      0      0      0       0      0      0
 Ethernet72      0         0      0      0      0      0      0       0      0      0
 Ethernet76      0         0      0      0      0      0      0       0      0      0
 Ethernet80      0         0      0      0      0      0      0       0      0      0
 Ethernet84      0         0      0      0      0      0      0       0      0      0
 Ethernet88      0         0      0      0      0      0      0       0      0      0
 Ethernet92      0         0      0      0      0      0      0       0      0      0
 Ethernet96      0         0      0      0      0      0      0       0      0      0
Ethernet100      0         0      0      0      0      0      0       0      0      0
Ethernet104      0         0      0      0      0      0      0       0      0      0
Ethernet108      0         0      0      0      0      0      0       0      0      0
Ethernet112      0         0      0      0      0      0      0    3072      0      0
Ethernet116      0         0      0      0      0      0      0    3072      0      0
Ethernet120      0         0      0      0      0      0      0    3072      0      0
Ethernet124      0         0      0      0      0      0      0    3072      0      0
Ethernet128      0         0      0      0      0      0      0       0      0      0
Ethernet132      0         0      0      0      0      0      0       0      0      0
```

## lossless PG triage


