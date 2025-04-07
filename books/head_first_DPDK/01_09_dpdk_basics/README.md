# DPDK basics

## how does DPDK improve performance?
* key takeaways:
    * cache alignment
    * cache prefetch
    * avoid cache coherency protocol overhead:
        * per-core memory
        * per-core ingress/egress queue
    * avoid TLB miss
        * huge pages
    * avoid NUMA cross-node overhead
        * per-core memory
        * local device is handled by local CPUs

## how does DPDK improve memcpy performance?
* key takeaways:
    * SIMD
    * batched sequential access

## references
* https://www.cse.wustl.edu/~roger/560M.f18/CSE560-Cache3.pdf
* https://gcc.gnu.org/onlinedocs/gcc/Vector-Extensions.html
* https://www.intel.com/content/www/us/en/developer/articles/technical/performance-optimization-of-memcpy-in-dpdk.html
