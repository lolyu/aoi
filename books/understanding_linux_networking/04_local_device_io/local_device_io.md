# local device I/O
* how does Linux handles the packet forward/receive on the local machine?

## `ip_queue_xmit`
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

```c

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
