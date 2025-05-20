# NiC performance optimization with DPDK

## how does kernel receives packet?
![image](https://github.com/user-attachments/assets/6d99c674-cc6d-4b16-b1ac-e622b1f5add0)

* the interrupt handling introduces context switch overhead

## how does DPDK work?

![image](https://github.com/user-attachments/assets/a1424cd8-7b2a-4f61-800e-2f1a0c195b6e)


## how does DPDK optimize packet forwarding?
* the principle: coalesce operations

### burst send/receive API
```c
static inline uint16_t
rte_eth_rx_burst(uint16_t port_id, uint16_t queue_id,
		 struct rte_mbuf **rx_pkts, const uint16_t nb_pkts)

static inline uint16_t
rte_eth_tx_burst(uint16_t port_id, uint16_t queue_id,
		 struct rte_mbuf **tx_pkts, uint16_t nb_pkts)
```

* burst API could send/receive multiple packets in one call:
    * leverage the cache prefetch to increase the cache hit

### CPU latency hiding and bulk operations
* benchmarking indicators:
    * latency: how many CPU cycles needed to process an instruction
    * throughput: how many instructions can be finished within a time unit

<img width="454" alt="image" src="https://github.com/user-attachments/assets/45df28a4-40ee-4e07-9814-251ac72d91b5" />

* bulk sending/receivement reduces the memory access with memory prefetch.

