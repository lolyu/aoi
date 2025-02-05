# send tuning
## keep packet size less than MTU
## zero-copy optimization
* two ways:
    * `mmap`
    * `sendfile`

## NIC offloading
* the principle is to delay the packet segmentation to save the protocol header calculation overhead in the network stack, and offload the segmentation to lower layers (NIC).
```
# ethtool -k eth0 | grep seg
tcp-segmentation-offload: on
        tx-tcp-segmentation: on
        tx-tcp-ecn-segmentation: on
        tx-tcp-mangleid-segmentation: off
        tx-tcp6-segmentation: on
generic-segmentation-offload: on
```


## references
* https://docs.kernel.org/networking/segmentation-offloads.html
