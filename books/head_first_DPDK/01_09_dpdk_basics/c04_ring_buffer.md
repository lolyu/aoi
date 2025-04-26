# dpdk `rte_ring`
* this note only focus on `rte_ring` implementation in `RTE_RING_SYNC_MT` and `RTE_RING_SYNC_ST` sync types.
```c
struct rte_ring {
	alignas(RTE_CACHE_LINE_SIZE) char name[RTE_RING_NAMESIZE];
	/**< Name of the ring. */
	int flags;               /**< Flags supplied at creation. */
	const struct rte_memzone *memzone;
			/**< Memzone, if any, containing the rte_ring */
	uint32_t size;           /**< Size of ring. */
	uint32_t mask;           /**< Mask (size-1) of ring. */
	uint32_t capacity;       /**< Usable size of ring */

	RTE_CACHE_GUARD;

	/** Ring producer status. */
	union __rte_cache_aligned {
		struct rte_ring_headtail prod;
	};

	RTE_CACHE_GUARD;

	/** Ring consumer status. */
	union __rte_cache_aligned {
		struct rte_ring_headtail cons;
	};

	RTE_CACHE_GUARD;
};

struct rte_ring_headtail {
	volatile RTE_ATOMIC(uint32_t) head;      /**< prod/consumer head. */
	volatile RTE_ATOMIC(uint32_t) tail;      /**< prod/consumer tail. */
	union {
		/** sync type of prod/cons */
		enum rte_ring_sync_type sync_type;
		/** deprecated -  True if single prod/cons */
		uint32_t single;
	};
};
```

## enqueue
* `rte_ring_sp_enqueue_elem` -> `rte_ring_sp_enqueue_bulk_elem`

```c
static __rte_always_inline unsigned int
__rte_ring_do_enqueue_elem(struct rte_ring *r, const void *obj_table,
		unsigned int esize, unsigned int n,
		enum rte_ring_queue_behavior behavior, unsigned int is_sp,
		unsigned int *free_space)
{
	uint32_t prod_head, prod_next;
	uint32_t free_entries;

	n = __rte_ring_move_prod_head(r, is_sp, n, behavior,
			&prod_head, &prod_next, &free_entries);

	__rte_ring_enqueue_elems(r, prod_head, obj_table, esize, n);

	__rte_ring_update_tail(&r->prod, prod_head, prod_next, is_sp, 1);

	return n;
}
```

### move producer head
* `__rte_ring_move_prod_head(r, is_sp, n, behavior, &prod_head, &prod_next, &free_entries)` ->
* `__rte_ring_headtail_move_head(&r->prod, &r->cons, r->capacity, is_sp, n, behavior, old_head, new_head, free_entries)`
    * check if enough room in the ring
    * update the producer head
```c
static __rte_always_inline unsigned int
__rte_ring_headtail_move_head(struct rte_ring_headtail *d,                // d is the producer, s is the consumer
		const struct rte_ring_headtail *s, uint32_t capacity,
		unsigned int is_st, unsigned int n,
		enum rte_ring_queue_behavior behavior,
		uint32_t *old_head, uint32_t *new_head, uint32_t *entries)
{
	unsigned int max = n;
	int success;

	do {
		/* Reset n to the initial burst count */
		n = max;

		*old_head = d->head;

		/* add rmb barrier to avoid load/load reorder in weak
		 * memory model. It is noop on x86
		 */
		rte_smp_rmb();

		*entries = (capacity + s->tail - *old_head);

		/* check that we have enough room in ring */
		if (unlikely(n > *entries))
			n = (behavior == RTE_RING_QUEUE_FIXED) ?
					0 : *entries;

		if (n == 0)
			return 0;

		*new_head = *old_head + n;
		if (is_st) {
			d->head = *new_head;
			success = 1;
		} else
			success = rte_atomic32_cmpset(
					(uint32_t *)(uintptr_t)&d->head,
					*old_head, *new_head);
	} while (unlikely(success == 0));
	return n;
}
```

### enqueue element
* `__rte_ring_enqueue_elems`

### update producer tail
* `__rte_ring_update_tail`

```c
static __rte_always_inline void
__rte_ring_update_tail(struct rte_ring_headtail *ht, uint32_t old_val,
		uint32_t new_val, uint32_t single, uint32_t enqueue)
{

	/*
	 * If there are other enqueues/dequeues in progress that preceded us,
	 * we need to wait for them to complete
	 */
	if (!single)
		rte_wait_until_equal_32((volatile uint32_t *)(uintptr_t)&ht->tail, old_val,
			rte_memory_order_relaxed);

	ht->tail = new_val;
}

static __rte_always_inline void
rte_wait_until_equal_32(volatile uint32_t *addr, uint32_t expected,
		rte_memory_order memorder)
{
	assert(memorder == rte_memory_order_acquire || memorder == rte_memory_order_relaxed);

	while (rte_atomic_load_explicit((volatile __rte_atomic uint32_t *)addr, memorder)
			!= expected)
		rte_pause();
}
```
