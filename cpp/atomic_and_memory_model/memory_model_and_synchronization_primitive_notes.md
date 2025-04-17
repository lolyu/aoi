# memory model and synchronization primitive

## memory reordering
* memory reordering:
    * compiler reordering
    * CPU reordering
* **NOTE**: `memory reordering shall not modify the behavior of a single-threaded program`
* the nature of this problem is that, during runtime, the reordering doesn't know if the program is single-threaded or multi-threaded.

### compiler reordering

#### Linux compiler barrier

* `barrier()`
```c
#define barrier() __asm__ __volatile__("": : :"memory")
```

* `volatile` keyword
    * in C, `volatile` informs the compiler to stop caching the variable
* `ACCESS_ONCE`
```c
#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
```
* `WRITE_ONCE` and `READ_ONCE`
```c
#define WRITE_ONCE(var, val) \
	(*((volatile typeof(val) *)(&(var))) = (val))

#define READ_ONCE(var) (*((volatile typeof(var) *)(&(var))))
```

* when to use the compiler barrier?
    * **TODO**


### CPU reordering
* CPU reordering types:
    * `StoreLoad`
    * `StoreStore`
    * `LoadStore`
    * `LoadLoad`
* due to the existence of cache, two types of cache coherence protocols are introduced: snooping-based and directory-based; to improve the snooping-based cache coherence protocol performance, store buffer and invalidate queue are introduced to avoid the CPU stall waiting for cache protocol replies, but the CPU reordering is introduced too.

![image](https://github.com/user-attachments/assets/c911697e-e6f4-4d1d-a67a-e1a9b9799d6b)


* why store buffer is introduced?
    * because the store to cache needs waiting for the invalidate reply from other CPUs if the store variable is not stored in the local cache.
    * with store buffer, the CPU doesn't need wait for the invalidate reply and directly write the data to the store buffer.
    * **store buffer allows the CPU that sends the invalidate continue to run without CPU stall waiting for invalidate acknowledge.**

![image](https://github.com/user-attachments/assets/53df9a35-7c24-4c8e-94d9-eda9ddeb9810)

* why invalidate queue is introduced?
    * store buffer is easy to fill up by writes waiting for invalidate acknowledge
    * with memory barrier, all writes after the memory barrier must be buffered in the store buffer to enforce write order, the store buffer is easier to fill up.
    * with invalidate queue, all invalidate received by one CPU will be buffered by the invalidate queue of that CPU, and the invalidate acknowledge will be sent immediately.
    * **invalidate queue reduce the time writes stored in the store buffer - better performance**

#### `StoreStore` reorder
* why `StoreStore` reorder happens?
    * for a local variable (stored in local cache, cache hit), the store operation is directly operated on the variable.
    * for a nonlocal variable (stored in other CPU's cache, cache miss), the data write is stored in the store buffer.
* so for the below program, the CPU runs it sees `a = 1` happens before `b = 1`; but other CPUs might see `b = 1` happens before `a = 1`
```c
a = 1;        // cache miss, write is stored in store buffer
b = 1;        // cache hit, write is directly to the cache
```
* how to resolve?
    * add a memory barrier to flush all pending writes in the store buffer
    * how does memeory barrier enforce write order?
        * the memory barrier is to flush the store buffer, the modified data cached in the store buffer will be updated to the cache line after the invalidate acknowledge message is received from all other CPUs.
        * during the store buffer flushing, the CPU can continue to perform the store operation after the memory barrier; the data modified after the store operation must be saved in the store buffer.

```c
a = 1;        // cache miss, write is stored in store buffer
smp_mb();
b = 1;        // cache hit, "a = 1" is in the store buffer, add "b = 1" to the store buffer
```

#### `StoreLoad` reorder
* why `StoreLoad` reorder happens?
    * for a nonlocal variable (cache miss), the data write is stored in the store buffer and CPU is able to execute the following read operation.
    * so CPU is able to execute `Load` before `Store`.

#### `LoadLoad` reorder
* why `LoadLoad` reorder happens?
    * the invalidate queue could store invalidate for the second `Load`, so the second `Load` could retrieve outdated value from the cache.
* how to resolve? - place a memory barrier between two `Load`s
    * the memory barrier will enforce all pending invalidate message to be processes before performing the second `Load`.

#### `LoadStore` reorder

## memory consistency model
* what is memory consistency model?
	* memory consistency model determines the shared-memory access order.

* key concepts:
	* program order
 	* execution order

### sequential consistency
* sequential consistency means the operations of all the processors are executed in some sequential order, and the operations of each processor appear in this sequence in the order specified by its program.
	* all processors' operations are interleaved.
 	* single processor's operations happens in "program order" (access to the same memory location).
* two required conditions:
	* maintain a single sequential order among operations from all processors
 	* maintain program order among operations from individual processors

#### sequential consistency without cache
* the sufficient conditions for the sequential memory model are:
	* each processor issues memory requests in the order specified by its program.
	* memory requests from all processors issued to an individual memory module are serviced from a single FIFO queue.
 		* issuing a memory request consists of placing the request in this queue.

#### sequential consistency with cache
* sufficient conditions of the sequential memory model applicable to the cache architecture have been updated:
	* each processor issues memory requests in the order specified by its program.
	* after a write operation is issued, the issuing processor should wait for the write to complete before issuing its next operation. - **write atomicity**
	* after a read operation is issued, the issuing processor should wait for the read to complete and for the write whose value is being returned by the read to complete before issuing its next operation. - **read atomicity**
	* write operations to the same location are serialized in the same order for all processors.
* `atomicity`:
	* `read atomicity`: the write operation will not end until the CPU receives invalidate acknowledges from all other processors
 	* `write atomicity`: the read operation is finished only when previous write operation to the same memory location is completed

### x86 consistency model
* x86 only reorders `LoadStore` operation, and this only applies to operations at different memory locations.


## references
* https://www.alibabacloud.com/blog/memory-model-and-synchronization-primitive---part-1-memory-barrier_597460
* https://www.alibabacloud.com/blog/memory-model-and-synchronization-primitive---part-2-memory-model_597461

