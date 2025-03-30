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
pkt ingress -> Ethernet120(uplink) -> DUT -> Ethernet4 (Vlan downlink) -> ptf
```

## lossy PG triage
* the lossy PG at most buffers 32732160 bytes
* the lossy queue at most buffers 24709632 * 2 ^ 3 / (1 + 2 ^ 3) = 21964117 bytes

1. disable `Ethernet4` tx
    * use [`set_port_tx`](https://github.com/lolyu/aoi/blob/master/scripts/set_port_tx.py) to disable port tx on a vlan port.

2. send 10000 packets from ptf to `Ethernet120`
```
>>> p
<Ether  dst=94:8e:d3:05:1b:1c src=98:03:9b:03:20:01 type=0x800 |<IP  ihl=None tos=0x0 id=1 ttl=64 proto=hopopt src=10.10.10.10 dst=192.168.0.100 |<Raw  load='\x00\x01\x02\x03\x04\x05\x06\x07\x08\t\n\x0b\x0c\r\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !"#$%&\'()*+,-./0123456789:;<=>?@A' |>>>
>>> len(p)
100
>>> sendp(p, iface="eth30", count=10000)
```

3. check PG && queue watermark, drop counter, no noticeable drops.
    * **each cell is of 2556672 / 10000 ~= 256 bytes**
    * the ingress lossy PG could buffer at most 32732160 / 256 = 127860 packets
    * the egress lossy queue could buffer at most 24709632 * 8 / 9 / 256 = 85797.3333

```
# portstat -i Ethernet120,Ethernet4
      IFACE    STATE    RX_OK      RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK     TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
-----------  -------  -------  ----------  ---------  --------  --------  --------  -------  ---------  ---------  --------  --------  --------
  Ethernet4        U        1    0.01 B/s      0.00%         0         0         0      128   0.14 B/s      0.00%         0         0         0
Ethernet120        U   10,254  200.54 B/s      0.00%         0        28         0      202  18.79 B/s      0.00%         0         0         0
# show priority-group watermark shared
Ingress shared pool occupancy per PG:
       Port      PG0    PG1    PG2    PG3    PG4    PG5    PG6    PG7
-----------  -------  -----  -----  -----  -----  -----  -----  -----
Ethernet120  2556672      0      0      0      0      0      0   9216
# show queue watermark unicast
Egress shared pool occupancy per unicast queue:
       Port    UC0      UC1    UC2    UC3    UC4    UC5    UC6    UC7    UC8    UC9
-----------  -----  -------  -----  -----  -----  -----  -----  -----  -----  -----
  Ethernet4      0  2554880      0      0      0      0      0   2048      0      0
```

4. send another 70000 packets
    * `Ethernet112:PG0` only takes `10240512`, why they are dropped?
```
# portstat -i Ethernet120,Ethernet4
      IFACE    STATE    RX_OK     RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK    TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
-----------  -------  -------  ---------  ---------  --------  --------  --------  -------  --------  ---------  --------  --------  --------
  Ethernet4        U        1   0.00 B/s      0.00%         0         0         0      128  0.00 B/s      0.00%         0         0         0
Ethernet120        U   80,311  28.08 B/s      0.00%         0        28         0      244  5.78 B/s      0.00%         0         0         0
# show priority-group watermark shared
Ingress shared pool occupancy per PG:
       Port       PG0    PG1    PG2    PG3    PG4    PG5    PG6    PG7
-----------  --------  -----  -----  -----  -----  -----  -----  -----
Ethernet120  20476672      0      0      0      0      0      0   9216
# show queue watermark unicast
Egress shared pool occupancy per unicast queue:
       Port    UC0       UC1    UC2    UC3    UC4    UC5    UC6    UC7    UC8    UC9
-----------  -----  --------  -----  -----  -----  -----  -----  -----  -----  -----
  Ethernet4      0  20474880      0      0      0      0      0   9728      0      0
```


5. send another 10000 packets, all dropped
    * due to the limitation of egress queue max size, SONiC at most buffers around 85000 packets.
    * the watermark is 21741568, which is close to the queue max size 21964117.
```
# portstat -i Ethernet120,Ethernet4
      IFACE    STATE    RX_OK       RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK    TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
