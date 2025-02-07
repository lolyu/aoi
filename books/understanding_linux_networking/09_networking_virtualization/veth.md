# veth
```
# ip link add veth0 type veth peer name veth1
# ip addr add 192.168.1.1/24 dev veth0
# ip addr add 192.168.1.2/24 dev veth1
# ip link set veth0 up
# ip link set veth1 up
# echo 0 > /proc/sys/net/ipv4/conf/all/rp_filter
# echo 0 > /proc/sys/net/ipv4/conf/veth0/rp_filter
# echo 0 > /proc/sys/net/ipv4/conf/veth1/rp_filter
# echo 1 > /proc/sys/net/ipv4/conf/veth0/accept_local
# echo 1 > /proc/sys/net/ipv4/conf/veth1/accept_local
# ping 192.168.1.2 -I veth0
```
* the `veth` netlink link operations:
```c
static struct rtnl_link_ops veth_link_ops = {
	.kind		= DRV_NAME,
	.priv_size	= sizeof(struct veth_priv),
	.setup		= veth_setup,
	.validate	= veth_validate,
	.newlink	= veth_newlink,
	.dellink	= veth_dellink,
	.policy		= veth_policy,
	.maxtype	= VETH_INFO_MAX,
};
```

## veth pair creation
* `rtnl_newlink` to create `dev` first, then calls the `veth_newlink` to create the `peer`.
```c
static int rtnl_newlink(struct sk_buff *skb, struct nlmsghdr *nlh)
{
	if (1) {
        ...

		dev = rtnl_create_link(dest_net, ifname, ops, tb);
        ...
		dev->ifindex = ifm->ifi_index;

		if (ops->newlink)
			err = ops->newlink(net, dev, tb, data);
		else
			err = register_netdevice(dev);

        ...
		err = rtnl_configure_link(dev, ifm);
		put_net(dest_net);
		return err;
	}
}
```

```c
struct veth_priv {
	struct net_device __rcu	*peer;
	atomic64_t		dropped;
};

static int veth_newlink(struct net *src_net, struct net_device *dev,
			 struct nlattr *tb[], struct nlattr *data[])
{
	int err;
	struct net_device *peer;
	struct veth_priv *priv;

    ...
	peer = rtnl_create_link(net, ifname, &veth_link_ops, tbp);
	err = register_netdevice(peer);
    ...
	netif_carrier_off(peer);
    rtnl_configure_link(peer, ifmp);

    ...
	err = register_netdevice(dev);
	netif_carrier_off(dev);

    ...
	priv = netdev_priv(dev);
	rcu_assign_pointer(priv->peer, peer);

	priv = netdev_priv(peer);
	rcu_assign_pointer(priv->peer, dev);
	return 0;
    ...
}
```

## veth device up
```c
static const struct net_device_ops veth_netdev_ops = {
	.ndo_init            = veth_dev_init,
	.ndo_open            = veth_open,
	.ndo_stop            = veth_close,
	.ndo_start_xmit      = veth_xmit,
	.ndo_change_mtu      = veth_change_mtu,
	.ndo_get_stats64     = veth_get_stats64,
	.ndo_set_mac_address = eth_mac_addr,
};

static void veth_setup(struct net_device *dev)
{
	ether_setup(dev);

	dev->priv_flags &= ~IFF_TX_SKB_SHARING;
	dev->priv_flags |= IFF_LIVE_ADDR_CHANGE;

	dev->netdev_ops = &veth_netdev_ops;
	dev->ethtool_ops = &veth_ethtool_ops;
	dev->features |= NETIF_F_LLTX;
	dev->features |= VETH_FEATURES;
	dev->destructor = veth_dev_free;

	dev->hw_features = VETH_FEATURES;
}
```

## packet send/receive between veth pairs
* this part is pretty the same as the [local device I/O](https://github.com/lolyu/aoi/blob/master/books/understanding_linux_networking/04_local_device_io/local_device_io.md), the difference here is that the device here is a `veth` device instead of `loopback` device.
* if the out device has no queue -> `dev_hard_start_xmit` -> `dev->netdev_ops->ndo_start_xmit`
    * in this case, the send function is `veth_xmit`
* `veth_xmit` basically retrieves the peer device, and calls `netif_rx` on the `skb`, the receive path is basically the same:
    * `netif_rx` enqueues the `skb` into the per-cpu backlog packet queue.
    * `netif_rx` raises a soft IRQ to notify `ksoftirqd` to handle the packets.
```c
static netdev_tx_t veth_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct veth_priv *priv = netdev_priv(dev);
	struct net_device *rcv;
	int length = skb->len;

	rcu_read_lock();
	rcv = rcu_dereference(priv->peer);
    ..

	if (likely(dev_forward_skb(rcv, skb) == NET_RX_SUCCESS)) {
        // update statistics
		...
	}
	return NETDEV_TX_OK;
}
```
```c
/**
 * dev_forward_skb - loopback an skb to another netif
 *
 * @dev: destination network device
 * @skb: buffer to forward
 *
 * return values:
 *	NET_RX_SUCCESS	(no congestion)
 *	NET_RX_DROP     (packet was dropped, but freed)
 *
 * dev_forward_skb can be used for injecting an skb from the
 * start_xmit function of one device into the receive queue
 * of another device.
 *
 * The receiving device may be in another namespace, so
 * we have to clear all information in the skb that could
 * impact namespace isolation.
 */
int dev_forward_skb(struct net_device *dev, struct sk_buff *skb)
{
    ...
	skb_orphan(skb);

	if (unlikely(!is_skb_forwardable(dev, skb))) {
		atomic_long_inc(&dev->rx_dropped);
		kfree_skb(skb);
		return NET_RX_DROP;
	}
	skb->skb_iif = 0;
	skb->dev = dev;
	skb_dst_drop(skb);
	skb->tstamp.tv64 = 0;
	skb->pkt_type = PACKET_HOST;
	skb->protocol = eth_type_trans(skb, dev);
	skb->mark = 0;
	secpath_reset(skb);
	nf_reset(skb);
	nf_reset_trace(skb);
	return netif_rx(skb);
}
```

