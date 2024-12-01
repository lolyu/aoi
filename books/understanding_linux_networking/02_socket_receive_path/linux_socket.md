# Linux socket

```c
int socket(int domain, int type, int protocol);;
```
* `domain`: the protocol family
* `type`: the communication semantics
* `protocol`: the protocol

```c
// include/linux/socket.h
/* Supported address families. */
#define AF_UNSPEC	0
#define AF_UNIX		1	/* Unix domain sockets 		*/
#define AF_LOCAL	1	/* POSIX name for AF_UNIX	*/
#define AF_INET		2	/* Internet IP Protocol 	*/
#define AF_AX25		3	/* Amateur Radio AX.25 		*/
#define AF_IPX		4	/* Novell IPX 			*/
#define AF_APPLETALK	5	/* AppleTalk DDP 		*/
#define AF_NETROM	6	/* Amateur Radio NET/ROM 	*/
#define AF_BRIDGE	7	/* Multiprotocol bridge 	*/
#define AF_ATMPVC	8	/* ATM PVCs			*/
#define AF_X25		9	/* Reserved for X.25 project 	*/
#define AF_INET6	10	/* IP version 6			*/
```

```c
// include/linux/net.h
enum sock_type {
	SOCK_STREAM	= 1,
	SOCK_DGRAM	= 2,
	SOCK_RAW	= 3,
	SOCK_RDM	= 4,
	SOCK_SEQPACKET	= 5,
	SOCK_DCCP	= 6,
	SOCK_PACKET	= 10,
};
```

## socket creation
```c
// net/socket.c
SYSCALL_DEFINE3(socket, int, family, int, type, int, protocol)
{
	int retval;
	struct socket *sock;
	int flags;

    ...

	retval = sock_create(family, type, protocol, &sock);
	if (retval < 0)
		goto out;

	retval = sock_map_fd(sock, flags & (O_CLOEXEC | O_NONBLOCK));            // allocate a file descriptor for this socket
	if (retval < 0)
		goto out_release;

out:
	/* It may be already another descriptor 8) Not kernel problem. */
	return retval;

out_release:
	sock_release(sock);
	return retval;
}

int sock_create(int family, int type, int protocol, struct socket **res)
{
	return __sock_create(current->nsproxy->net_ns, family, type, protocol, res, 0);
}
```
* `__sock_create`
    * allocate the `struct socket` object
    * calls corresponding protocol family create function to initialize this `socket` object.
```c
// net/socket.c
int __sock_create(struct net *net, int family, int type, int protocol,
			 struct socket **res, int kern)
{
	int err;
	struct socket *sock;
	const struct net_proto_family *pf;

    ...

	/*
	 *	Allocate the socket and allow the family to set things up. if
	 *	the protocol is 0, the family is instructed to select an appropriate
	 *	default.
	 */
	sock = sock_alloc();
	if (!sock) {
		net_warn_ratelimited("socket: no more sockets\n");
		return -ENFILE;	/* Not exactly a match, but its the
				   closest posix thing */
	}

	sock->type = type;

    ...

	rcu_read_lock();
	pf = rcu_dereference(net_families[family]);
	err = -EAFNOSUPPORT;
	if (!pf)
		goto out_release;

    ...

	err = pf->create(net, sock, protocol, kern);
	if (err < 0)
		goto out_module_put;

	/*
	 * Now to bump the refcnt of the [loadable] module that owns this
	 * socket at sock_release time we decrement its refcnt.
	 */
	if (!try_module_get(sock->ops->owner))
		goto out_module_busy;

	/*
	 * Now that we're done with the ->create function, the [loadable]
	 * module can have its refcnt decremented
	 */
	module_put(pf->owner);
	err = security_socket_post_create(sock, family, type, protocol, kern);
	if (err)
		goto out_sock_release;
	*res = sock;

	return 0;

    ...
}
```
### `AF_INET` protocol family registration
```c
// net/socket.c
static const struct net_proto_family __rcu *net_families[NPROTO] __read_mostly;
```
* `AF_INET` protocol family initialization
```c
// net/ipv4/af_inet.c
static const struct net_proto_family inet_family_ops = {
	.family = PF_INET,
	.create = inet_create,
	.owner	= THIS_MODULE,
};

static int __init inet_init(void)
{
    ...

	(void)sock_register(&inet_family_ops);

    ...

	for (q = inetsw_array; q < &inetsw_array[INETSW_ARRAY_LEN]; ++q)
		inet_register_protosw(q);
}
```


