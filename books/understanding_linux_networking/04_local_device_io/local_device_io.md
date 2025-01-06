# local device I/O
* how does Linux handles the packet forward/receive on the local machine?
	* to sum up: Linux stores local routes to local IPs in the `LOCAL` route table; when the kernel handles traffic to local IPs, the kernel will do a fib lookup, which will try to find a match in the `LOCAL` route table. In this case, the routes found are all local routes in the `LOCAL` route table, so the out device is replaced with the loopback device `lo`; as the loopback has no output queue, the kernel will try to call the device's transmit op function, which will store the packets in the per-cpu backlog and trigger a softirq `NET_RX_SOFTIRQ` to notify `softirqd` to process the packets in the backlog.


## kernel network layer
```c
int ip_queue_xmit(struct sk_buff *skb, struct flowi *fl)
{
	struct sock *sk = skb->sk;
	struct inet_sock *inet = inet_sk(sk);
	struct ip_options_rcu *inet_opt;
	struct flowi4 *fl4;
	struct rtable *rt;
	struct iphdr *iph;
	int res;

    ...

	/* Make sure we can route this packet. */
	rt = (struct rtable *)__sk_dst_check(sk, 0);
	if (rt == NULL) {

        ...
		/* If this fails, retransmit mechanism of transport layer will
		 * keep trying until route appears or the connection times
		 * itself out.
		 */
		rt = ip_route_output_ports(sock_net(sk), fl4, sk,
					   daddr, inet->inet_saddr,
					   inet->inet_dport,
					   inet->inet_sport,
					   sk->sk_protocol,
					   RT_CONN_FLAGS(sk),
					   sk->sk_bound_dev_if);
		...
		sk_setup_caps(sk, &rt->dst);
	}
	skb_dst_set_noref(skb, &rt->dst);

    ...

	res = ip_local_out(skb);
	return res;
}
```

* `ip_route_output_ports` -> `ip_route_output_flow` -> `__ip_route_output_key` -> `fib_lookup`
* `__ip_route_output_key` will try to resolve the local IP from the FIB via `fib_lookup`; `fib_lookup` will try to find a match from the `LOCAL` route table first, which stores the local routes to local IPs; `__ip_route_output_key` will change the out device to the loopback device `lo` if the route is a local route.

```c
struct rtable *__ip_route_output_key(struct net *net, struct flowi4 *fl4)
{
	struct net_device *dev_out = NULL;
	__u8 tos = RT_FL_TOS(fl4);
	unsigned int flags = 0;
	struct fib_result res;
	struct rtable *rth;
	int orig_oif;

	res.tclassid	= 0;
	res.fi		= NULL;
	res.table	= NULL;

	orig_oif = fl4->flowi4_oif;

	...
	if (fib_lookup(net, fl4, &res)) {
		...
	}

	if (res.type == RTN_LOCAL) {
		if (!fl4->saddr) {
			if (res.fi->fib_prefsrc)
				fl4->saddr = res.fi->fib_prefsrc;
			else
				fl4->saddr = fl4->daddr;
		}
		dev_out = net->loopback_dev;
		fl4->flowi4_oif = dev_out->ifindex;
		flags |= RTCF_LOCAL;
		goto make_route;
	}

	...
	rth = __mkroute_output(&res, fl4, orig_oif, dev_out, flags);

out:
	rcu_read_unlock();
	return rth;
}
```

```c
static inline int fib_lookup(struct net *net, const struct flowi4 *flp,
			     struct fib_result *res)
{
	struct fib_table *table;

	table = fib_get_table(net, RT_TABLE_LOCAL);
	if (!fib_table_lookup(table, flp, res, FIB_LOOKUP_NOREF))
		return 0;

	table = fib_get_table(net, RT_TABLE_MAIN);
	if (!fib_table_lookup(table, flp, res, FIB_LOOKUP_NOREF))
		return 0;
	return -ENETUNREACH;
}
```
* the `LOCAL` route table
```
# ip route show table local
local 10.0.0.56 dev PortChannel101 proto kernel scope host src 10.0.0.56
local 10.0.0.58 dev PortChannel102 proto kernel scope host src 10.0.0.58
local 10.0.0.60 dev PortChannel103 proto kernel scope host src 10.0.0.60
local 10.0.0.62 dev PortChannel104 proto kernel scope host src 10.0.0.62
local 10.1.0.32 dev Loopback0 proto kernel scope host src 10.1.0.32
local 10.1.0.34 dev Loopback1 proto kernel scope host src 10.1.0.34
local 10.1.0.36 dev Loopback2 proto kernel scope host src 10.1.0.36
local 10.1.0.38 dev tun0 proto kernel scope host src 10.1.0.38
local 10.1.0.38 dev Loopback3 proto kernel scope host src 10.1.0.38
local 10.206.144.131 dev eth0 proto kernel scope host src 10.206.144.131
broadcast 10.206.144.255 dev eth0 proto kernel scope link src 10.206.144.131
local 127.0.0.0/16 dev lo proto kernel scope host src 127.0.0.1
local 127.0.0.1 dev lo proto kernel scope host src 127.0.0.1
broadcast 127.0.255.255 dev lo proto kernel scope link src 127.0.0.1
```

