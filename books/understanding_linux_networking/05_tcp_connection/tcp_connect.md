# tcp connect
```c
int main()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    connect(fd, ...);
    ...
}
```

* `connect()` -> `inet_stream_connect()` -> `__inet_stream_connect()` -> `tcp_v4_connect`

## `tcp_v4_connect`
```c
int tcp_v4_connect(struct sock *sk, struct sockaddr *uaddr, int addr_len)
{
    ...

	/* Socket identity is still unknown (sport may be zero).
	 * However we set state to SYN-SENT and not releasing socket
	 * lock select source port, enter ourselves into the hash tables and
	 * complete initialization after this.
	 */
	tcp_set_state(sk, TCP_SYN_SENT);                            // change TCP connection to SYN_SENT
	err = inet_hash_connect(&tcp_death_row, sk);                // select a port
    ...
	err = tcp_connect(sk);                                        // send a SYN packet
    ...
}
```

### `tcp_death_row`
```c
struct inet_timewait_death_row tcp_death_row = {
	.sysctl_max_tw_buckets = NR_FILE * 2,
	.period		= TCP_TIMEWAIT_LEN / INET_TWDR_TWKILL_SLOTS,
	.death_lock	= __SPIN_LOCK_UNLOCKED(tcp_death_row.death_lock),
	.hashinfo	= &tcp_hashinfo,
	.tw_timer	= TIMER_INITIALIZER(inet_twdr_hangman, 0,
					    (unsigned long)&tcp_death_row),
	.twkill_work	= __WORK_INITIALIZER(tcp_death_row.twkill_work,
					     inet_twdr_twkill_work),
/* Short-time timewait calendar */

	.twcal_hand	= -1,
	.twcal_timer	= TIMER_INITIALIZER(inet_twdr_twcal_tick, 0,
					    (unsigned long)&tcp_death_row),
};

struct inet_hashinfo tcp_hashinfo;

struct inet_hashinfo {
	struct inet_ehash_bucket	*ehash;
	spinlock_t			*ehash_locks;
	unsigned int			ehash_mask;
	unsigned int			ehash_locks_mask;

	struct inet_bind_hashbucket	*bhash;

	unsigned int			bhash_size;

	struct inet_listen_hashbucket	listening_hash[INET_LHTABLE_SIZE]
					____cacheline_aligned_in_smp;

	atomic_t			bsockets;
};

```
* `tcp_hashinfo` stores all tcp connections:
    * `ehash`: stores `ESTABLISHED` connections, key is `(local addr, local port, peer addr, peer port)`
    * `bhash`: stores local binding connections, key is `local port`.
    * `listening_hash`: stores `LISTEN` connections, key is `local port`

![image](https://github.com/user-attachments/assets/a1b615a0-7b90-4925-800d-a1b51f269f8b)

### `inet_hash_connect`
```
int inet_hash_connect(struct inet_timewait_death_row *death_row,
		      struct sock *sk)
{
	return __inet_hash_connect(death_row, sk, inet_sk_port_offset(sk),
			__inet_check_established, __inet_hash_nolisten);
}

int __inet_hash_connect(struct inet_timewait_death_row *death_row,
		struct sock *sk, u32 port_offset,
		int (*check_established)(struct inet_timewait_death_row *,
			struct sock *, __u16, struct inet_timewait_sock **),
		int (*hash)(struct sock *sk, struct inet_timewait_sock *twp))
{
	struct inet_hashinfo *hinfo = death_row->hashinfo;
	const unsigned short snum = inet_sk(sk)->inet_num;
	struct inet_bind_hashbucket *head;
	struct inet_bind_bucket *tb;
	int ret;
	struct net *net = sock_net(sk);
	int twrefcnt = 1;

	if (!snum) {
		int i, remaining, low, high, port;
		static u32 hint;
		u32 offset = hint + port_offset;
        ...

		inet_get_local_port_range(&low, &high);                    // get tcp port range
		remaining = (high - low) + 1;

        ...
		for (i = 1; i <= remaining; i++) {
			port = low + (i + offset) % remaining;
			if (inet_is_reserved_local_port(port))                    // ensure port is not in the reserved port list
				continue;
			head = &hinfo->bhash[inet_bhashfn(net, port,
					hinfo->bhash_size)];
            ...
			inet_bind_bucket_for_each(tb, &head->chain) {
				if (net_eq(ib_net(tb), net) &&                        // if the port is used in the same netns
				    tb->port == port) {
                    ...
					if (!check_established(death_row, sk,
								port, &tw))
						goto ok;
					goto next_port;
				}
			}

			tb = inet_bind_bucket_create(hinfo->bind_bucket_cachep,    
					net, head, port);
            ...
			tb->fastreuse = -1;
			tb->fastreuseport = -1;
			goto ok;

		next_port:
			spin_unlock(&head->lock);
		}
        ...

		return -EADDRNOTAVAIL;

ok:
		hint += i;

		/* Head lock still held and bh's disabled */
		inet_bind_hash(sk, tb, port);
		if (sk_unhashed(sk)) {
			inet_sk(sk)->inet_sport = htons(port);
			twrefcnt += hash(sk, tw);
		}
        ...

		ret = 0;
		goto out;
	}

    ...
out:
		local_bh_enable();
		return ret;
	}
}

static int __inet_check_established(struct inet_timewait_death_row *death_row,
				    struct sock *sk, __u16 lport,
				    struct inet_timewait_sock **twp)
{
	struct inet_hashinfo *hinfo = death_row->hashinfo;
	struct inet_sock *inet = inet_sk(sk);
	__be32 daddr = inet->inet_rcv_saddr;
	__be32 saddr = inet->inet_daddr;
	int dif = sk->sk_bound_dev_if;
	INET_ADDR_COOKIE(acookie, saddr, daddr)
	const __portpair ports = INET_COMBINED_PORTS(inet->inet_dport, lport);
	struct net *net = sock_net(sk);
	unsigned int hash = inet_ehashfn(net, daddr, lport,
					 saddr, inet->inet_dport);
	struct inet_ehash_bucket *head = inet_ehash_bucket(hinfo, hash);
	spinlock_t *lock = inet_ehash_lockp(hinfo, hash);
	struct sock *sk2;
	const struct hlist_nulls_node *node;
	struct inet_timewait_sock *tw;
	int twrefcnt = 0;

    ...

	/* And established part... */
	sk_nulls_for_each(sk2, node, &head->chain) {
		if (sk2->sk_hash != hash)
			continue;
		if (likely(INET_MATCH(sk2, net, acookie,
				      saddr, daddr, ports, dif)))
			goto not_unique;
	}

unique:
    ...
	return 0;

not_unique:
	spin_unlock(lock);
	return -EADDRNOTAVAIL;
}
```
* `inet_hash_connect`:
    * choose a source port from the tcp port range:
        * if the sport is not found in the `bhash`(not used):
            * use the source port
        * else:
            * if any of the established connections in `ehash` have the same four-tuple `(saddr, sport, daddr, dport)`:
                * continue to choose a next source port
            * else:
                * use the source port
* **a Linux machine can have multiple TCP connections with the same source port, as long as their four-tuple is not the same.**
* how to get/set the tcp port range and reserved ports?
```
# sysctl net.ipv4.ip_local_reserved_ports
net.ipv4.ip_local_reserved_ports =
# sudo sysctl net.ipv4.ip_local_port_range
net.ipv4.ip_local_port_range = 32768    60999
```
