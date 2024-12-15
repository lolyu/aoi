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

wait_for_sndbuf:
			set_bit(SOCK_NOSPACE, &sk->sk_socket->flags);
wait_for_memory:
			if (copied)
				tcp_push(sk, flags & ~MSG_MORE, mss_now, TCP_NAGLE_PUSH);

			if ((err = sk_stream_wait_memory(sk, &timeo)) != 0)
				goto do_error;

			mss_now = tcp_send_mss(sk, &size_goal, flags);
		}
	}

out:
	if (copied)
		tcp_push(sk, flags, mss_now, tp->nonagle);
	release_sock(sk);
	return copied + copied_syn;

do_fault:
	if (!skb->len) {
		tcp_unlink_write_queue(skb, sk);
		/* It is the one place in all of TCP, except connection
		 * reset, where we can be unlinking the send_head.
		 */
		tcp_check_send_head(sk, skb);
		sk_wmem_free_skb(sk, skb);
	}

do_error:
	if (copied + copied_syn)
		goto out;
out_err:
	err = sk_stream_error(sk, flags, err);
	release_sock(sk);
	return err;
}
```
