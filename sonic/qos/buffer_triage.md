# buffer triage

* steps:
    * disable tx on the VLAN port on t0
    * send downstream traffic to hit the off threshold, see the behaviors of ingress port.

## MMU config
```
Pool: ingress_lossless_pool
----  --------
mode  dynamic
size  32732160
type  ingress
xoff  1622016
----  --------

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
1. disable `Ethernet4` tx
    * use [`set_port_tx`](https://github.com/lolyu/aoi/tree/master/scripts) to disable port tx on a vlan port.

2. send 40000 packets from ptf to `Ethernet112`
```
>>> pkt
<Ether  dst=00:01:02:03:04:05 src=00:06:07:08:09:0a type=0x800 |<IP  ihl=None tos=0x0 id=1 ttl=64 proto=hopopt src=192.168.0.1 dst=192.168.0.2 |<Raw  load='\x00\x01\x02\x03\x04\x05\x06\x07\x08\t\n\x0b\x0c\r\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !"#$%&\'()*+,-./0123456789:;<=>?@A' |>>>
>>> sendp(pkt, iface="eth29", count=21000)
```
3. check watermark && PG drop counter, no noticeable drops
```
# portstat -i Ethernet112; show priority-group drop counters | grep Ethernet112; show priority-group watermark shared | grep Ethernet112
Last cached time was 2025-03-25T14:21:24.683784
      IFACE    STATE    RX_OK      RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK    TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
-----------  -------  -------  ----------  ---------  --------  --------  --------  -------  --------  ---------  --------  --------  --------
Ethernet112        U   40,227  129.12 B/s      0.00%         0         0         0      110  0.00 B/s      0.00%         0         0         0
Ethernet112    228      0      0      0      0      0      0      0
Ethernet112  10240512      0      0      0      0      0      0      0
```

3. continue to send 20000 packets, all dropped
    * `Ethernet112:PG0` only takes `10240512`, why they are dropped?
```
# portstat -i Ethernet112; show priority-group drop counters | grep Ethernet112; show priority-group watermark shared | grep Ethernet112
Last cached time was 2025-03-25T14:21:24.683784
      IFACE    STATE    RX_OK      RX_BPS    RX_UTIL    RX_ERR    RX_DRP    RX_OVR    TX_OK    TX_BPS    TX_UTIL    TX_ERR    TX_DRP    TX_OVR
-----------  -------  -------  ----------  ---------  --------  --------  --------  -------  --------  ---------  --------  --------  --------
Ethernet112        U   60,339  156.90 B/s      0.00%         0    20,000         0      110  0.00 B/s      0.00%         0         0         0
Ethernet112  20342      0      0      0      0      0      0      0
Ethernet112  10240512      0      0      0      0      0      0      0
```

## lossless PG triage


