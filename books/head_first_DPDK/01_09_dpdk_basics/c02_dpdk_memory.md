# memory and cache

## system architecture

* SMP -> NUMA

![image](https://github.com/user-attachments/assets/6a24922a-1959-45c5-af4f-217189bd66d2)


## memory
* terms:
    * RAM
    * SRAM (static RAM)
    * DRAM (dynamic RAM)
    * SDRAM (synchronous DRAM)
    * DDR (Double Data Rate SDRAM)

## cache

### cache category

```
$ lscpu
Architecture:                       x86_64
CPU op-mode(s):                     32-bit, 64-bit
Byte Order:                         Little Endian
Address sizes:                      46 bits physical, 48 bits virtual
CPU(s):                             56
On-line CPU(s) list:                0-55
Thread(s) per core:                 2
Core(s) per socket:                 14
Socket(s):                          2
NUMA node(s):                       2
Vendor ID:                          GenuineIntel
CPU family:                         6
Model:                              85
Model name:                         Intel(R) Xeon(R) Gold 5120 CPU @ 2.20GHz
Stepping:                           4
CPU MHz:                            1130.054
CPU max MHz:                        3200.0000
CPU min MHz:                        1000.0000
BogoMIPS:                           4400.00
Virtualization:                     VT-x
L1d cache:                          896 KiB
L1i cache:                          896 KiB
L2 cache:                           28 MiB
L3 cache:                           38.5 MiB
NUMA node0 CPU(s):                  0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54
NUMA node1 CPU(s):                  1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53,55

```

![image](https://github.com/user-attachments/assets/72033b88-b30a-49d4-b16b-2df54896039b)

|cache|L1 cache|L2 cache|L3 cache (LLC - last level cache)|
|-|-|-|-|
|size|10K+|100K+|20M+|
|locality||||
|speed||||

## TLB cache
* TLB (translate look-aside buffer) cache is used to store the mapping from VA to PA

![image](https://github.com/user-attachments/assets/f6c0d6ab-a5c7-458e-b174-1c5a10801ea3)


* why not store the page table in L1/L2/L3 cache instead using TLB cache?
    * TLB cache is a fast path to get the PA
    * two reasons:
        * storing page table in L1/L2/L3 cache introduces cache miss, downgrade the CPU performance
        * storing page table in L1/L2/L3 cache still needs the PA caculation


## cache
* cache line: fixed block size for data tranferred between memory and cache

### cache entry
```
|tag|data block|flag bits|
```
* the cache entry usually contains the following structures:
    * `tag`
    * `data block`: the actual data fetched from the main memory
    * `flag bits`
* From wikipedia: **This size can be calculated as the number of bytes stored in each data block times the number of blocks stored in the cache. (The tag, flag and error correction code bits are not included in the size, although they do affect the physical area of a cache.)**


### cache placement policy

* cache placement policy：decides which cache entry is used to stores the data from memory

|cache placement policy|pros|cons|
|-|-|-|
|direct-mapped|<br>simple, both in software/hardware</br>|<br>high cache miss rate, conflict introduces cache miss</br>|
|fully-associative|<br>better cache hit rate</br>|<br>high cost of associative-comparison hardware</br>|
|set-associative|||


#### direct-mapped
![image](https://github.com/user-attachments/assets/d0e3dca0-60c4-4bd6-893c-8e19efdc5023)
* the virtual address is divided into three parts:
    * tag: the remaining address
    * index: the cache entry index
    * offset: the data offset in the cache line
* use the virtual address index field to find the cache entry, and compare the virtual address tag field with the cache entry tag, if they are the same, it means this cache entry stores the data in this VA - cache hit!.

#### fully-associated
![image](https://github.com/user-attachments/assets/1f45c57e-d8c3-40ee-a346-ea6c5963a88e)

* use the virtual address tag field to find the cache entry.

#### set-associative
* the cache is divided into N sets and each set contains M cache lines. A memory block is first **direct-mapped** onto a set and then placed into any cache line of the set in **fully-associated** way.

![image](https://github.com/user-attachments/assets/3ce01fac-2233-4e3d-be51-f42d7407d5a4)

### cache write policy
* cache write policy:
   * write-through
   * write-back
   * write-comibining
   * unreachable


## references
* https://en.wikipedia.org/wiki/Cache_placement_policies
* https://en.wikipedia.org/wiki/CPU_cache#Cache_entry_structure
