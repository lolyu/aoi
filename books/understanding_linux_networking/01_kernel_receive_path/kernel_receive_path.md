# Linux kernel receive path

![image](https://github.com/user-attachments/assets/c544cdc2-4823-4db7-a596-4710dec25863)


* to sum up, each tx/tx queue has a ring buffer, and incoming packets are mapped/stored to specific rx queue by hashing; after NiC copies packets to the ring buffer, it will trigger a dedicated hardware interrupt, which will be handled by specific CPU (each rx queue has its own IRQ, and the IRQ could be handled by any CPU; but this can be configured by IRQ affinity); when the dedidate CPU handles the interrupt, it will add the ring buffer to the per-cpu poll list and trigger a software interrupt by setting a per-cpu flag. The `ksoftirqd` thread that runs on the same CPU keeps polling on the software interrupt flag, once it is set, it will call the corresponding software interrupt handler; In the software interrupt handler for packet receivement, it will retrieve the ring buffer from the poll list for this CPU and polls packets from the device by calling the polling function registered by the driver. The driver's polling function will fetch packets from the ring buffer, and pass it to the kernel networking stack.

## init igb driver
```c
static struct pci_driver igb_driver = {
	.name     = igb_driver_name,
	.id_table = igb_pci_tbl,
	.probe    = igb_probe,
	.remove   = igb_remove,
	.shutdown = igb_shutdown,
	.sriov_configure = igb_pci_sriov_configure,
	.err_handler = &igb_err_handler
};

static int __init igb_init_module(void)
{
	int ret;
	ret = pci_register_driver(&igb_driver);
	return ret;
}

module_init(igb_init_module);
```
* `igb_probe` is called to initialize the `igb` driver:
    * init DMA
    * hook `ethtool` implementations
    * hool device operation implementations
    * **init `NAPI`**?
```c
// drivers/net/ethernet/intel/igb/igb_ethtool.c
static const struct ethtool_ops igb_ethtool_ops = {
	.get_settings		= igb_get_settings,
	.set_settings		= igb_set_settings,
	.get_drvinfo		= igb_get_drvinfo,
	.get_regs_len		= igb_get_regs_len,
  ...
};
```
```c
// drivers/net/ethernet/intel/igb/igb_main.c
static const struct net_device_ops igb_netdev_ops = {
	.ndo_open		= igb_open,
	.ndo_stop		= igb_close,
	.ndo_start_xmit		= igb_xmit_frame,
	.ndo_get_stats64	= igb_get_stats64,
	.ndo_set_rx_mode	= igb_set_rx_mode,
	.ndo_set_mac_address	= igb_set_mac,
	.ndo_change_mtu		= igb_change_mtu,
	.ndo_do_ioctl		= igb_ioctl,
	.ndo_tx_timeout		= igb_tx_timeout,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_vlan_rx_add_vid	= igb_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid	= igb_vlan_rx_kill_vid,
	.ndo_set_vf_mac		= igb_ndo_set_vf_mac,
	.ndo_set_vf_vlan	= igb_ndo_set_vf_vlan,
	.ndo_set_vf_tx_rate	= igb_ndo_set_vf_bw,
	.ndo_set_vf_spoofchk	= igb_ndo_set_vf_spoofchk,
	.ndo_get_vf_config	= igb_ndo_get_vf_config,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= igb_netpoll,
#endif
	.ndo_fix_features	= igb_fix_features,
	.ndo_set_features	= igb_set_features,
};

static int igb_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct net_device *netdev;
	struct igb_adapter *adapter;

  ...

	err = -ENOMEM;
	netdev = alloc_etherdev_mq(sizeof(struct igb_adapter),
				   IGB_MAX_TX_QUEUES);

	netdev->netdev_ops = &igb_netdev_ops;              // set the device operation implementations
	igb_set_ethtool_ops(netdev);                       // set the device nettool implementations


	err = igb_sw_init(adapter);
```

### igb sw init
```c
static int igb_sw_init(struct igb_adapter *adapter)
{
	...
	/* This call may decrease the number of queues */
	if (igb_init_interrupt_scheme(adapter, true)) {
		dev_err(&pdev->dev, "Unable to allocate memory for queues\n");
		return -ENOMEM;
	}
	...
}

/**
 *  igb_init_interrupt_scheme - initialize interrupts, allocate queues/vectors
 *  @adapter: board private structure to initialize
 *  @msix: boolean value of MSIX capability
 *
 *  This function initializes the interrupts and allocates all of the queues.
 **/
static int igb_init_interrupt_scheme(struct igb_adapter *adapter, bool msix)
{
	struct pci_dev *pdev = adapter->pdev;
	int err;

	igb_set_interrupt_capability(adapter, msix);							// [1] count how many interrupts are needed = rx queue + tx queue + 1

	err = igb_alloc_q_vectors(adapter);										// [2] allocate the interrupt vector
	if (err) {
		dev_err(&pdev->dev, "Unable to allocate memory for vectors\n");
		goto err_alloc_q_vectors;
	}

	igb_cache_ring_register(adapter);

	return 0;

err_alloc_q_vectors:
	igb_reset_interrupt_capability(adapter);
	return err;
}
```
* `igb_sw_init` initializes the NiC software components, especially the interrupt part:
	* get the interrupt count and allocate the interrupt vector
* during the interrupt allocation, the `NAPI` is initialized with callback hook `igb_poll`
```c
static int igb_alloc_q_vector(struct igb_adapter *adapter,
			      int v_count, int v_idx,
			      int txr_count, int txr_idx,
			      int rxr_count, int rxr_idx)
{
	struct igb_q_vector *q_vector;
	struct igb_ring *ring;
	int ring_count, size;

	/* allocate q_vector and rings */
	q_vector = kzalloc(size, GFP_KERNEL);
	if (!q_vector)
		return -ENOMEM;

	/* initialize NAPI */
	netif_napi_add(adapter->netdev, &q_vector->napi,
		       igb_poll, 64);
	...
}
```

* `netif_napi_add` sets the `q_vector->napi.poll` as `igb_poll`

## startup NiC
* when the NiC is admin `UP`, `igb_open` is called to startup the NiC
	* all resources for send/receive operations are allocated/configured
	* register interrupt handler
	* start the watchdog timer
	* notify the interface is ready

```c
static int __igb_open(struct net_device *netdev, bool resuming)
{
	/* allocate transmit descriptors */
	err = igb_setup_all_tx_resources(adapter);		// allocate the tx resources

	/* allocate receive descriptors */
	err = igb_setup_all_rx_resources(adapter);		// allocate the rx resources

	igb_configure(adapter);							// configure the rx and tx

	err = igb_request_irq(adapter);
```

### allocate the ring buffer
```c
/* board specific private data structure */
struct igb_adapter {
	...
	/* TX */
	int num_tx_queues;
	struct igb_ring *tx_ring[16];

	/* RX */
	int num_rx_queues;
	struct igb_ring *rx_ring[16];
	...
};

struct igb_ring {
	struct igb_q_vector *q_vector;	/* backlink to q_vector */
	struct net_device *netdev;	/* back pointer to net_device */
	struct device *dev;		/* device pointer for dma mapping */
	union {				/* array of buffer info structs */
		struct igb_tx_buffer *tx_buffer_info;
		struct igb_rx_buffer *rx_buffer_info;
	};
	unsigned long last_rx_timestamp;
	void *desc;			/* descriptor ring memory */
	unsigned long flags;		/* ring specific flags */
	void __iomem *tail;		/* pointer to ring tail register */
	dma_addr_t dma;			/* phys address of the ring */
	unsigned int  size;		/* length of desc. ring in bytes */
	...
};

/* wrapper around a pointer to a socket buffer,
 * so a DMA handle can be stored along with the buffer
 */
struct igb_tx_buffer {
	union e1000_adv_tx_desc *next_to_watch;
	unsigned long time_stamp;
	struct sk_buff *skb;
	unsigned int bytecount;
	u16 gso_segs;
	__be16 protocol;
	DEFINE_DMA_UNMAP_ADDR(dma);
	DEFINE_DMA_UNMAP_LEN(len);
	u32 tx_flags;
};

struct igb_rx_buffer {
	dma_addr_t dma;
	struct page *page;
	unsigned int page_offset;
};
```
```c
static int igb_setup_all_tx_resources(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	int i, err = 0;

	for (i = 0; i < adapter->num_tx_queues; i++) {
		err = igb_setup_tx_resources(adapter->tx_ring[i]);
		...
	}

	return err;
}

static int igb_setup_all_rx_resources(struct igb_adapter *adapter)
{
	struct pci_dev *pdev = adapter->pdev;
	int i, err = 0;

	for (i = 0; i < adapter->num_rx_queues; i++) {
		err = igb_setup_rx_resources(adapter->rx_ring[i]);
		...
	}

	return err;
}

int igb_setup_tx_resources(struct igb_ring *tx_ring)
{
	struct device *dev = tx_ring->dev;
	int size;

	size = sizeof(struct igb_tx_buffer) * tx_ring->count;
	tx_ring->tx_buffer_info = vzalloc(size);

	tx_ring->size = tx_ring->count * sizeof(union e1000_adv_tx_desc);
	tx_ring->size = ALIGN(tx_ring->size, 4096);
	tx_ring->desc = dma_alloc_coherent(dev, tx_ring->size,
					   &tx_ring->dma, GFP_KERNEL);

	return 0;
}

int igb_setup_rx_resources(struct igb_ring *rx_ring)
{
	struct device *dev = rx_ring->dev;
	int size;

	size = sizeof(struct igb_rx_buffer) * rx_ring->count;
	rx_ring->rx_buffer_info = vzalloc(size);

	rx_ring->size = rx_ring->count * sizeof(union e1000_adv_rx_desc);
	rx_ring->size = ALIGN(rx_ring->size, 4096);
	rx_ring->desc = dma_alloc_coherent(dev, rx_ring->size,
					   &rx_ring->dma, GFP_KERNEL);

	return 0;
}
```
### configure the rx/tx
```c
/**
 *  igb_configure - configure the hardware for RX and TX
 *  @adapter: private board structure
 **/
static void igb_configure(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	int i;

	igb_setup_mrqc(adapter);								// setup the RSS

	igb_configure_tx(adapter);
	igb_configure_rx(adapter);

	/* call igb_desc_unused which always leaves
	 * at least 1 descriptor unused to make sure
	 * next_to_use != next_to_clean
	 */
	for (i = 0; i < adapter->num_rx_queues; i++) {
		struct igb_ring *ring = adapter->rx_ring[i];
		igb_alloc_rx_buffers(ring, igb_desc_unused(ring));
	}
}
```

### register interrupt handler
```c
/**
 *  igb_request_msix - Initialize MSI-X interrupts
 *  @adapter: board private structure to initialize
 *
 *  igb_request_msix allocates MSI-X vectors and requests interrupts from the
 *  kernel.
 **/
static int igb_request_msix(struct igb_adapter *adapter)
{
	struct net_device *netdev = adapter->netdev;
	struct e1000_hw *hw = &adapter->hw;
	int i, err = 0, vector = 0, free_vector = 0;

	err = request_irq(adapter->msix_entries[vector].vector,
			  igb_msix_other, 0, netdev->name, adapter);

	for (i = 0; i < adapter->num_q_vectors; i++) {
		struct igb_q_vector *q_vector = adapter->q_vector[i];

		vector++;

		q_vector->itr_register = hw->hw_addr + E1000_EITR(vector);

		err = request_irq(adapter->msix_entries[vector].vector,
				  igb_msix_ring, 0, q_vector->name,
				  q_vector);
	}

	igb_configure_msix(adapter);
	return 0;
}

static void igb_set_interrupt_capability(struct igb_adapter *adapter, bool msix)
{
	int err;
	int numvecs, i;

	if (!msix)
		goto msi_only;

	/* Number of supported queues. */
	adapter->num_rx_queues = adapter->rss_queues;
	if (adapter->vfs_allocated_count)
		adapter->num_tx_queues = 1;
	else
		adapter->num_tx_queues = adapter->rss_queues;

	/* start with one vector for every Rx queue */
	numvecs = adapter->num_rx_queues;

	/* if Tx handler is separate add 1 for every Tx queue */
	if (!(adapter->flags & IGB_FLAG_QUEUE_PAIRS))
		numvecs += adapter->num_tx_queues;

	/* store the number of vectors reserved for queues */
	adapter->num_q_vectors = numvecs;

	/* add 1 vector for link status interrupts */
	numvecs++;
	adapter->msix_entries = kcalloc(numvecs, sizeof(struct msix_entry),
					GFP_KERNEL);

	if (!adapter->msix_entries)
		goto msi_only;

	for (i = 0; i < numvecs; i++)
		adapter->msix_entries[i].entry = i;

	err = pci_enable_msix(adapter->pdev,
			      adapter->msix_entries,
			      numvecs);
	if (err == 0)
		return;
}
```
* `igb_msix_ring` is registered as hardware interrupt handler
* the interrupts registered = rx queue count + tx queue count + 1 (for the link status)

## handware interrupt handle
* when the NiC receives packets, it transfer the packet to the ring buffer, and request a hardware interrupt, which will be handled by `igb_msix_ring`

```c
static irqreturn_t igb_msix_ring(int irq, void *data)
{
	struct igb_q_vector *q_vector = data;

	napi_schedule(&q_vector->napi);

	return IRQ_HANDLED;
}
```
```c
// include/linux/netdevice.h
static inline void napi_schedule(struct napi_struct *n)
{
	if (napi_schedule_prep(n))
		__napi_schedule(n);
}
```
```c
// net/core/dev.c
void __napi_schedule(struct napi_struct *n)
{
	unsigned long flags;

	local_irq_save(flags);
	____napi_schedule(&__get_cpu_var(softnet_data), n);
	local_irq_restore(flags);
}

static inline void ____napi_schedule(struct softnet_data *sd,
				     struct napi_struct *napi)
{
	list_add_tail(&napi->poll_list, &sd->poll_list);
	__raise_softirq_irqoff(NET_RX_SOFTIRQ);
}
```
* `igb_msix_ring` does two things:
	* add the `poll_list` from driver's `napi_struct` to the per-cpu `softnet_data`'s `poll_list`
	* trigger a software interrupt: `NET_RX_SOFTIRQ`
		* the software interrupt is set via settin a flag over a per-cpu variable, so only the `ksoftirqd` thread that runs on the same CPU will handle the software interrupt.
			* **the hardware and software interrupts are handled on the same CPU**

## software interrupt handle
```c
// kernel/softirq.c
static struct smp_hotplug_thread softirq_threads = {
	.store			= &ksoftirqd,
	.thread_should_run	= ksoftirqd_should_run,
	.thread_fn		= run_ksoftirqd,
	.thread_comm		= "ksoftirqd/%u",
};

asmlinkage void __do_softirq(void)
{
	struct softirq_action *h;
	__u32 pending;
	int cpu;

	pending = local_softirq_pending();
	lockdep_softirq_enter();

	cpu = smp_processor_id();
restart:
	/* Reset the pending bitmask before enabling irqs */
	set_softirq_pending(0);

	local_irq_enable();

	h = softirq_vec;

	do {
		if (pending & 1) {
			unsigned int vec_nr = h - softirq_vec;
			int prev_count = preempt_count();

			kstat_incr_softirqs_this_cpu(vec_nr);

			trace_softirq_entry(vec_nr);
			h->action(h);
			trace_softirq_exit(vec_nr);
			...
		}
		h++;
		pending >>= 1;
	} while (pending);

	local_irq_disable();
}
```
* `__do_softirq` loops the irq table and calls the handler for each software IRQ.
* the handler for `NET_RX_SOFTIRQ` is `net_rx_action`

### `net_rx_action`
```c
static void net_rx_action(struct softirq_action *h)
{
	struct softnet_data *sd = &__get_cpu_var(softnet_data);
	unsigned long time_limit = jiffies + 2;
	int budget = netdev_budget;
	void *have;

	local_irq_disable();

	while (!list_empty(&sd->poll_list)) {
		struct napi_struct *n;
		int work, weight;

		/* If softirq window is exhuasted then punt.
		 * Allow this to run for 2 jiffies since which will allow
		 * an average latency of 1.5/HZ.
		 */
		if (unlikely(budget <= 0 || time_after_eq(jiffies, time_limit)))
			goto softnet_break;

		local_irq_enable();

		/* Even though interrupts have been re-enabled, this
		 * access is safe because interrupts can only add new
		 * entries to the tail of this list, and only ->poll()
		 * calls can remove this head entry from the list.
		 */
		n = list_first_entry(&sd->poll_list, struct napi_struct, poll_list);


		weight = n->weight;

		work = 0;
		if (test_bit(NAPI_STATE_SCHED, &n->state)) {
			work = n->poll(n, weight);
			trace_napi_poll(n);
		}

		WARN_ON_ONCE(work > weight);

		budget -= work;
		local_irq_disable();

		if (unlikely(work == weight)) {
				list_move_tail(&n->poll_list, &sd->poll_list);
			}
		}

		netpoll_poll_unlock(have);
	}
out:
	net_rps_action_and_irq_enable(sd);

	return;
softnet_break:
	sd->time_squeeze++;
	__raise_softirq_irqoff(NET_RX_SOFTIRQ);
	goto out;
}
```

* what's the `poll` function called by `net_rx_action`?
	* `igb_poll` is the `poll` function.
* if the device polls the same packets as the `weight` (`work` == `weight`), the device will be added back to the poll list.
* **difference between `weight` and `budget`?**
	* `budget` controls how many packets `net_rx_action` polls from the device list.
	* `weight` controls how many packets `net_rx_action` polls from a single device.
		* `weight` is hard coded to 64 for igb driver, it is set by `netif_napi_add`
* what if there are packets available to poll after timeout or `budget` drops to zero?
	* `net_rx_action` will do two things:
		1. increase `softnet_data::time_squeeze`, which could be checked from the third column of `/proc/net/softnet_stat`
 		2. trigger another software interrupt: `NET_RX_SOFTIRQ`
			* this explains why software interrupt `NET_RX_SOFTIRQ` count is much larger than the hardware interrupt count

```c
// net/core/dev.c
void netif_napi_add(struct net_device *dev, struct napi_struct *napi,
		    int (*poll)(struct napi_struct *, int), int weight)
{
	INIT_LIST_HEAD(&napi->poll_list);
	napi->poll = poll;
	if (weight > NAPI_POLL_WEIGHT)
		pr_err_once("netif_napi_add() called with weight %d on device %s\n",
			    weight, dev->name);
	napi->weight = weight;
	list_add(&napi->dev_list, &dev->napi_list);
	...
}
```

### `igb_poll`
```c
/**
 *  igb_poll - NAPI Rx polling callback
 *  @napi: napi polling structure
 *  @budget: count of how many packets we should handle
 **/
static int igb_poll(struct napi_struct *napi, int budget)
{
	struct igb_q_vector *q_vector = container_of(napi,
						     struct igb_q_vector,
						     napi);
	bool clean_complete = true;

	if (q_vector->tx.ring)
		clean_complete = igb_clean_tx_irq(q_vector);

	if (q_vector->rx.ring)
		clean_complete &= igb_clean_rx_irq(q_vector, budget);

	/* If all work not completed, return budget and keep polling */
	if (!clean_complete)
		return budget;

	/* If not enough Rx work done, exit the polling mode */
	napi_complete(napi);
	igb_ring_irq_enable(q_vector);

	return 0;
}
```
### `igb_clean_rx_irq`
```c
static bool igb_clean_rx_irq(struct igb_q_vector *q_vector, const int budget)
{
	struct igb_ring *rx_ring = q_vector->rx.ring;
	struct sk_buff *skb = rx_ring->skb;
	unsigned int total_bytes = 0, total_packets = 0;
	u16 cleaned_count = igb_desc_unused(rx_ring);

	do {
		union e1000_adv_rx_desc *rx_desc;

		rx_desc = IGB_RX_DESC(rx_ring, rx_ring->next_to_clean);

		skb = igb_fetch_rx_buffer(rx_ring, rx_desc, skb);

		cleaned_count++;

		/* fetch next buffer in frame if non-eop */
		if (igb_is_non_eop(rx_ring, rx_desc))
			continue;

		/* verify the packet layout is correct */
		if (igb_cleanup_headers(rx_ring, rx_desc, skb)) {
			skb = NULL;
			continue;
		}

		/* probably a little skewed due to removing CRC */
		total_bytes += skb->len;

		/* populate checksum, timestamp, VLAN, and protocol */
		igb_process_skb_fields(rx_ring, rx_desc, skb);

		napi_gro_receive(&q_vector->napi, skb);

		/* reset skb pointer */
		skb = NULL;

		/* update budget accounting */
		total_packets++;
	} while (likely(total_packets < budget));

	...
	
	return (total_packets < budget);
}
```
```c
gro_result_t napi_gro_receive(struct napi_struct *napi, struct sk_buff *skb)
{
	skb_gro_reset_offset(skb);

	return napi_skb_finish(dev_gro_receive(napi, skb), skb);
}

static gro_result_t napi_skb_finish(gro_result_t ret, struct sk_buff *skb)
{
	switch (ret) {
	case GRO_NORMAL:
		if (netif_receive_skb(skb))
			ret = GRO_DROP;
		break;

	...
	return ret;
}

int netif_receive_skb(struct sk_buff *skb)
{
	net_timestamp_check(netdev_tstamp_prequeue, skb);

	...

	return __netif_receive_skb(skb);
}

static int __netif_receive_skb(struct sk_buff *skb)
{
	int ret;

	if (sk_memalloc_socks() && skb_pfmemalloc(skb)) {
		unsigned long pflags = current->flags;

		/*
		 * PFMEMALLOC skbs are special, they should
		 * - be delivered to SOCK_MEMALLOC sockets only
		 * - stay away from userspace
		 * - have bounded memory usage
		 *
		 * Use PF_MEMALLOC as this saves us from propagating the allocation
		 * context down to all allocation sites.
		 */
		current->flags |= PF_MEMALLOC;
		ret = __netif_receive_skb_core(skb, true);
		tsk_restore_flags(current, pflags, PF_MEMALLOC);
	} else
		ret = __netif_receive_skb_core(skb, false);

	return ret;
}
```
* the packets received will be passed to `netif_receive_skb`, which will be dispatched to the protocol the packet belongs to.

