# DPDK basics

## how does DPDK improves performance?
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

## references
* https://www.cse.wustl.edu/~roger/560M.f18/CSE560-Cache3.pdf
