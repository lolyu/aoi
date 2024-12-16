# kernel send path
* **NOTE**: this note focuses on the tcp socket kernel send path.
```
sendto -> sock_sendmsg -> __sock_sendmsg -> __sock_sendmsg_nosec -> inet_sendmsg -> tcp_sendmsg
```

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

```
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


## references
* http://oldvger.kernel.org/~davem/tcp_output.html
* http://oldvger.kernel.org/~davem/skb_data.html
