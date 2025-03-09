# DPDK memory

## memory issues in packet processing

|issue|description|how DPDK solves|
|-|-|-|
|cache consistency|Contention between CPU if two data structures are stored in one cache line|cache line alignment|
|false sharing|||
|TLB miss|DPDK process data up to tens of GB, TLB misses are expensive operations|huge pages|
|NUMA locality|local memory access is faster than non-local memory|pinning memory to NUMA nodes, every DPDK API has explicit NUMA awareness|
|memory access slow||DDIO|

## huge page
* huge page is used to reduce TLB miss
* two ways to enable the system to manager large amounts of memory:
    * increase the page table entry number in hardware MMU
        * this is expensive, and if an application needs to use more memory pages than the MMU supports, the system falls back to slower software based MMU.
    * increase the page size
        * either 2MB or 1GB page size


### how to configure
* `/proc/sys/vm/nr_hugepages` indicates the current huge page number
* `/proc/meminfo` contains information about the huge page statistics

* edit `/etc/default/grub` to add the following text to the end of the file:

```
GRUB_CMDLINE_LINUX_DEFAULT="${GRUB_CMDLINE_LINUX_DEFAULT} default_hugepagesz=2MB hugepagesz=1G hugepages=2 hugepagesz=2M hugepages=20"
```
* save and update the grub:
```
# update-grub
```

```
$ cat /proc/meminfo
...
HugePages_Total:      20
HugePages_Free:       20
HugePages_Rsvd:        0
HugePages_Surp:        0
Hugepagesize:       2048 kB
Hugetlb:         2138112 kB
...
# cat /sys/kernel/mm/hugepages/hugepages-1048576kB/nr_hugepages
2
# cat /sys/devices/system/node/node0/hugepages/hugepages-1048576kB/nr_hugepages
2
# cat /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
20
# cat /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages
20
# mount -t hugetlbfs nodev /mnt/huge -o pagesize=1G
# mount -t hugetlbfs nodev /mnt/huge_2m -o pagesize=2MB
# dpdk-hugepages.py -s
Node  Pages  Size  Total
0        64   2Mb  128Mb
0         2   1Gb    2Gb

Huge page filesystems mounted at: /mnt/huge /mnt/huge_2m /dev/hugepages
```

## DPDK hugepage management
* the hugepage's virtual memory address space is managed based on `memory type` -> `segment list` -> `segment`
* DPDK get hugepages from `/sys/kernel/mm/hugepage` and `/proc/mounts`

![image](https://github.com/user-attachments/assets/0f1b7b00-f2b2-413f-a285-43137883d378)

* if the application needs 3000MB memory, all in NUMA node 0, two 1G segments will have physical address mappings and 476 2M segments will have physical address mappings.

![image](https://github.com/user-attachments/assets/c67cbbbb-7806-44bf-955c-83986468c932)

### heap
* each NUMA node has a heap to manage objects, it is similar to the Linux buddy allocator
```
Example element size ranges for a heap with five free lists:
heap->free_head[0] - (0 , 2^8]
heap->free_head[1] - (2^8 , 2^10]
heap->free_head[2] - (2^10 ,2^12]
heap->free_head[3] - (2^12, 2^14]
heap->free_head[4] - (2^14, MAX_SIZE]
```

## mempool and local cache

![image](https://github.com/user-attachments/assets/ee239f81-38a8-48a0-94ef-7b57c2b44d2e)


## references
* https://www.intel.com/content/www/us/en/docs/programmable/683840/1-2-1/enabling-hugepages.html
* https://docs.kernel.org/admin-guide/mm/hugetlbpage.html
* https://mp.weixin.qq.com/s/ZVgkB0TDVJ_3achH5UTcLQ
