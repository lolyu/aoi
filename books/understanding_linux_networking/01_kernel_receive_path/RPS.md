# Receive Packet Steering
* RPS is logically a software implementation of RSS;
    * Whereas RSS selects the queue and hence CPU that will run the hardware interrupt handler, RPS selects the CPU to perform protocol processing above the interrupt handler.
    * This is accomplished by placing the packet on the desired CPU’s backlog queue and waking up the CPU for processing.
 
## Implementation
* if `RPS` is enabled, `netif_receive_skb` will put the frame into the backlog queue.
```c
/ net/core/dev.c
int netif_receive_skb(struct sk_buff *skb)
{
#ifdef CONFIG_RPS
	if (static_key_false(&rps_needed)) {
		struct rps_dev_flow voidflow, *rflow = &voidflow;
		int cpu, ret;
		cpu = get_rps_cpu(skb->dev, skb, &rflow);

		if (cpu >= 0) {
			ret = enqueue_to_backlog(skb, cpu, &rflow->last_qtail);
			return ret;
		}
	}
#endif
	return __netif_receive_skb(skb);
}
```
```c
// include/linux/netdevice.h
/*
 * Incoming packets are placed on per-cpu queues
 */
struct softnet_data {
	struct Qdisc		*output_queue;
	struct Qdisc		**output_queue_tailp;
	struct list_head	poll_list;
	struct sk_buff		*completion_queue;
	struct sk_buff_head	process_queue;

	/* stats */
	unsigned int		processed;
	unsigned int		time_squeeze;
	unsigned int		cpu_collision;
	unsigned int		received_rps;

#ifdef CONFIG_RPS
	struct softnet_data	*rps_ipi_list;

	/* Elements below can be accessed between CPUs for RPS */
	struct call_single_data	csd ____cacheline_aligned_in_smp;
	struct softnet_data	*rps_ipi_next;
	unsigned int		cpu;
	unsigned int		input_queue_head;
	unsigned int		input_queue_tail;
#endif
	unsigned int		dropped;
	struct sk_buff_head	input_pkt_queue;
	struct napi_struct	backlog;
};
```

### `enqueue_to_backlog`
```c
// net/core/dev.c
static int enqueue_to_backlog(struct sk_buff *skb, int cpu,
			      unsigned int *qtail)
{
	struct softnet_data *sd;
	unsigned long flags;

	sd = &per_cpu(softnet_data, cpu);

	local_irq_save(flags);

	rps_lock(sd);
	if (skb_queue_len(&sd->input_pkt_queue) <= netdev_max_backlog) {
		if (skb_queue_len(&sd->input_pkt_queue)) {
enqueue:
			__skb_queue_tail(&sd->input_pkt_queue, skb);
			input_queue_tail_incr_save(sd, qtail);
			rps_unlock(sd);
			local_irq_restore(flags);
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
```
* `enqueue_to_backlog` adds the frame to the backlog queue `softnet_data::sk_buff_head` and put the backlog into the target CPU `NAPI` poll list.

### packet polling from the backlog
* the poll function of the backlog `napi_struct` is `process_backlog`, which will be called when `net_rx_action` is called to handle the softirq.
```c
// net/core/dev.c
static int __init net_dev_init(void)
{
    ...

	for_each_possible_cpu(i) {
		struct softnet_data *sd = &per_cpu(softnet_data, i);

		memset(sd, 0, sizeof(*sd));
		skb_queue_head_init(&sd->input_pkt_queue);
		skb_queue_head_init(&sd->process_queue);
		sd->completion_queue = NULL;
		INIT_LIST_HEAD(&sd->poll_list);
		sd->output_queue = NULL;
		sd->output_queue_tailp = &sd->output_queue;
#ifdef CONFIG_RPS
		sd->csd.func = rps_trigger_softirq;
		sd->csd.info = sd;
		sd->csd.flags = 0;
		sd->cpu = i;
#endif

		sd->backlog.poll = process_backlog;
		sd->backlog.weight = weight_p;
		sd->backlog.gro_list = NULL;
		sd->backlog.gro_count = 0;
	}

    ...
}
```
#### `process_backlog`
* `process_backlog` splices the backlog queue into the process queue, and keeps dequeue packets out of the process queue.
```c
// net/core/dev.c
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

		while ((skb = __skb_dequeue(&sd->process_queue))) {        // dequeue packets out of the process queue
			local_irq_enable();
			__netif_receive_skb(skb);
			local_irq_disable();
			input_queue_head_incr(sd);
			if (++work >= quota) {
				local_irq_enable();
				return work;
			}
		}

		rps_lock(sd);
		qlen = skb_queue_len(&sd->input_pkt_queue);
		if (qlen)
			skb_queue_splice_tail_init(&sd->input_pkt_queue,        // splice the backlog queue into the process queue
						   &sd->process_queue);
        ...
		rps_unlock(sd);
	}
	local_irq_enable();

	return work;
}
```
