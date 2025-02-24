# RX

![image](https://github.com/user-attachments/assets/cf5bc0ea-7667-46e4-8095-67da87eac899)

* steps:
    1. when the packet in the rx queue is forwarded to the kernel network procotol stack, the buffer ownership to store the packet will be handed over to the kernel network procotol stack, and the NIC driver will allocate new buffers to the rx queue.
        * the descriptors between rx queue head ptr and tail ptr are those free to be used by the NIC to store incoming rx packets.
        * when the buffers are refilled, rx queue software head ptr is incremented, and hardware head ptr will be updated to notify the NIC that the buffers are ready.
    2. NIC finds its rx queue hardware head ptr != rx queue hareware tail ptr -> buffers are ready -> NIC starts to receive packets
    3. NIC receives one packet, and adds the packet related information to the completion queue
    4. NIC enqueues a rx finish event to the event queue and raise hardware IRQ
    5. CPU receives the IRQ and calls its handler to process the events in the event queue
    6. the hardware IRQ handler calls the corresponding RX completion queue's handler - `mqnic_rx_irq`, and `mqnic_rx_irq` in turn raise soft IRQ to delegate the rx completion queue handle job to the ksoftirqd
    7. the rx completion queue poll function `mqnic_poll_rx_cq` (called by ksoftirqd) will retrieve rx completion event from the rx completion queue.
    8. NIC driver hands the packet to the kernel network protocol stack and allocate new buffers to the rx queue -> go back to 1.