### `AF_INET` socket creation

#### `inet_create`
```c
// net/ipv4/af_inet.c
static int inet_create(struct net *net, struct socket *sock, int protocol,
		       int kern)
{
	struct sock *sk;
	struct inet_protosw *answer;
	struct inet_sock *inet;
	struct proto *answer_prot;
	unsigned char answer_flags;
	char answer_no_check;
	int try_loading_module = 0;
	int err;

    ...

	sock->state = SS_UNCONNECTED;

	/* Look for the requested type/protocol pair. */
lookup_protocol:
	err = -ESOCKTNOSUPPORT;
	rcu_read_lock();
	list_for_each_entry_rcu(answer, &inetsw[sock->type], list) {

		err = 0;
		/* Check the non-wild match. */
		if (protocol == answer->protocol) {
			if (protocol != IPPROTO_IP)
				break;
		} 
		err = -EPROTONOSUPPORT;
	}

    ...

	sock->ops = answer->ops;
	answer_prot = answer->prot;

    ...
    sk = sk_alloc(net, PF_INET, GFP_KERNEL, answer_prot);
    ...
    sock_init_data(sock, sk);
}

```
* `inetsw` stores the mapping from the socket type to the protocol list

![image](https://github.com/user-attachments/assets/bde67cfd-f543-4df9-8a58-bd5c469ee308)

* `sock_init_data` initialize the `struct sock` object:
	* set the packet ready callback `sk_data_ready` as `sock_def_readable`
	* set the socket read timeout default to `MAX_SCHEDULE_TIMEOUT`, which means blocking wait for data.

![image](https://github.com/user-attachments/assets/6a72edb6-edfd-4ca4-9829-540c790c5f22)



## how does task wait reading from a blocking TCP socket
```
recvfrom ->
sock_recvmsg ->
__sock_recvmsg ->
__sock_recvmsg_nosec
```

### `__sock_recvmsg_nosec`
```c
static inline int __sock_recvmsg_nosec(struct kiocb *iocb, struct socket *sock,
				       struct msghdr *msg, size_t size, int flags)
{
	return sock->ops->recvmsg(iocb, sock, msg, size, flags);		// inet_recvmsg in this case
}
```

### `inet_recvmsg`
```c
int inet_recvmsg(struct kiocb *iocb, struct socket *sock, struct msghdr *msg,
		 size_t size, int flags)
{

	...
	err = sk->sk_prot->recvmsg(iocb, sk, msg, size, flags & MSG_DONTWAIT,
				   flags & ~MSG_DONTWAIT, &addr_len);
	return err;
}
```

### `tcp_recvmsg`
```c
int tcp_recvmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg,
		size_t len, int nonblock, int flags, int *addr_len)
{
	int copied = 0;
	int target;		/* Read at least this many bytes */

	timeo = sock_rcvtimeo(sk, nonblock);					// noblock ? 0 : sk->sk_rcvtimeo;
	...
	target = sock_rcvlowat(sk, flags & MSG_WAITALL, len);

	...
	do {
		skb_queue_walk(&sk->sk_receive_queue, skb) {
			...
		}

		...

		if (copied >= target) {
			/* Do not sleep, just process backlog. */
			release_sock(sk);
			lock_sock(sk);
		} else
			sk_wait_data(sk, &timeo);
		...
	}
	...
}
```
* if no data/not enough data, call `sk_wait_data` to put current process into the wait queue and yield the CPU.

```c
// net/core/sock.c
int sk_wait_data(struct sock *sk, long *timeo)
{
	int rc;
	DEFINE_WAIT(wait);																// use current task to define a wait object, use autoremove_wake_function as wake callback

	prepare_to_wait(sk_sleep(sk), &wait, TASK_INTERRUPTIBLE);						// put current task into the sock wait queue, and put current task into TASK_INTERRUPTABLE
	set_bit(SOCK_ASYNC_WAITDATA, &sk->sk_socket->flags);
	rc = sk_wait_event(sk, timeo, !skb_queue_empty(&sk->sk_receive_queue));			// if current receive queue is empty, reschedule for timeout jiffies
	clear_bit(SOCK_ASYNC_WAITDATA, &sk->sk_socket->flags);
	finish_wait(sk_sleep(sk), &wait);												// remove current task from the sock wait queue, and put current task into TASK_RUNNING
	return rc;
}

// include/linux/wait.h
#define DEFINE_WAIT_FUNC(name, function)				\
	wait_queue_t name = {						\
		.private	= current,				\
		.func		= function,				\
		.task_list	= LIST_HEAD_INIT((name).task_list),	\
	}

#define DEFINE_WAIT(name) DEFINE_WAIT_FUNC(name, autoremove_wake_function)
```

## how does softirq wakes thread blocking waiting for reading from socket?
```
tcp_v4_rcv ->
tcp_v4_do_rcv ->
tcp_rcv_established
```
### `tcp_rcv_established`
```c
int tcp_rcv_established(struct sock *sk, struct sk_buff *skb,
			const struct tcphdr *th, unsigned int len)
{
	...
				eaten = tcp_queue_rcv(sk, skb, tcp_header_len,
						      &fragstolen);
			}
			...
			sk->sk_data_ready(sk, 0);
			return 0;
		}
	}
	...
}

static int __must_check tcp_queue_rcv(struct sock *sk, struct sk_buff *skb, int hdrlen,
		  bool *fragstolen)
{
	int eaten;
	...
	if (!eaten) {
		__skb_queue_tail(&sk->sk_receive_queue, skb);
		skb_set_owner_r(skb, sk);
	}
	return eaten;
}
```
* `sk_data_ready` is `sock_def_readable`
	* `sock_def_readable` wakes up one thread blocked on the sock wait queue
 		* the sync wakeup will yield this CPU to the thread that is blocked.
```c
// net/core/sock.c
static void sock_def_readable(struct sock *sk, int len)
{
	struct socket_wq *wq;

	rcu_read_lock();
	wq = rcu_dereference(sk->sk_wq);
	if (wq_has_sleeper(wq))													// if sock wait queue is not empty
		wake_up_interruptible_sync_poll(&wq->wait, POLLIN | POLLPRI |		// wakes up one thread blocked on this socket
						POLLRDNORM | POLLRDBAND);
	sk_wake_async(sk, SOCK_WAKE_WAITD, POLL_IN);
	rcu_read_unlock();
}
```
```c
// include/linux/wait.h
#define wake_up_interruptible_sync_poll(x, m)				\
	__wake_up_sync_key((x), TASK_INTERRUPTIBLE, 1, (void *) (m))
// kernel/sched/core.c
void __wake_up_sync_key(wait_queue_head_t *q, unsigned int mode,
			int nr_exclusive, void *key)
{
	unsigned long flags;
	int wake_flags = WF_SYNC;

	if (unlikely(!q))
		return;

	if (unlikely(!nr_exclusive))
		wake_flags = 0;

	spin_lock_irqsave(&q->lock, flags);
	__wake_up_common(q, mode, nr_exclusive, wake_flags, key);
	spin_unlock_irqrestore(&q->lock, flags);
}

// kernel/sched/core.c
static void __wake_up_common(wait_queue_head_t *q, unsigned int mode,
			int nr_exclusive, int wake_flags, void *key)
{
	wait_queue_t *curr, *next;

	list_for_each_entry_safe(curr, next, &q->task_list, task_list) {
		unsigned flags = curr->flags;

		if (curr->func(curr, mode, wake_flags, key) &&
				(flags & WQ_FLAG_EXCLUSIVE) && !--nr_exclusive)
			break;
	}
}
```

#### `autoremove_wake_function`
```c
// kernel/wait.c
int autoremove_wake_function(wait_queue_t *wait, unsigned mode, int sync, void *key)
{
	int ret = default_wake_function(wait, mode, sync, key);

	if (ret)
		list_del_init(&wait->task_list);
	return ret;
}

// kernel/sched/core.c
int default_wake_function(wait_queue_t *curr, unsigned mode, int wake_flags,
			  void *key)
{
	return try_to_wake_up(curr->private, mode, wake_flags);
}
```

## summary
* when a thread tries to read from a blocking socket, it will check the socket receive queue first, if there is no packet, it will yield the CPU.
* after the hard interrupt handler/softirqd puts the packets into the socket receive queue, it will wakes up one thread that is blocked reading from the socket.

* question: how many interrupts?
	* two, one happens from current blocking thread yields CPU, and one happens when the interrupt handler yields CPU to the blocing thread.


## reference
* https://man7.org/linux/man-pages/man2/socket.2.html
