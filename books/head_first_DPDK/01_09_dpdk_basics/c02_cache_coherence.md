# cache coherence
* The cache coherence problem arises when multiple caches store copies of the same data, and modifications made by one cache need to be propagated to other caches. Failure to maintain cache coherency can result in data corruption and incorrect program behavior.


## cache coherence protocols
* cache coherence protocols are used to govern how caches communicate and coordinate the operations to ensure data correctness.
    * bus snooping protocols
    * directory based protocols

## false sharing
* `false sharing` is the overhead introduced by the cache coherence protocols:
    * when a system participant attempts to periodically access data that is not being altered by another party, but that data shares a cache line with data that is being altered, the caching protocol may force the first participant to reload the whole cache block despite a lack of logical necessity.

* how to avoid?
    * padding the struct/data to be multiple of cache line size.


## how does DPDK avoid cache coherence overhead?
* cache coherence protocol introduces overhead, and DPDK aims to avoid this overhead by avoid sharing data with other cores.

1. each core has its own copy of critical data structures, and those data structures are cache-line-aligned:
```c++
struct __rte_cache_aligned lcore_conf {
	uint16_t n_rx_queue;
	struct lcore_rx_queue rx_queue_list[ETHDEV_RX_QUEUE_PER_LCORE_MAX];
	struct rte_graph *graph;
	char name[RTE_GRAPH_NAMESIZE];
	rte_graph_t graph_id;
};

struct lcore_conf lcore_conf[RTE_MAX_LCORE];
```
2. each port has multiple rx/tx queues, and traffic on each queue is handled by different CPU.
