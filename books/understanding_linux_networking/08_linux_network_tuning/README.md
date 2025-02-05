# Linux networking tuning

## what is NIC offload?
* due to the MUT limit (1500 bytes), large frames are segmented to smaller frames, which causes Linux system resources to be underutilized.
* NIC offload allows the protocol stack to operates(send/receive) packets larger than MTU, and the segmentation/de-segmentation are offloaded to NICs, this means less IRQ generated, less CPU processing overhead is spent on spliting/combining.
* offload types:
    * send
        * TSO (tcp segmentation offload)
        * UFO (UDP segmentation offload)
        * GSO (generic segmentation offload)
    * receive:
        * LRO (large receive offload)
        * GRO (generic receive offload)

```
# ethtool -k eth0 | grep offload
tcp-segmentation-offload: on
generic-segmentation-offload: on
generic-receive-offload: on
large-receive-offload: off [fixed]
```

## references
