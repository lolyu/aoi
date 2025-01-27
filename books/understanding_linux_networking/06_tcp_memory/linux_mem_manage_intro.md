# Linux memory manage introduction

* what is `node`?
    * in `NUMA` system, a `node` is a collection of CPUs and memory that are physically close to each other.
    * memory access is faster for local CPUs compared to non-local CPUs in different nodes.
    * nodes are connected via bus.
* what is `zone`?
    * `zone` is a range of physical memory that has a specific restriction:
        * `ZONE_DMA`
        * `ZONE_DMA32`
        * `ZONE_NORMAL`
        * `ZONE_HIGHMEM`

## the buddy allocator

## references
*
