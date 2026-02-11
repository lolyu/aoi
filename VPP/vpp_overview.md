# VPP overview

## vector processing
* cache miss is expensive
    * instruction cache
    * data cache
<img width="1040" height="598" alt="image" src="https://github.com/user-attachments/assets/a29ae5f9-6601-408c-add9-c949c260b295" />

* VPP was specifically built to solve this using Vector Processing. Instead of one packet at a time, it takes a batch (a vector) of 256 packets and runs them through the same graph node.
    * Warm Caches: When the first packet of the vector is processed, the CPU loads the necessary code and data (like the routing table) into the cache.
    * Zero-Cost for the rest: For the next 255 packets, that data is already in the cache.
* Result: You "pay" for the cache miss once, and "profit" 255 times.

* the design principle is to keep the instructions and common data in the cache and try to process more packets.
    * less instruction cache miss
    * less common data miss

* VPP is adaptive polling, no "tail latency" penalty

## references
