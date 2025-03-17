# memory latency
* memory latency is the time between a memory read/write request and the response, which is the communication from CPU to the memory devices.
* how to reduce memory latency?
   * cache alignment
   * cache prefetching

## cache alignment

## cache prefetch

* cache prefetch could improve the system performance due to the locality:
    * temporal locality: if a particular memory location is referenced, it is likely that the same location will be reference again.
    * spatial locality (memory locality): if a particular memory location is referenced, it is likely that nearby memory locations will be referenced again.

## references
* https://learn.arm.com/learning-paths/cross-platform/memory-latency/latency-and-cache-alignment/
