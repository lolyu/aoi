# application tuning

## no blocking read/write
## kernel bypass - dpdk
## TCP_NODELAY
* disable the TCP Nagle algorithm
```
# int one = 1;
# setsockopt(descriptor, SOL_TCP, TCP_NODELAY, &one, sizeof(one));
```

## TCP_CORK


## references
* https://docs.redhat.com/zh-cn/documentation/red_hat_enterprise_linux_for_real_time/7/html/tuning_guide/tcp_nodelay_and_small_buffer_writes
