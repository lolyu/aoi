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

## XPS (transmit packet steering)
```
# cat /sys/class/net/<port>/queues/tx-<queue>/xps_cpus
```

## ebpf
* use ebpf to bypass linux networking stack for local socket communication:
    * `sockmap`
    * `sk redirect`

![image](https://github.com/user-attachments/assets/61ff7b7c-449d-451e-9e8a-d4e307986f3f)

## references
* https://docs.kernel.org/networking/segmentation-offloads.html
* https://arthurchiao.art/blog/socket-acceleration-with-ebpf-zh/