-----------  -------  -------  -----------  ---------  --------  --------  --------  -------  --------  ---------  --------  --------  --------
  Ethernet4        U        1     0.00 B/s      0.00%         0         0         0      128  0.00 B/s      0.00%         0     5,052         0
Ethernet120        U   90,327  117.05 KB/s      0.00%         0        28         0      256  2.14 B/s      0.00%         0         0         0
# show priority-group watermark shared
Ingress shared pool occupancy per PG:
       Port       PG0    PG1    PG2    PG3    PG4    PG5    PG6    PG7
-----------  --------  -----  -----  -----  -----  -----  -----  -----
Ethernet120  21743360      0      0      0      0      0      0   9216
# show queue watermark unicast
Egress shared pool occupancy per unicast queue:
       Port    UC0       UC1    UC2    UC3    UC4    UC5    UC6    UC7    UC8    UC9
-----------  -----  --------  -----  -----  -----  -----  -----  -----  -----  -----
  Ethernet4      0  21741568      0      0      0      0      0  11776      0      0
```

## lossless PG triage
* the lossless PG at most buffers 32732160 * 2 ^ 0 / 1 + 2 ^ 0 = 16366080 bytes = 16366080 /256 = 63930 packets
    * the headroom is 160000 bytes = 160000 / 256 = 625 packets
* the lossless queue at most buffers 32599040 bytes = 32599040 / 256 = 127340 packets


```
>>> p
<Ether  dst=94:8e:d3:05:1b:1c src=98:03:9b:03:20:01 type=0x800 |<IP  ihl=None tos=0xc id=1 ttl=64 proto=hopopt src=10.10.10.10 dst=192.168.0.100 |<Raw  load='\x00\x01\x02\x03\x04\x05\x06\x07\x08\t\n\x0b\x0c\r\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !"#$%&\'()*+,-./0123456789:;<=>?@A' |>>>
```

1. send 100000 packets

```
# portstat -i Ethernet120,Ethernet4
      IFACE    STATE    RX_OK     RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK      TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
-----------  -------  -------  ---------  ---------  --------  --------  --------  -------  ----------  ---------  --------  --------  --------
  Ethernet4        U        5  17.06 B/s      0.00%         0         0         0    8,376  14.32 KB/s      0.00%         0    91,786         0
Ethernet120        U  100,301  85.91 B/s      0.00%         0         7         0      242   58.43 B/s      0.00%         0         0         0
# show priority-group watermark shared
Ingress shared pool occupancy per PG:
       Port    PG0    PG1    PG2      PG3    PG4    PG5    PG6    PG7
-----------  -----  -----  -----  -------  -----  -----  -----  -----
Ethernet120      0      0      0  2092800      0      0      0  18432
# show queue watermark unicast
Egress shared pool occupancy per unicast queue:
       Port    UC0    UC1    UC2      UC3    UC4    UC5    UC6    UC7    UC8    UC9
-----------  -----  -----  -----  -------  -----  -----  -----  -----  -----  -----
  Ethernet4      0      0      0  2097408      0      0      0   7168      0      0
# show queue counters
For namespace :
     Port    TxQ    Counter/pkts    Counter/bytes    Drop/pkts    Drop/bytes
---------  -----  --------------  ---------------  -----------  ------------
Ethernet4    UC3            8193           852072        91786       9545744
# show pfc counters
    Port Rx    PFC0    PFC1    PFC2    PFC3    PFC4    PFC5    PFC6    PFC7
-----------  ------  ------  ------  ------  ------  ------  ------  ------
  Ethernet4       0       0       0       0       0       0       0       0
Ethernet120       0       0       0       0       0       0       0       0

    Port Tx    PFC0    PFC1    PFC2    PFC3    PFC4    PFC5    PFC6    PFC7
-----------  ------  ------  ------  ------  ------  ------  ------  ------
  Ethernet4       0       0       0       0       0       0       0       0
Ethernet120       0       0       0       0       0       0       0       0

```
