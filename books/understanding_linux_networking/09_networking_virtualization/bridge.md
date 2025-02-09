# bridge

![image](https://github.com/user-attachments/assets/abbd2346-f6d4-4f3c-a8aa-e287d3576f2e)


* lab setup
```
# ip netns add net1
# ip link add veth1 type veth peer name veth1_p
# ip link set veth1 netns net1
# ip netns exec net1 ip addr add 192.168.0.101/24 dev veth1
# ip netns exec net1 ip link set veth1 up
# ip netns add net2
# ip link add veth2 type veth peer name veth2_p
# ip link set veth2 netns net2
# ip netns exec net2 ip addr add 192.168.0.102/24 dev veth2
# ip netns exec net2 ip link set veth2 up
# brctl addbr br0
# ip link set dev veth1_p master br0
# ip link set dev veth2_p master br0
# ip addr add 192.168.0.100/24 dev br0
# ifconfig veth1_p up
# ifconfig veth2_p up
# ifconfig br0 up
# brctl show
bridge name     bridge id               STP enabled     interfaces
br0             8000.36ead04aabc9       no              veth1_p
                                                        veth2_p
# ip netns exec net1 ping 192.168.0.102 -I veth1
PING 192.168.0.102 (192.168.0.102) from 192.168.0.101 veth1: 56(84) bytes of data.
64 bytes from 192.168.0.102: icmp_seq=1 ttl=64 time=0.082 ms
64 bytes from 192.168.0.102: icmp_seq=2 ttl=64 time=0.054 ms
...
```

## bridge structures
```c
struct net_bridge
{
	struct list_head		port_list;                            // list of ports that are part of the bridge
	struct net_device		*dev;                                 // each bridge device is associated with a `struct net_device`, this points to the associated `net_device`

	struct br_cpu_netstats __percpu *stats;
	struct hlist_head		hash[BR_HASH_SIZE];                   // 
    ...
}
```

## bridge creation
```c
int br_add_bridge(struct net *net, const char *name)
{
	struct net_device *dev;
	dev = alloc_netdev(sizeof(struct net_bridge), name,
			   br_dev_setup);

	dev_net_set(dev, net);
	dev->rtnl_link_ops = &br_link_ops;

	res = register_netdev(dev);
	return res;
}
```
* `alloc_netdev` -> `alloc_netdev_mqs`:
    * `alloc_netdev_mqs` allocates a memory space to store two objects: `struct net_device` + `struct net_bridge`
```c
struct net_device *alloc_netdev_mqs(int sizeof_priv, const char *name,
		void (*setup)(struct net_device *),
		unsigned int txqs, unsigned int rxqs)
{
	struct net_device *dev;
	size_t alloc_size;
	struct net_device *p;

	alloc_size = sizeof(struct net_device);
	if (sizeof_priv) {
		/* ensure 32-byte alignment of private area */
		alloc_size = ALIGN(alloc_size, NETDEV_ALIGN);
		alloc_size += sizeof_priv;
	}
	/* ensure 32-byte alignment of whole construct */
	alloc_size += NETDEV_ALIGN - 1;

	p = kzalloc(alloc_size, GFP_KERNEL);
    ...
```

## add device to bridge
![image](https://github.com/user-attachments/assets/0af5f4d3-b9c0-448f-be1d-95f312de879e)

```c
int br_add_if(struct net_bridge *br, struct net_device *dev)
{
	struct net_bridge_port *p;
    ...
	p = new_nbp(br, dev);
    ...
	list_add_rcu(&p->list, &br->port_list);
    ...
	err = netdev_rx_handler_register(dev, br_handle_frame, p);
    ...
}

static struct net_bridge_port *new_nbp(struct net_bridge *br,
				       struct net_device *dev)
{
	int index;
	struct net_bridge_port *p;

	index = find_portno(br);                        // find next available port number
    ...
	p = kzalloc(sizeof(*p), GFP_KERNEL);
    ...
	p->br = br;
	p->dev = dev;
	p->path_cost = port_cost(dev);
	p->priority = 0x8000 >> BR_PORT_BITS;
	p->port_no = index;
	p->flags = 0;
	br_init_port(p);
	p->state = BR_STATE_DISABLED;
    ...

	return p;
}
```
* `struct net_bridge_port` represents a bridge port; adding a device to bridge is like connecting the cable to a physical Ethernet port of a switch.
* **NOTE**: `netdev_rx_handler_register` register receive handler for the bridge port.
    * when a `veth` device is added to bridge, its receive handler is set as `br_handle_frame`
* **what's the original receive handler of `veth` device?**
    * its original handler is not set.

## packet switching
* when a packet is sent from one side of the `veth` pair, it is enqueued to the CPU's backlog queue, soft IRQ is raised and `ksoftirqd` will handle the receivement -> `__netif_receive_skb_core` will be called to demultiplex the packet.
* `__netif_receive_skb_core` demultiplexes the packet:
    * determines the packet type and pass the packet to the protocol family handler.
    * if the packet device has receive handler, pass the packet to the receive handler.

```c

static int __netif_receive_skb_core(struct sk_buff *skb, bool pfmemalloc)
{
	struct packet_type *ptype, *pt_prev;
	rx_handler_func_t *rx_handler;

    ...
	rx_handler = rcu_dereference(skb->dev->rx_handler);                               // calls device receive handler if any
	if (rx_handler) {
		if (pt_prev) {
			ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = NULL;
		}
		switch (rx_handler(&skb)) {
		case RX_HANDLER_CONSUMED:                                                     // jump to the end, skip the packet demultiplexing based on packet type
			ret = NET_RX_SUCCESS;
			goto unlock;
		case RX_HANDLER_ANOTHER:
			goto another_round;
		case RX_HANDLER_EXACT:
			deliver_exact = true;
		case RX_HANDLER_PASS:
			break;
		default:
			BUG();
		}
	}

	type = skb->protocol;                                                            // demultiplexing the packet to its protocol family handler
	list_for_each_entry_rcu(ptype,
			&ptype_base[ntohs(type) & PTYPE_HASH_MASK], list) {
		if (ptype->type == type &&
		    (ptype->dev == null_or_dev || ptype->dev == skb->dev ||
		     ptype->dev == orig_dev)) {
			if (pt_prev)
				ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = ptype;
		}
	}

	if (pt_prev) {
		if (unlikely(skb_orphan_frags(skb, GFP_ATOMIC)))
			goto drop;
		else
			ret = pt_prev->func(skb, skb->dev, pt_prev, orig_dev);
	} else {
drop:
    ...
	}

unlock:
	rcu_read_unlock();
out:
	return ret;
}
```
* `br_handle_frame` is the receive handler for `veth` device in this case.

### `br_handle_frame`
```c
rx_handler_result_t br_handle_frame(struct sk_buff **pskb)
{
    ...
forward:
	switch (p->state) {
	case BR_STATE_FORWARDING:
		...
		/* fall through */
	case BR_STATE_LEARNING:
        ...
		NF_HOOK(NFPROTO_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
        ...
	}
	return RX_HANDLER_CONSUMED;
}
```

#### `br_handle_frame_finish`
* `br_handle_frame_finish` -> `__br_forward` -> `br_forward_finish` -> `br_dev_queue_push_xmit` -> `dev_queue_xmit` -> `dev_hard_start_xmit` -> `veth_xmit`
```c
int br_handle_frame_finish(struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_port *p = br_port_get_rcu(skb->dev);
	struct net_bridge *br;
	struct net_bridge_fdb_entry *dst;
	struct net_bridge_mdb_entry *mdst;
	struct sk_buff *skb2;
	u16 vid = 0;

	/* insert into forwarding database after filtering to avoid spoofing */
	br = p->br;
	br_fdb_update(br, p, eth_hdr(skb)->h_source, vid);                                    // update the bridge fdb table

	if (!is_broadcast_ether_addr(dest) && is_multicast_ether_addr(dest) &&

	BR_INPUT_SKB_CB(skb)->brdev = br->dev;

	/* The packet skb2 goes to the local host (NULL to skip). */
	skb2 = NULL;

	if (br->dev->flags & IFF_PROMISC)
		skb2 = skb;

	skb2 = NULL;
	dst = NULL;

	if (is_broadcast_ether_addr(dest))
		skb2 = skb;
	} else if ((dst = __br_fdb_get(br, dest, vid)) &&
			dst->is_local) {
		skb2 = skb;
		/* Do not forward the packet since it's local. */
		skb = NULL;
	}

	if (skb) {
		if (dst) {
			dst->used = jiffies;
			br_forward(dst->dst, skb, skb2);
		} else
			br_flood_forward(br, skb, skb2);
	}

	return 0;
}
```
* `_br_forward` basically changes the packet device from the source `veth` device to the target `veth` device(calculated based on the bridge fdb)

![image](https://github.com/user-attachments/assets/ef3903b9-5c8d-41a4-857b-720874a4cb11)

```c
static void __br_forward(const struct net_bridge_port *to, struct sk_buff *skb)
{
	struct net_device *indev;
    ...
	indev = skb->dev;
	skb->dev = to->dev;
	skb_forward_csum(skb);

	NF_HOOK(NFPROTO_BRIDGE, NF_BR_FORWARD, skb, indev, skb->dev,
		br_forward_finish);
}
```
```c
int br_forward_finish(struct sk_buff *skb)
{
	return NF_HOOK(NFPROTO_BRIDGE, NF_BR_POST_ROUTING, skb, NULL, skb->dev,
		       br_dev_queue_push_xmit);

}
```

## references
