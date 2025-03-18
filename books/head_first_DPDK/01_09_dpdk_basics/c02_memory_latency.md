# memory latency
* memory latency is the time between a memory read/write request and the response, which is the communication from CPU to the memory devices.
* how to reduce memory latency?
   * cache alignment
   * cache prefetching

## memory alignment
* better aligned struct helps the memory read/write better fits into one cache line.
* how does alignment to cache line helps:
    * avoid `false sharing`
        * the struct shares the same cache line with other data, and is loaded into different CPU cores; the writes to the other data from one CPU core will invalidate this cache line on another CPU core.
    * avoid the struct to spread across multiple cache lines.
        * 118 bytes struct can spread across 3 cache line (64 bits)

* how to do?
    * `posix_memalign`
    * `__attribute__((aligned(64)))`

## cache prefetch

* cache prefetch could improve the system performance due to the locality:
    * temporal locality: if a particular memory location is referenced, it is likely that the same location will be reference again.
    * spatial locality (memory locality): if a particular memory location is referenced, it is likely that nearby memory locations will be referenced again.
*  Cache prefetching is a way to instruct the CPU to prefetch data to arrive in the L1 cache so that it is already close to the CPU when needed. Keep in mind that typical RAM latency is about 100 ns, so it is not sufficient to just prefetch the data that will be processed in the next iteration of a loop, **it needs to be done a few iterations before**.
* best practices:
    * prefetch the data that will be used in the iterations ahead, not just for the data needed immediately before use
    * prefetching tells the CPU to prefetch data once, better to do this in a loop

## references
* https://learn.arm.com/learning-paths/cross-platform/memory-latency/latency-and-cache-alignment/
