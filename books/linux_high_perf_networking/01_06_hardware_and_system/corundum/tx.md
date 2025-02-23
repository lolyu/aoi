# TX

## the tx operations

![image](https://github.com/user-attachments/assets/fbe19576-4e76-4590-8b21-ee4a0f34c787)



* the operations:
	1. the kernel network stack calls the device's `ndo_start_xmit` to transfer the packet; for corundum, it is `mqnic_start_xmit`
	2. `mqnic_start_xmit` does the following:
		* get the next index as from the software head ptr
		* get the descriptor from the next index
		* get the `struct mqnic_tx_info` and `struct mqnic_desc` from the next index;
		* stores the skb DMA address in the descriptor (`struct mqnic_desc`) and keeps the skb in the `struct mqnic_tx_info`
		* update the software head ptr
	3.  `mqnic_start_xmit` writes the software head ptr into hardware head ptr register.
	4. after NIC finishes sending the packets in the tx queue, it will enqueue those packets into the tx completion queue
		* actually the NIC fills up the completion queue with the descriptor indexes of those sent packets.
	5. NIC enqueue a send finish event to the event queue
	6. NIC triggers a hardware IRQ
	7. the hardware IRQ handler calls the corresponding TX completion queue's handler - `mqnic_tx_irq`, and `mqnic_tx_irq` in turn raise soft IRQ to delegate the completion queue handle job to the `ksoftirqd`
	8. `ksoftirqd` calls the TX completion queue's NAPI poll function - `mqnic_poll_tx_cq` to free up the descriptors in the tx queue based on the tx completion queue

## ring basics

![image](https://github.com/user-attachments/assets/48d81f22-f274-4f85-827a-a09f3196d923)

* each ring buffer has customer/producer:
    * producer pushes data into the ring buffer and increment the head ptr.
    * customer pops data out of the ring buffer and increment the head ptr.
* two situations:
    * software as the producer, hardware as the customer
        * software pushes data into the ring buffer, update the software head ptr, and update the hardware head ptr register to notify hardware to process data.
        * the hardware keeps consuming data from the ring buffer as long as the data is available in ring buffer: hardware tail ptr != hardware head ptr.
    * hardware as the producer, software as the customer
        * hardware pushes data into the ring buffer, update the hardware head ptr, raise IRQ to notify software to process data.
        * the software updates the software head ptr by reading the hardware head ptr register, and keeps consuming data from the ring buffer as long as the data is available in ring buffer: software tail ptr != software head ptr.
        * software updates the hardware tail ptr register to notify hardware how many data is consumed.

## TX ring allocation
```
struct mqnic_priv {
    ...

	struct mqnic_eq_ring *event_ring[MQNIC_MAX_EVENT_RINGS];
	struct mqnic_ring *tx_ring[MQNIC_MAX_TX_RINGS];
	struct mqnic_cq_ring *tx_cpl_ring[MQNIC_MAX_TX_CPL_RINGS];
	struct mqnic_ring *rx_ring[MQNIC_MAX_RX_RINGS];
	struct mqnic_cq_ring *rx_cpl_ring[MQNIC_MAX_RX_CPL_RINGS];
	struct mqnic_port *ports[MQNIC_MAX_PORTS];
};
```
```c
int mqnic_init_netdev(struct mqnic_dev *mdev, int index, u8 __iomem *hw_addr)
{
	for (k = 0; k < priv->event_queue_count; k++) {
		ret = mqnic_create_eq_ring(
			priv,
			&priv->event_ring[k],
			1024,
			MQNIC_EVENT_SIZE,
			k,
			hw_addr + priv->event_queue_offset + k * MQNIC_EVENT_QUEUE_STRIDE
		); // TODO configure/constant
	}

	for (k = 0; k < priv->tx_queue_count; k++) {
		ret = mqnic_create_tx_ring(
			priv,
			&priv->tx_ring[k],
			1024,
			MQNIC_DESC_SIZE * desc_block_size,
			k,
			hw_addr + priv->tx_queue_offset + k * MQNIC_QUEUE_STRIDE
		); // TODO configure/constant
	}

	for (k = 0; k < priv->tx_cpl_queue_count; k++) {
		ret = mqnic_create_cq_ring(
			priv,
			&priv->tx_cpl_ring[k],
			1024,
			MQNIC_CPL_SIZE,
			k,
			hw_addr + priv->tx_cpl_queue_offset + k * MQNIC_CPL_QUEUE_STRIDE
		); // TODO configure/constant
	}
}
```

* key structures:
    * `mqnic_ring`: rx/tx ring
    * `mqnic_eq_ring`: event ring
    * `mqnic_cq_ring`: rx/tx completion ring
* each ring has four pointers:
    * software head ptr
    * software tail ptr
    * hardware head ptr
    * software tail ptr

```c
struct mqnic_tx_info {
	struct sk_buff *skb;
	DEFINE_DMA_UNMAP_ADDR(dma_addr);
	DEFINE_DMA_UNMAP_LEN(len);
	u32 frag_count;
	struct mqnic_frag frags[MQNIC_MAX_FRAGS - 1];
	int ts_requested;
};

struct mqnic_ring {
	// written on enqueue (i.e. start_xmit)
	u32 head_ptr;
	u32 tail_ptr;

    ...
	size_t buf_size;
	u8 *buf;
	dma_addr_t buf_dma_addr;

	union {
		struct mqnic_tx_info *tx_info;
		struct mqnic_rx_info *rx_info;
	};

    ...
	u32 hw_ptr_mask;
	u8 __iomem *hw_addr;
	u8 __iomem *hw_head_ptr;
	u8 __iomem *hw_tail_ptr;
} ____cacheline_aligned_in_smp;

struct mqnic_cq_ring {
	u32 head_ptr;
	u32 tail_ptr;

    ...
	size_t buf_size;
	u8 *buf;
	dma_addr_t buf_dma_addr;


	void (*handler)(struct mqnic_cq_ring *ring);

	u32 hw_ptr_mask;
	u8 __iomem *hw_addr;
	u8 __iomem *hw_head_ptr;
	u8 __iomem *hw_tail_ptr;
};

struct mqnic_eq_ring {
	u32 head_ptr;
	u32 tail_ptr;

    ...
	size_t buf_size;
	u8 *buf;
	dma_addr_t buf_dma_addr;

	int irq;

	void (*handler)(struct mqnic_eq_ring *ring);

	u32 hw_ptr_mask;
	u8 __iomem *hw_addr;
	u8 __iomem *hw_head_ptr;
	u8 __iomem *hw_tail_ptr;
};
```

### `mqnic_create_tx_ring`
* `struct mqnic_ring` allocates DMAed ring buffer to store the descriptors with four pointers:

```c
int mqnic_create_tx_ring(struct mqnic_priv *priv, struct mqnic_ring **ring_ptr,
		int size, int stride, int index, u8 __iomem *hw_addr)
{
	ring = kzalloc(sizeof(*ring), GFP_KERNEL);
	ring->tx_info = kvzalloc(sizeof(*ring->tx_info) * ring->size, GFP_KERNEL);                        // allocate mqnic_tx_info list, same count as descriptors, used to store ths sk_buff for the corresponding descriptor
	ring->buf_size = ring->size * ring->stride;
	ring->buf = dma_alloc_coherent(dev, ring->buf_size, &ring->buf_dma_addr, GFP_KERNEL);            // allocate DMA memory to store descriptors

	ring->hw_addr = hw_addr;
	ring->hw_ptr_mask = 0xffff;
	ring->hw_head_ptr = hw_addr + MQNIC_QUEUE_HEAD_PTR_REG;                                            // set ring head ptr register address, this is hardware head ptr
	ring->hw_tail_ptr = hw_addr + MQNIC_QUEUE_TAIL_PTR_REG;                                            // set ring tail ptr register address, this is hardware tail ptr

	ring->head_ptr = 0;                                                                                // set the software head ptr                                              
	ring->tail_ptr = 0;                                                                                // set the software tail ptr
	ring->clean_tail_ptr = 0;

	// set base address
	iowrite32(ring->buf_dma_addr, ring->hw_addr + MQNIC_QUEUE_BASE_ADDR_REG + 0);
	iowrite32(ring->buf_dma_addr >> 32, ring->hw_addr + MQNIC_QUEUE_BASE_ADDR_REG + 4);
	// set completion queue index
	iowrite32(0, ring->hw_addr + MQNIC_QUEUE_CPL_QUEUE_INDEX_REG);
	// set pointers
	iowrite32(ring->head_ptr & ring->hw_ptr_mask, ring->hw_addr + MQNIC_QUEUE_HEAD_PTR_REG);            // init the hardware head ptr
	iowrite32(ring->tail_ptr & ring->hw_ptr_mask, ring->hw_addr + MQNIC_QUEUE_TAIL_PTR_REG);            // init the software tail ptr
    ...
}
```

## tx logic
* tx operation is implemented by `mqnic_start_xmit`
	* `mqnic_start_xmit` adds packets to the ring buffer and updates the software head ptr.
 	* `mqnic_start_xmit` updates the hardware head ptr register to notify the hardware to send the packets.

```c
netdev_tx_t mqnic_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	ring_index = skb_get_queue_mapping(skb);
	ring = priv->tx_ring[ring_index];

	index = ring->head_ptr & ring->size_mask;

	tx_desc = (struct mqnic_desc *)(ring->buf + index * ring->stride);

	tx_info = &ring->tx_info[index];

	// map skb
	if (!mqnic_map_skb(ring, tx_info, tx_desc, skb))
		// map failed
		goto tx_drop_count;

	// count packet
	ring->packets++;
	ring->bytes += skb->len;

	// enqueue
	ring->head_ptr++;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 2, 0)
	if (unlikely(!netdev_xmit_more() || stop_queue)) {
#else
	if (unlikely(!skb->xmit_more || stop_queue)) {
#endif
		dma_wmb();
		mqnic_tx_write_head_ptr(ring);
	}

	return NETDEV_TX_OK;
}

void mqnic_tx_write_head_ptr(struct mqnic_ring *ring)
{
	iowrite32(ring->head_ptr & ring->hw_ptr_mask, ring->hw_head_ptr);
}

static bool mqnic_map_skb(struct mqnic_ring *ring, struct mqnic_tx_info *tx_info,
		struct mqnic_desc *tx_desc, struct sk_buff *skb)
{
	struct skb_shared_info *shinfo = skb_shinfo(skb);
	const skb_frag_t *frag;
	u32 i;
	u32 len;
	dma_addr_t dma_addr;

	// update tx_info
	tx_info->skb = skb;                                                                        // bookkeeping information in mqnic_tx_info
	tx_info->frag_count = 0;

	// map skb
	len = skb_headlen(skb);
	dma_addr = dma_map_single(ring->dev, skb->data, len, PCI_DMA_TODEVICE);                    // map the skb data for DMA transfer to NIC


	// write descriptor
	tx_desc[0].len = cpu_to_le32(len);                                                         // stores the DMA address in the descriptor
	tx_desc[0].addr = cpu_to_le64(dma_addr);

	// update tx_info
	dma_unmap_addr_set(tx_info, dma_addr, dma_addr);
	dma_unmap_len_set(tx_info, len, len);

	return true;
}
```

## tx completion
* when the port is started, `mqnic_start_port` will be called, which will does two things:
	* set the tx completion handler as `mqnic_tx_irq`
 	* set the completion NAPI poll function as `mqnic_poll_tx_cq`
```c
static int mqnic_start_port(struct net_device *ndev)
{
	...

	// set up TX completion queues
	for (k = 0; k < priv->tx_cpl_queue_count; k++) {
		mqnic_activate_cq_ring(priv->tx_cpl_ring[k], k % priv->event_queue_count);
		priv->tx_cpl_ring[k]->ring_index = k;
		priv->tx_cpl_ring[k]->handler = mqnic_tx_irq;

		netif_tx_napi_add(ndev, &priv->tx_cpl_ring[k]->napi,
				mqnic_poll_tx_cq, NAPI_POLL_WEIGHT);
		napi_enable(&priv->tx_cpl_ring[k]->napi);

		mqnic_arm_cq(priv->tx_cpl_ring[k]);
	}
	...
}
```
* when a packet is sent by NIC, NIC will add a `struct mqnic_cpl` to the tx completion queue:
```c
struct mqnic_cpl {
	__le16 queue;			// tx queue index
	__le16 index;			// packet index in the tx queue ring buffer
	__le16 len;
	...
};
```

## tx completion hardware IRQ

