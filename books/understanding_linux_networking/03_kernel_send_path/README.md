# kernel send path

## questions
### does softirq handles all packet sending?
* No, the process finishes most of the packets' sending without the the help from `softirqd`.
```c
void __qdisc_run(struct Qdisc *q)
{
	int quota = weight_p;

	while (qdisc_restart(q)) {
		/*
		 * Ordered by possible occurrence: Postpone processing if
		 * 1. we've exceeded packet quota
		 * 2. another process needs the CPU;
		 */
		if (--quota <= 0 || need_resched()) {
			__netif_schedule(q);
			break;
		}
	}

	qdisc_run_end(q);
}
```

#### how to change the quota?
* change the `__qdis_run` quota for all `qdisc` with `sysctl`
```
# cat /proc/sys/net/core/dev_weight
64
# sysctl -w net.core.dev_weight=600 
```

### how many packets can a `qdisc` stores?
* `qdisc` will check if the device has sufficient tx queue buffer when enqueuing packets, the default value is 1000 bytes.
```c
static int pfifo_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
	if (likely(skb_queue_len(&sch->q) < sch->limit))
		return qdisc_enqueue_tail(skb, sch);

	return qdisc_reshape_fail(skb, sch);
}
```
```
# ifconfig eth0
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 10.206.144.131  netmask 255.255.255.0  broadcast 10.206.144.255
        inet6 fe80::2edd:e9ff:fe0f:4e50  prefixlen 64  scopeid 0x20<link>
        inet6 2a01:111:e205:100::ace:9083  prefixlen 64  scopeid 0x0<global>
        ether 2c:dd:e9:0f:4e:50  txqueuelen 1000  (Ethernet)
# ifconfig eth0 txqueuelen 10000
```

### why `NET_RX` is much larger than `NET_TX` in `/proc/softirqs`?
```
# cat /proc/softirqs
                    CPU0       CPU1       CPU2       CPU3
          HI:          2          0          0          5
       TIMER:    2548987    2083573    3639886    2533494
      NET_TX:     202439    2485340     193665     194321
      NET_RX:    5445846    8199945    5885107    5632724
       BLOCK:     145552     264517     152364     152695
    IRQ_POLL:          0          0          0          0
     TASKLET:         18        186       4025         46
       SCHED:    8377631    7394412    7185161    6649592
     HRTIMER:          0          0          0          0
         RCU:   10349387   10342750   10435199   10299188
```
* two reasons:
    1. when the NIC finishes sending packets, it will trigger a hardware IRQ, which in turn triggers a `NET_RX_SOFTIRQ` to cleanup the tx buffer.
    2. the process that is sending packets finishes the packet sending in its kernel mode without the help of `softirqd`.


