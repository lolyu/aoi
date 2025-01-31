# tcp max connection limit
* how many tcp connections can be established on Linux?

* things to consider:
    * tcp port range
    * file descriptor limit
    * tcp buffer limit

## tcp port range
```
# sysctl net.ipv4.ip_local_port_range
net.ipv4.ip_local_port_range = 32768    60999    
```

## file descriptor limit
* ulimit `NOFILE`
* `fs.file-max`
* `fs.nr_open`

## tcp buffer limit
* if the tcp connection transmits some data, the send/write buffer will be allocated:

```
# sysctl -a | grep tcp | grep mem
net.ipv4.tcp_mem = 769143       1025527 1538286
net.ipv4.tcp_rmem = 4096        131072  6291456
net.ipv4.tcp_wmem = 4096        16384   4194304
```

## references
* https://www.kernel.org/doc/Documentation/networking/ip-sysctl.txt