## kernel neighbor system
* how does the kernel neighbor system handles the traffic to local IPs?
* will the kernel neighbor system create ARP entries for local IPs?

## `dev_queue_xmit`
```c
int dev_queue_xmit(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	struct netdev_queue *txq;
	struct Qdisc *q;
	int rc = -ENOMEM;

	...


	if (q->enqueue) {
		rc = __dev_xmit_skb(skb, q, dev, txq);
		goto out;
	}

	/* The device has no queue. Common case for software devices:
	   loopback, all the sorts of tunnels...

	   Really, it is unlikely that netif_tx_lock protection is necessary
	   here.  (f.e. loopback and IP tunnels are clean ignoring statistics
	   counters.)
	   However, it is possible, that they rely on protection
	   made by us here.

	   Check this and shot the lock. It is not prone from deadlocks.
	   Either shot noqueue qdisc, it is even simpler 8)
	 */
	if (dev->flags & IFF_UP) {
		int cpu = smp_processor_id(); /* ok because BHs are off */

		if (txq->xmit_lock_owner != cpu) {

			...

			if (!netif_xmit_stopped(txq)) {
				__this_cpu_inc(xmit_recursion);
				rc = dev_hard_start_xmit(skb, dev, txq);
				if (dev_xmit_complete(rc)) {
					goto out;
				}
			}
			HARD_TX_UNLOCK(dev, txq);
			net_crit_ratelimited("Virtual device %s asks to queue packet!\n",
					     dev->name);
		} else {
			...
		}
	}

	rc = -ENETDOWN;
	kfree_skb(skb);
	return rc;
out:
	rcu_read_unlock_bh();
	return rc;
}
```
* if the out device has queue -> `__dev_xmit_skb`
* if the out device has no queue -> `dev_hard_start_xmit` -> `dev->netdev_ops->ndo_start_xmit`
```c
static const struct net_device_ops loopback_ops = {
	.ndo_init      = loopback_dev_init,
	.ndo_start_xmit= loopback_xmit,
	.ndo_get_stats64 = loopback_get_stats64,
};

static netdev_tx_t loopback_xmit(struct sk_buff *skb,
				 struct net_device *dev)
{
	struct pcpu_lstats *lb_stats;
	int len;

	skb_orphan(skb);

	..

	len = skb->len;
	if (likely(netif_rx(skb) == NET_RX_SUCCESS)) {
		...
	}

	return NETDEV_TX_OK;
}
```

### `netif_rx`
```c
int netif_rx(struct sk_buff *skb)
{
	int ret;

	...
	unsigned int qtail;
	ret = enqueue_to_backlog(skb, get_cpu(), &qtail);
	...
	return ret;
}

static int enqueue_to_backlog(struct sk_buff *skb, int cpu,
			      unsigned int *qtail)
{
	struct softnet_data *sd;
	unsigned long flags;

	sd = &per_cpu(softnet_data, cpu);


	...
	if (skb_queue_len(&sd->input_pkt_queue) <= netdev_max_backlog) {
		if (skb_queue_len(&sd->input_pkt_queue)) {
enqueue:
			__skb_queue_tail(&sd->input_pkt_queue, skb);
			input_queue_tail_incr_save(sd, qtail);
			return NET_RX_SUCCESS;
		}

		/* Schedule NAPI for backlog device
		 * We can use non atomic operation since we own the queue lock
		 */
		if (!__test_and_set_bit(NAPI_STATE_SCHED, &sd->backlog.state)) {
			if (!rps_ipi_queued(sd))
				____napi_schedule(sd, &sd->backlog);
		}
		goto enqueue;
	}

	sd->dropped++;
	rps_unlock(sd);

	local_irq_restore(flags);

	atomic_long_inc(&skb->dev->rx_dropped);
	kfree_skb(skb);
	return NET_RX_DROP;
}

static inline void ____napi_schedule(struct softnet_data *sd,
				     struct napi_struct *napi)
{
	list_add_tail(&napi->poll_list, &sd->poll_list);
	__raise_softirq_irqoff(NET_RX_SOFTIRQ);
}
```

## packet receivement
* the per cpu backlog poll function is `process_backlog`
```c
static int __init net_dev_init(void)
{
	...
	for_each_possible_cpu(i) {
		struct softnet_data *sd = &per_cpu(softnet_data, i);

		memset(sd, 0, sizeof(*sd));
		...
		sd->backlog.poll = process_backlog;
		...
	}

	...
}

static int process_backlog(struct napi_struct *napi, int quota)
{
	int work = 0;
	struct softnet_data *sd = container_of(napi, struct softnet_data, backlog);

	...
	napi->weight = weight_p;
	local_irq_disable();
	while (work < quota) {
		struct sk_buff *skb;
		unsigned int qlen;

		while ((skb = __skb_dequeue(&sd->process_queue))) {
			__netif_receive_skb(skb);
			input_queue_head_incr(sd);
			if (++work >= quota) {
				return work;
			}
		}


		qlen = skb_queue_len(&sd->input_pkt_queue);
		if (qlen)
			skb_queue_splice_tail_init(&sd->input_pkt_queue,
						   &sd->process_queue);

		...
	}

	return work;
}
```
