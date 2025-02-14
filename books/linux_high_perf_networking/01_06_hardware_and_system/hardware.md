# hardware

## CPU
![image](https://github.com/user-attachments/assets/68a4ae58-3bd2-4b96-b46f-0bdfee899a13)

## cache
![image](https://github.com/user-attachments/assets/d7d42888-697b-49b5-8fd0-a83076aed9a7)

* L1 cache has two types:
    * L1 data cache: D-cache
    * L1 instruction cache: I-cache

### cache line
* CPU caches transfer data from and to main memory in chunks called `cache line`
* `cache line` aligned structs have better performance.

![image](https://github.com/user-attachments/assets/b4077f27-ba28-48c6-a626-6f10bcbbaccd)

### cache write policy
* write policy:
    * write through
        * write the data into cache/memory the same time
        * coherence
        * unneccesary writes to the memory
    * write back
        * write the data into cache, and mark the cache line as `DIRTY`; when the `DIRTY` cache line will be overwritten, it will be flushed to memory
        * incoherence

### cache coherence
* two solutions:
    * snooping-based
        * each cache controller are connected to a shared bus; any cache operation is broadcasted.
    * directory-based
        * the data being shared is placed in a common directory that maintains the coherence between caches.

### cache operations
* cache operations:
    * `clean`: flush `DIRTY` cache lines to memory
    * `invalid`: invalid cache lines; next time CPU accesses this address, it will retrieve from memory
    * `flush`: `clean` first, then `invalid`

## NUMA (non-uniform memory access)
* `SMP`: symmetric multi processing
    * all processors share the same main memory and system bus.
    * contention issues on the system bus as the number of processors increases

![image](https://github.com/user-attachments/assets/1806dd30-b155-491c-9266-7acabab03558)
* QPI: quick path interconnect

## references
* https://lwn.net/Articles/252125/
