# cache coherence
* cache line is the cache data store unit, as each CPU has it's own cache, cache coherence issue arises:
    * data structure might not be cache line aligned, it might occupies two cache lines, which could be modified by different CPU cores
    * cache-line-aligned data structure might be written back by multiple CPUs.

![image](https://github.com/user-attachments/assets/2ba449e4-f682-4e14-ba3b-c5bf49b4e321)
