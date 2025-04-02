## DPDK threading

### CPU basics
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

### CPU affinity

* what's the benefit to set the process CPU affinity?
    * avoid the cache invalidation from the process CPU migrations.

* best practices:
    * use a CPU core for all system processes and set the application to run the rest of cores.
    * configure a thread application and a specific kernel thread on the same CPU (network `softirq` or driver thread on the same CPU with a user thread)
    * pair the producer/consumer threads on each CPU

#### how to set process affinity?

### DPDK threading
* `lcore`: represents `EAL`thread, is based on `pthread`
    * `lcore` is essentially a thread managed by the Environment Abstraction Layer (EAL)
    *  these threads execute tasks issued by `rte_eal_remote_launch`.
    *  each `lcore`(EAL thread) has a unique identifier called `lcore_id` stored in the thread local storage.
    *  `EAL` thread is scheduled by linux kernel
* the helloworld example:
```c
	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_panic("Cannot init EAL\n");
	/* >8 End of initialization of Environment Abstraction Layer */

	/* Launches the function on each lcore. 8< */
	RTE_LCORE_FOREACH_WORKER(lcore_id) {
		/* Simpler equivalent. 8< */
		rte_eal_remote_launch(lcore_hello, NULL, lcore_id);
		/* >8 End of simpler equivalent. */
	}

	/* call it on main lcore too */
	lcore_hello(NULL);
	/* >8 End of launching the function on each lcore. */

	rte_eal_mp_wait_lcore();

	/* clean up the EAL */
	rte_eal_cleanup();
```

* the whole workflow:
<img width="433" alt="image" src="https://github.com/user-attachments/assets/e880ddc9-5adb-4ff1-add0-7390d4c76e19" />

#### `EAL` thread affinity
1. `--lcores` option
```
--lcores=<lcore_set>[@cpu_set],<lcore_set>[@cpu_set]
```
* `--lcores='1,2@(5-7),(3-5)@(0,2),(0,6),7-8`:
    * `lcore` 1 -> cpu 1
    * `lcore` 2 -> cpu 5, 6, 7
    * `lcore` 3 ~ 5 -> cpu 0, 2
    * `lcore` 0 -> cpu 0
    * `lcore` 6 -> cpu 6
    * `lcore` 7 -> cpu 7
    * `lcore` 8 -> cpu 8

2. DPDK API
```c
rte_thread_set_affinity()
rte_thread_get_affinity()
```

#### use `cgroup` to manage CPU resources
* **TODO**

## references
* https://docs.redhat.com/en/documentation/red_hat_enterprise_linux_for_real_time/8/html/optimizing_rhel_8_for_real_time_for_low_latency_operation/assembly_setting-cpu-affinity-on-rhel-for-real-time_optimizing-rhel8-for-real-time-for-low-latency-operation#assembly_setting-cpu-affinity-on-rhel-for-real-time_optimizing-RHEL8-for-real-time-for-low-latency-operation
