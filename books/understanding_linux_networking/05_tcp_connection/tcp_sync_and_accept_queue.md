# TCP syn and accept queue
* this note focuses on what's the function of `listen` and the TCP syn and accept queue.
    * **sync queue**: stores connection requests (one connection request per `SYN` packet.)
		* TCP connections in `SYN-RCVD`
    * **accept queue**: stores fully established connections (but haven't been `accept()`ed by the application code)
		* TCP connections in `ESTABLISHED` but not accepted.

![image](https://github.com/user-attachments/assets/c837a62a-1800-4b04-bacc-a68582f1882d)

* the **syn queue** length is `min(backlog, somaxconn, tcp_max_syn_backlog) + 1`
* the **accept queue** length is `min(backlog, somaxconn)`


## listen
```c
SYSCALL_DEFINE2(listen, int, fd, int, backlog)
{
	struct socket *sock;
	int err, fput_needed;
	int somaxconn;

	sock = sockfd_lookup_light(fd, &err, &fput_needed);
	if (sock) {
		somaxconn = sock_net(sock->sk)->core.sysctl_somaxconn;
		if ((unsigned int)backlog > somaxconn)
			backlog = somaxconn;
        ...
        err = sock->ops->listen(sock, backlog);
	}
	return err;
}
```
```c
int inet_listen(struct socket *sock, int backlog)
{
	struct sock *sk = sock->sk;
	unsigned char old_state;
	int err;

	lock_sock(sk);

	err = -EINVAL;
    ...

	/* Really, if the socket is already in listen state
	 * we can only allow the backlog to be adjusted.
	 */
	if (old_state != TCP_LISTEN) {
        ...
		err = inet_csk_listen_start(sk, backlog);
        ...
	}
	sk->sk_max_ack_backlog = backlog;			// the length of accept queue
	err = 0;

    ...
	return err;
}
```

* `sk_max_ack_backlog` is the length of accept queue, is `min(backlog, somaxconn)`

## allocate queue
```c
int inet_csk_listen_start(struct sock *sk, const int nr_table_entries)
{
	struct inet_sock *inet = inet_sk(sk);
	struct inet_connection_sock *icsk = inet_csk(sk);
	int rc = reqsk_queue_alloc(&icsk->icsk_accept_queue, nr_table_entries);

	if (rc != 0)
		return rc;

	sk->sk_max_ack_backlog = 0;
	sk->sk_ack_backlog = 0;
	inet_csk_delack_init(sk);

	/* There is race window here: we announce ourselves listening,
	 * but this transition is still not validated by get_port().
	 * It is OK, because this socket enters to hash table only
	 * after validation is complete.
	 */
	sk->sk_state = TCP_LISTEN;
	if (!sk->sk_prot->get_port(sk, inet->inet_num)) {
		inet->inet_sport = htons(inet->inet_num);

		sk_dst_reset(sk);
		sk->sk_prot->hash(sk);

		return 0;
	}

	sk->sk_state = TCP_CLOSE;
	__reqsk_queue_destroy(&icsk->icsk_accept_queue);
	return -EADDRINUSE;
}
```
* the relationship between `sock_common`, `sock`, `inet_sock`, `inet_connection_sock`, and `tcp_sock`:

![image](https://github.com/user-attachments/assets/b7bdc628-5d36-43c4-b18f-8f8cf9307263)

```c
// include/net/inet_connection_sock.h
struct inet_connection_sock {
	/* inet_sock has to be the first member! */
	struct inet_sock	  icsk_inet;
	struct request_sock_queue icsk_accept_queue;
    ...
};

// include/net/request_sock.h
struct request_sock_queue {
	struct request_sock	*rskq_accept_head;
	struct request_sock	*rskq_accept_tail;
    ...
	struct listen_sock	*listen_opt;
    ...
};

struct listen_sock {
	u8			max_qlen_log;
	u8			synflood_warned;
	/* 2 bytes hole, try to use */
	int			qlen;
	int			qlen_young;
	int			clock_hand;
	u32			hash_rnd;
	u32			nr_table_entries;
	struct request_sock	*syn_table[0];
};
```

```c
int reqsk_queue_alloc(struct request_sock_queue *queue,
		      unsigned int nr_table_entries)
{
	size_t lopt_size = sizeof(struct listen_sock);
	struct listen_sock *lopt;

	nr_table_entries = min_t(u32, nr_table_entries, sysctl_max_syn_backlog);
	nr_table_entries = max_t(u32, nr_table_entries, 8);
	nr_table_entries = roundup_pow_of_two(nr_table_entries + 1);
	lopt_size += nr_table_entries * sizeof(struct request_sock *);
	if (lopt_size > PAGE_SIZE)
		lopt = vzalloc(lopt_size);
	else
		lopt = kzalloc(lopt_size, GFP_KERNEL);
	if (lopt == NULL)
		return -ENOMEM;

	for (lopt->max_qlen_log = 3;
	     (1 << lopt->max_qlen_log) < nr_table_entries;
	     lopt->max_qlen_log++);

	get_random_bytes(&lopt->hash_rnd, sizeof(lopt->hash_rnd));
	rwlock_init(&queue->syn_wait_lock);
	queue->rskq_accept_head = NULL;
	lopt->nr_table_entries = nr_table_entries;

	write_lock_bh(&queue->syn_wait_lock);
	queue->listen_opt = lopt;
	write_unlock_bh(&queue->syn_wait_lock);

	return 0;
}
```

## questions
### how to check the sync queue size?
* there is no direct system metrics, but the number of `SYN_RCVD` state sockets can be get by `netstat`
```
# netstat -antp | grep SYN_RECV | wc -l
102
```

### how to check the accept queue size?
* the accept queue size is listed as `Send-Q` for the `LISTEN` sockets.
```
ss -lnt                                                                                                       255 ↵
State        Recv-Q        Send-Q                Local Address:Port               Peer Address:Port       Process
LISTEN       0             4096                     127.0.0.54:53                      0.0.0.0:*
LISTEN       0             1000                 10.255.255.254:53                      0.0.0.0:*
LISTEN       0             4096                  127.0.0.53%lo:53                      0.0.0.0:*
```

### what is listen drop and listen overflow?
* `listen overflow` means Linux cannot take more requests due to the accept queue is full.
* `listen drop` means Linux cannot take more requests; this can happen for many reasons, like cannot allocate memory for new requests, accept queue is full, etc.
* `listen drop` and `listen overflow` can be checked via `netstat`:
```
# netstat -s | grep -i listen
    545772 times the listen queue of a socket overflowed		# listen overflow
    545776 SYNs to LISTEN sockets dropped						# listen drop
```
## references
* https://arthurchiao.art/blog/tcp-listen-a-tale-of-two-queues/
* https://www.alibabacloud.com/blog/tcp-syn-queue-and-accept-queue-overflow-explained_599203

