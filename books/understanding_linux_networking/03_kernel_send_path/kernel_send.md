# kernel send path
* **NOTE**: this note focuses on the tcp socket kernel send path.
```
sendto -> sock_sendmsg -> __sock_sendmsg -> __sock_sendmsg_nosec -> inet_sendmsg -> tcp_sendmsg
```
![image](https://github.com/user-attachments/assets/5ab3b7f6-8096-4635-a0de-ac4a4ae91bd8)


## `tcp_sendmsg`

```c
int tcp_sendmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg,
		size_t size)
{
	struct iovec *iov;
	struct tcp_sock *tp = tcp_sk(sk);
	struct sk_buff *skb;
	int iovlen, flags, err, copied = 0;
	int mss_now = 0, size_goal, copied_syn = 0, offset = 0;

	lock_sock(sk);



	/* This should be in poll */
	clear_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags);

	mss_now = tcp_send_mss(sk, &size_goal, flags);

	/* Ok commence sending. */
	iovlen = msg->msg_iovlen;
	iov = msg->msg_iov;
	copied = 0;

  ...

	while (--iovlen >= 0) {
		size_t seglen = iov->iov_len;
		unsigned char __user *from = iov->iov_base;

		iov++;

		while (seglen > 0) {
			int copy = 0;
			int max = size_goal;

			skb = tcp_write_queue_tail(sk);                            // check if last skb in the write queue has room to write
			if (tcp_send_head(sk)) {
				if (skb->ip_summed == CHECKSUM_NONE)
					max = mss_now;
				copy = max - skb->len;
			}

			if (copy <= 0) {                                            // write queue last skb doesn't have room to write, allocate new skb
new_segment:
				/* Allocate new segment. If the interface is SG,
				 * allocate skb fitting to single page.
				 */
                ...
				skb = sk_stream_alloc_skb(sk,
							  select_size(sk, sg),
							  sk->sk_allocation);
                ...
				skb_entail(sk, skb);                                    // append the new allocated skb to the write queue
				copy = size_goal;
				max = size_goal;
			}

			/* Try to append data to the end of skb. */
			if (copy > seglen)
				copy = seglen;

			/* Where to copy to? */
			if (skb_availroom(skb) > 0) {
				/* We have some space in skb head. Superb! */
				copy = min_t(int, copy, skb_availroom(skb));
				err = skb_add_data_nocache(sk, skb, from, copy);
				if (err)
					goto do_fault;
			}

            ...

			if (forced_push(tp)) {
				tcp_mark_push(tp, skb);
				__tcp_push_pending_frames(sk, mss_now, TCP_NAGLE_PUSH);
			} else if (skb == tcp_send_head(sk))
				tcp_push_one(sk, mss_now);
			continue;
			...
}
```

* At the top-level, `tcp_sendmsg` gather up data (either from userspace or the page cache) into SKB packets and tack them onto the `sk_write_queue` of the TCP socket. At appropriate times they invoke either `tcp_write_xmit()` or `tcp_push_one()` to try and output those data frames.

###

```c
static inline int skb_add_data_nocache(struct sock *sk, struct sk_buff *skb,
				       char __user *from, int copy)
{
	int err, offset = skb->len;

	err = skb_do_copy_data_nocache(sk, skb, from, skb_put(skb, copy),
				       copy, offset);
	if (err)
		__skb_trim(skb, offset);

	return err;
}
```
* `skb_put` will try to find the next user data insert position in `sk_buff`

```c
/**
 *	skb_put - add data to a buffer
 *	@skb: buffer to use
 *	@len: amount of data to add
 *
 *	This function extends the used data area of the buffer. If this would
 *	exceed the total buffer size the kernel will panic. A pointer to the
 *	first byte of the extra data is returned.
 */
unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	unsigned char *tmp = skb_tail_pointer(skb);
	SKB_LINEAR_ASSERT(skb);
	skb->tail += len;
	skb->len  += len;
	return tmp;
}
EXPORT_SYMBOL(skb_put);

/**
 *	skb_push - add data to the start of a buffer
 *	@skb: buffer to use
 *	@len: amount of data to add
 *
 *	This function extends the used data area of the buffer at the buffer
 *	start. If this would exceed the total buffer headroom the kernel will
 *	panic. A pointer to the first byte of the extra data is returned.
 */
unsigned char *skb_push(struct sk_buff *skb, unsigned int len)
{
	skb->data -= len;
	skb->len  += len;
	return skb->data;
}
```

* four pointers in `sk_buff`:
	* `head`: the buffer start
 	* `end`: the buffer end
	* `data`: the user data start
 	* `tail`: the user data end

* a new `sk_buff` buffer layout:
	* reserve enough bytes for the headers
 	* to write packet headers, decrease `data`
	* to write packet contents, increase `tail`
 
![image](https://github.com/user-attachments/assets/9b7bda4b-6540-4729-9e9a-37145d431d15)

* a IP packet buffer layout:

![image](https://github.com/user-attachments/assets/18a16e61-347f-406d-94a2-a660acd8e3c8)

## `dev_queue_xmit`

```c
int dev_queue_xmit(struct sk_buff *skb)
{
	struct net_device *dev = skb->dev;
	struct netdev_queue *txq;
	struct Qdisc *q;

	...

	txq = netdev_pick_tx(dev, skb);					// get the tx queue based on the XPS config
	q = rcu_dereference_bh(txq->qdisc);				// get the qdisc of the tx queue, the default is pfifo_fast

	trace_net_dev_queue(skb);
	if (q->enqueue) {
		rc = __dev_xmit_skb(skb, q, dev, txq);
		goto out;
	}

	...	
}
```
```c
// include/net/sch_generic.h
struct Qdisc_ops {
	struct Qdisc_ops	*next;
	const struct Qdisc_class_ops	*cl_ops;
	char			id[IFNAMSIZ];
	int			priv_size;

	int 			(*enqueue)(struct sk_buff *, struct Qdisc *);
	struct sk_buff *	(*dequeue)(struct Qdisc *);
	struct sk_buff *	(*peek)(struct Qdisc *);
	unsigned int		(*drop)(struct Qdisc *);

	int			(*init)(struct Qdisc *, struct nlattr *arg);
	void			(*reset)(struct Qdisc *);
	void			(*destroy)(struct Qdisc *);
	int			(*change)(struct Qdisc *, struct nlattr *arg);
	void			(*attach)(struct Qdisc *);

	int			(*dump)(struct Qdisc *, struct sk_buff *);
	int			(*dump_stats)(struct Qdisc *, struct gnet_dump *);

	struct module		*owner;
};

struct Qdisc {
	int 			(*enqueue)(struct sk_buff *skb, struct Qdisc *dev);
	struct sk_buff *	(*dequeue)(struct Qdisc *dev);
	unsigned int		flags;
#define TCQ_F_BUILTIN		1
#define TCQ_F_INGRESS		2
#define TCQ_F_CAN_BYPASS	4
#define TCQ_F_MQROOT		8
#define TCQ_F_ONETXQUEUE	0x10 /* dequeue_skb() can assume all skbs are for
				      * q->dev_queue : It can test
				      * netif_xmit_frozen_or_stopped() before
				      * dequeueing next packet.
				      * Its true for MQ/MQPRIO slaves, or non
				      * multiqueue device.
				      */
#define TCQ_F_WARN_NONWC	(1 << 16)
	int			padded;
	const struct Qdisc_ops	*ops;
	struct qdisc_size_table	__rcu *stab;
	struct list_head	list;
	u32			handle;
	u32			parent;
	atomic_t		refcnt;
	struct gnet_stats_rate_est	rate_est;
	int			(*reshape_fail)(struct sk_buff *skb,
					struct Qdisc *q);

	void			*u32_node;

	/* This field is deprecated, but it is still used by CBQ
	 * and it will live until better solution will be invented.
	 */
	struct Qdisc		*__parent;
	struct netdev_queue	*dev_queue;
	struct Qdisc		*next_sched;

	struct sk_buff		*gso_skb;
	/*
	 * For performance sake on SMP, we put highly modified fields at the end
	 */
	unsigned long		state;
	struct sk_buff_head	q;
	struct gnet_stats_basic_packed bstats;
	unsigned int		__state;
	struct gnet_stats_queue	qstats;
	struct rcu_head		rcu_head;
	spinlock_t		busylock;
	u32			limit;
};

// net/sched/sch_mq.c
struct Qdisc_ops mq_qdisc_ops __read_mostly = {						// mq has no queue, so no "enqueue"
	.cl_ops		= &mq_class_ops,
	.id		= "mq",
	.priv_size	= sizeof(struct mq_sched),
	.init		= mq_init,
	.destroy	= mq_destroy,
	.attach		= mq_attach,
	.dump		= mq_dump,
	.owner		= THIS_MODULE,
};
```
* `tc qdisc` shows the device output queue disciplines:
```
$ sudo tc qdisc
qdisc noqueue 0: dev lo root refcnt 2
qdisc mq 0: dev eth0 root
qdisc fq_codel 0: dev eth0 parent :1 limit 10240p flows 1024 quantum 1514 target 5ms interval 100ms memory_limit 32Mb ecn drop_batch 64
qdisc noqueue 0: dev docker0 root refcnt 2
qdisc noqueue 0: dev PortChannel101 root refcnt 2
qdisc noqueue 0: dev PortChannel102 root refcnt 2
qdisc noqueue 0: dev PortChannel103 root refcnt 2
qdisc noqueue 0: dev PortChannel104 root refcnt 2
qdisc noqueue 0: dev Bridge root refcnt 2
qdisc noqueue 0: dev Vlan1000 root refcnt 2
qdisc noqueue 0: dev Loopback0 root refcnt 2
qdisc noqueue 0: dev Loopback1 root refcnt 2
qdisc noqueue 0: dev Loopback2 root refcnt 2
qdisc noqueue 0: dev Loopback3 root refcnt 2
qdisc noqueue 0: dev tun0 root refcnt 2
qdisc fq_codel 0: dev Ethernet4 root refcnt 2 limit 10240p flows 1024 quantum 9114 target 5ms interval 100ms memory_limit 32Mb ecn drop_batch 64
qdisc fq_codel 0: dev Ethernet8 root refcnt 2 limit 10240p flows 1024 quantum 9114 target 5ms interval 100ms memory_limit 32Mb ecn drop_batch 64
qdisc fq_codel 0: dev Ethernet12 root refcnt 2 limit 10240p flows 1024 quantum 9114 target 5ms interval 100ms memory_limit 32Mb ecn drop_batch 64

$ sudo tc qdisc
qdisc noqueue 0: dev lo root refcnt 2
qdisc mq 0: dev eth0 root
qdisc pfifo_fast 0: dev eth0 parent :8 bands 3 priomap 1 2 2 2 1 2 0 0 1 1 1 1 1 1 1 1
qdisc pfifo_fast 0: dev eth0 parent :7 bands 3 priomap 1 2 2 2 1 2 0 0 1 1 1 1 1 1 1 1
qdisc pfifo_fast 0: dev eth0 parent :6 bands 3 priomap 1 2 2 2 1 2 0 0 1 1 1 1 1 1 1 1
qdisc pfifo_fast 0: dev eth0 parent :5 bands 3 priomap 1 2 2 2 1 2 0 0 1 1 1 1 1 1 1 1
qdisc pfifo_fast 0: dev eth0 parent :4 bands 3 priomap 1 2 2 2 1 2 0 0 1 1 1 1 1 1 1 1
qdisc pfifo_fast 0: dev eth0 parent :3 bands 3 priomap 1 2 2 2 1 2 0 0 1 1 1 1 1 1 1 1
qdisc pfifo_fast 0: dev eth0 parent :2 bands 3 priomap 1 2 2 2 1 2 0 0 1 1 1 1 1 1 1 1
qdisc pfifo_fast 0: dev eth0 parent :1 bands 3 priomap 1 2 2 2 1 2 0 0 1 1 1 1 1 1 1 1
```

## `__dev_xmit_skb`
```c
static inline int __dev_xmit_skb(struct sk_buff *skb, struct Qdisc *q,
				 struct net_device *dev,
				 struct netdev_queue *txq)
{
	spinlock_t *root_lock = qdisc_lock(q);
	bool contended;
	int rc;

	...
	if (unlikely(test_bit(__QDISC_STATE_DEACTIVATED, &q->state))) {
		kfree_skb(skb);
		rc = NET_XMIT_DROP;
	} else if ((q->flags & TCQ_F_CAN_BYPASS) && !qdisc_qlen(q) &&
		   qdisc_run_begin(q)) {
		/*
		 * This is a work-conserving queue; there are no old skbs
		 * waiting to be sent out; and the qdisc is not running -
		 * xmit the skb directly.
		 */
		if (!(dev->priv_flags & IFF_XMIT_DST_RELEASE))
			skb_dst_force(skb);

		qdisc_bstats_update(q, skb);

		if (sch_direct_xmit(skb, q, dev, txq, root_lock)) {
			if (unlikely(contended)) {
				spin_unlock(&q->busylock);
				contended = false;
			}
			__qdisc_run(q);
		} else
			qdisc_run_end(q);

		rc = NET_XMIT_SUCCESS;
	} else {
		skb_dst_force(skb);
		rc = q->enqueue(skb, q) & NET_XMIT_MASK;
		if (qdisc_run_begin(q)) {
			if (unlikely(contended)) {
				spin_unlock(&q->busylock);
				contended = false;
			}
			__qdisc_run(q);
		}
	}
	spin_unlock(root_lock);
	if (unlikely(contended))
		spin_unlock(&q->busylock);
	return rc;
}
```
* `__qdisc_run` continues sending queued packets until:
	1. runs out of quota
 	2. needs reschedule 	
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

static inline int qdisc_restart(struct Qdisc *q)
{
	struct netdev_queue *txq;
	struct net_device *dev;
	spinlock_t *root_lock;
	struct sk_buff *skb;

	/* Dequeue packet */
	skb = dequeue_skb(q);
	if (unlikely(!skb))
		return 0;
	WARN_ON_ONCE(skb_dst_is_noref(skb));
	root_lock = qdisc_lock(q);
	dev = qdisc_dev(q);
	txq = netdev_get_tx_queue(dev, skb_get_queue_mapping(skb));

	return sch_direct_xmit(skb, q, dev, txq, root_lock);
}
```

## raise tx softirq
* **NOTE**: `output_queue_tailp` is a double pointer that stores to the address of next output queue
```c
static inline void __netif_reschedule(struct Qdisc *q)
{
	struct softnet_data *sd;
	unsigned long flags;

	local_irq_save(flags);
	sd = &__get_cpu_var(softnet_data);
	q->next_sched = NULL;
	*sd->output_queue_tailp = q;						// append `q` to the tail of output queue
	sd->output_queue_tailp = &q->next_sched;			// reset output_queue_tailp to the tail of output queue, which is the next_sched of the appended `q`
	raise_softirq_irqoff(NET_TX_SOFTIRQ);
	local_irq_restore(flags);
}

void __netif_schedule(struct Qdisc *q)
{
	if (!test_and_set_bit(__QDISC_STATE_SCHED, &q->state))
		__netif_reschedule(q);
}
```

```c
struct softnet_data {
	struct Qdisc		*output_queue;
	struct Qdisc		**output_queue_tailp;
	struct list_head	poll_list;
	...
};
```

### `net_tx_action`
* `net_tx_action` retrieves output queue from per-cpu `softnet_data`, and send the packets stored in each queue.
```c
static void net_tx_action(struct softirq_action *h)
{
	struct softnet_data *sd = &__get_cpu_var(softnet_data);

	...

	if (sd->output_queue) {
		struct Qdisc *head;

		head = sd->output_queue;
		sd->output_queue = NULL;
		sd->output_queue_tailp = &sd->output_queue;

		while (head) {
			struct Qdisc *q = head;
			head = head->next_sched;
			qdisc_run(q);
		}
	}
}
```

## tx buffer cleanup
* when the NIC finishes sending packets, it will trigger a hardware IRQ.
	* the IRQ handler is `igb_msix_ring` for both rx and tx ring

```c
static irqreturn_t igb_msix_ring(int irq, void *data)
{
	struct igb_q_vector *q_vector = data;

	/* Write the ITR value calculated from the previous interrupt. */
	igb_write_itr(q_vector);

	napi_schedule(&q_vector->napi);

	return IRQ_HANDLED;
}

static int igb_poll(struct napi_struct *napi, int budget)
{
	struct igb_q_vector *q_vector = container_of(napi,
						     struct igb_q_vector,
						     napi);
	bool clean_complete = true;
	...
	if (q_vector->tx.ring)
		clean_complete = igb_clean_tx_irq(q_vector);		// cleanup the tx buffer

	...

	return 0;
}
```

## references
* http://oldvger.kernel.org/~davem/tcp_output.html
* http://oldvger.kernel.org/~davem/skb_data.html
