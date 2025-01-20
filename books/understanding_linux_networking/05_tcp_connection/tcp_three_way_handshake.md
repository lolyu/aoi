# tcp three way handshake

![image](https://github.com/user-attachments/assets/e39d8d6e-3614-4183-8293-c610d134e50c)


## client connect
* `tcp_connect`
    * set the socket state to `SYN_SENT`
    * select a source port
    * build a SYN packet and enqueue/send
    * start retran timer

```c
int tcp_connect(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct sk_buff *buff;
	int err;

	tcp_connect_init(sk);

    ...
	buff = alloc_skb_fclone(MAX_TCP_HEADER + 15, sk->sk_allocation);
    ...
	tcp_connect_queue_skb(sk, buff);
	TCP_ECN_send_syn(sk, buff);

	/* Send off SYN; include data in Fast Open. */
	err = tp->fastopen_req ? tcp_send_syn_data(sk, buff) :
	      tcp_transmit_skb(sk, buff, 1, sk->sk_allocation);
	if (err == -ECONNREFUSED)
		return err;

    ..
	inet_csk_reset_xmit_timer(sk, ICSK_TIME_RETRANS,
				  inet_csk(sk)->icsk_rto, TCP_RTO_MAX);
	return 0;
}
```

## server responds and sends `SYNACK`
* `tcp_v4_do_rcv` -> `tcp_rcv_state_process` -> `tcp_v4_conn_request`
*  `tcp_v4_conn_request`
    * if the listen queue is full, drop
    * if the accept queue is full and there is `young ack`, drop
    * send `SYNACK`
    * create request sock and add to the listen queue
* `young ack` means the connection that `SYN` is received and there is no retran for the `SYNACK`.

```c
int tcp_v4_conn_request(struct sock *sk, struct sk_buff *skb)
{


	if (inet_csk_reqsk_queue_is_full(sk) && !isn) {
		want_cookie = tcp_syn_flood_action(sk, skb, "TCP");
		if (!want_cookie)
			goto drop;
	}

	/* Accept backlog is full. If we have already queued enough
	 * of warm entries in syn queue, drop request. It is better than
	 * clogging syn queue with openreqs with exponentially increasing
	 * timeout.
	 */
	if (sk_acceptq_is_full(sk) && inet_csk_reqsk_queue_young(sk) > 1) {
		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENOVERFLOWS);
		goto drop;
	}

	req = inet_reqsk_alloc(&tcp_request_sock_ops);

    ...
	skb_synack = tcp_make_synack(sk, dst, req,
	    fastopen_cookie_present(&valid_foc) ? &valid_foc : NULL);
    ...

	if (likely(!do_fastopen)) {
		int err;
		err = ip_build_and_send_pkt(skb_synack, sk, ireq->loc_addr,
		     ireq->rmt_addr, ireq->opt);
        ...
	} else if (tcp_v4_conn_req_fastopen(sk, skb, skb_synack, req))
		goto drop_and_free;
    ...
}
```

## client responds to `SYNACK`
* `tcp_v4_do_rcv` -> `tcp_rcv_state_process` -> `tcp_rcv_synsent_state_process`

```c
static int tcp_rcv_synsent_state_process(struct sock *sk, struct sk_buff *skb,
					 const struct tcphdr *th, unsigned int len)
{
    ...
	if (th->ack) {
		if (!th->syn)                                                        // both the SYN and ACK are set
			goto discard_and_undo;

        ...
		tcp_ack(sk, skb, FLAG_SLOWPATH);                                    // cleanup the send queue (the SYN packet)

        ...

		tcp_finish_connect(sk, skb);

		if ((tp->syn_fastopen || tp->syn_data) &&
		    tcp_rcv_fastopen_synack(sk, skb, &foc))
			return -1;

		if (sk->sk_write_pending ||
		    icsk->icsk_accept_queue.rskq_defer_accept ||
		    icsk->icsk_ack.pingpong) {
            ...
		} else {
			tcp_send_ack(sk);
		}
		return -1;
	}

    ...
}

```
