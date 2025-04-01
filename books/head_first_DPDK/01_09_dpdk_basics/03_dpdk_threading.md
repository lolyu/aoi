# DPDK threading

## CPU basics
* `number of logical cores` = `threads per core` * `cores per socket` * `socket`
```
$ lscpu
Architecture:                       x86_64
CPU op-mode(s):                     32-bit, 64-bit
Byte Order:                         Little Endian
Address sizes:                      46 bits physical, 48 bits virtual
CPU(s):                             56                                          // number of logical cores
On-line CPU(s) list:                0-55
Thread(s) per core:                 2                                           // HT
Core(s) per socket:                 14                                          // the number of physical CPU package, one socket hosts one CPU package
Socket(s):                          2
NUMA node(s):                       2
Vendor ID:                          GenuineIntel
CPU family:                         6
Model:                              85
Model name:                         Intel(R) Xeon(R) Gold 5120 CPU @ 2.20GHz
Stepping:                           4
CPU MHz:                            1000.238
CPU max MHz:                        3200.0000
CPU min MHz:                        1000.0000
BogoMIPS:                           4400.00
Virtualization:                     VT-x
L1d cache:                          896 KiB
L1i cache:                          896 KiB
L2 cache:                           28 MiB
L3 cache:                           38.5 MiB
```

## CPU affinity

* what's the benefit to set the process CPU affinity?
    * avoid the cache invalidation from the process CPU migrations.

* best practices:
    * use a CPU core for all system processes and set the application to run the rest of cores.
    * configure a thread application and a specific kernel thread on the same CPU (network `softirq` or driver thread on the same CPU with a user thread)
    * pair the producer/consumer threads on each CPU

### how to set process affinity?


## references
* https://docs.redhat.com/en/documentation/red_hat_enterprise_linux_for_real_time/8/html/optimizing_rhel_8_for_real_time_for_low_latency_operation/assembly_setting-cpu-affinity-on-rhel-for-real-time_optimizing-rhel8-for-real-time-for-low-latency-operation#assembly_setting-cpu-affinity-on-rhel-for-real-time_optimizing-RHEL8-for-real-time-for-low-latency-operation
