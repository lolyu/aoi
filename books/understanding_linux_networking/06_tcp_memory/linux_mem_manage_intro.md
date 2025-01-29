# Linux memory manage introduction

* what is `node`?
    * in `NUMA` system, a `node` is a collection of CPUs and memory that are physically close to each other.
    * memory access is faster for local CPUs compared to non-local CPUs in different nodes.
    * nodes are connected via bus.

```
# lscpu
Architecture:             x86_64
  CPU op-mode(s):         32-bit, 64-bit
  Address sizes:          40 bits physical, 48 bits virtual
  Byte Order:             Little Endian
CPU(s):                   4
  On-line CPU(s) list:    0-3
...
NUMA:
  NUMA node(s):           1
  NUMA node0 CPU(s):      0-3
```

* what is `zone`?
    * `zone` is a range of physical memory that has a specific restriction:
        * `ZONE_DMA`
        * `ZONE_DMA32`
        * `ZONE_NORMAL`
        * `ZONE_HIGHMEM`

```
# cat /proc/zoneinfo
Node 0, zone      DMA
  pages free     3584
        boost    0
        min      14
        low      17
        high     20
        spanned  4095
        present  3743
        managed  3584
        cma      0
        protection: (0, 3503, 15957, 15957, 15957)
Node 0, zone    DMA32
  pages free     888101
        boost    0
        min      3681
        low      4601
        high     5521
        spanned  1044480
        present  913355
        managed  896971
        cma      0
        protection: (0, 0, 12453, 12453, 12453)
Node 0, zone   Normal
  pages free     1773789
        boost    0
        min      13199
        low      16498
        high     19797
        spanned  3272704
        present  3272704
        managed  3188192
        cma      0
        protection: (0, 0, 0, 0, 0)
Node 0, zone  Movable
  pages free     0
        boost    0
        min      0
        low      0
        high     0
        spanned  0
        present  0
        managed  0
        cma      0
        protection: (0, 0, 0, 0, 0)
Node 0, zone   Device
  pages free     0
        boost    0
        min      0
        low      0
        high     0
        spanned  0
        present  0
        managed  0
        cma      0
        protection: (0, 0, 0, 0, 0)
```

## the buddy allocator

![image](https://github.com/user-attachments/assets/3e07a882-cabe-4775-a6fe-818127e37b4d)

* eleven orders from 0 ~ 11 (4KB to 4 * 1024KB)
* the following shows a system that suffers from memory fragmentation:
    * the memories 

```
# cat /proc/pagetypeinfo
Page block order: 9
Pages per block:  512

Free pages count per migrate type at order       0      1      2      3      4      5      6      7      8      9     10
Node    0, zone      DMA, type    Unmovable      0      0      0      0      0      0      0      0      1      0      0
Node    0, zone      DMA, type      Movable      0      0      0      0      0      0      0      0      1      2      2
Node    0, zone      DMA, type  Reclaimable      0      0      0      0      0      0      0      0      0      0      0
Node    0, zone      DMA, type   HighAtomic      0      0      0      0      0      0      0      0      0      0      0
Node    0, zone      DMA, type      Isolate      0      0      0      0      0      0      0      0      0      0      0
Node    0, zone    DMA32, type    Unmovable      0      1      0      1      1      0      1      0      1      0      7
Node    0, zone    DMA32, type      Movable      1      1      0      1      0      0      1      1      1      1    859
Node    0, zone    DMA32, type  Reclaimable      0      0      0      0      0      0      0      0      0      0      0
Node    0, zone    DMA32, type   HighAtomic      0      0      0      0      0      0      0      0      0      0      0
Node    0, zone    DMA32, type      Isolate      0      0      0      0      0      0      0      0      0      0      0
Node    0, zone   Normal, type    Unmovable     62    273     41    130     45     25     17      5      3      1    103
Node    0, zone   Normal, type      Movable  10531  18222   9941   5397   2338    853    321    191     43      3   1372
Node    0, zone   Normal, type  Reclaimable     87     83     75     54     33     15      2      2      1      0      0
Node    0, zone   Normal, type   HighAtomic      0      0      0      0      0      0      0      0      0      0      0
Node    0, zone   Normal, type      Isolate      0      0      0      0      0      0      0      0      0      0      0

Number of blocks type     Unmovable      Movable  Reclaimable   HighAtomic      Isolate
Node 0, zone      DMA            1            7            0            0            0
Node 0, zone    DMA32           32         1752            0            0            0
Node 0, zone   Normal          501         5811           80            0            0
```

## the slab allocator

```
# cat /proc/slabinfo
slabinfo - version: 2.1
# name            <active_objs> <num_objs> <objsize> <objperslab> <pagesperslab> : tunables <limit> <batchcount> <sharedfactor> : slabdata <active_slabs> <num_slabs> <sharedavail>
nf_conntrack         877   1250    320   25    2 : tunables    0    0    0 : slabdata     50     50      0
kvm_async_pf           0      0    136   30    1 : tunables    0    0    0 : slabdata      0      0      0
kvm_vcpu               0      0   7216    4    8 : tunables    0    0    0 : slabdata      0      0      0
kvm_mmu_page_header      0      0    184   22    1 : tunables    0    0    0 : slabdata      0      0      0
x86_emulator           0      0   2656   12    8 : tunables    0    0    0 : slabdata      0      0      0
kcopyd_job             0      0   3240   10    8 : tunables    0    0    0 : slabdata      0      0      0
dm_uevent              0      0   2888   11    8 : tunables    0    0    0 : slabdata      0      0      0
fuse_request           0      0    152   26    1 : tunables    0    0    0 : slabdata      0      0      0
fuse_inode             0      0    832   19    4 : tunables    0    0    0 : slabdata      0      0      0
ext4_groupinfo_4k    704    704    184   22    1 : tunables    0    0    0 : slabdata     32     32      0
ext4_fc_dentry_update      0      0     96   42    1 : tunables    0    0    0 : slabdata      0      0      0
ext4_inode_cache    7994   8046   1192   27    8 : tunables    0    0    0 : slabdata    298    298      0
# slabtop
Active / Total Objects (% used)    : 555531 / 696181 (79.8%)
 Active / Total Slabs (% used)      : 20638 / 20638 (100.0%)
 Active / Total Caches (% used)     : 125 / 172 (72.7%)
 Active / Total Size (% used)       : 141134.09K / 165705.48K (85.2%)
 Minimum / Average / Maximum Object : 0.01K / 0.24K / 8.00K

  OBJS ACTIVE  USE OBJ SIZE  SLABS OBJ/SLAB CACHE SIZE NAME
104601  68252  65%    0.19K   4981       21     19924K dentry
 96512  61607  63%    0.03K    754      128      3016K lsm_inode_cache
 50016  49476  98%    0.12K   1563       32      6252K kernfs_node_cache
 36556  35958  98%    0.15K   1406       26      5624K vm_area_struct
 34560  25893  74%    0.06K    540       64      2160K kmalloc-64
 30900  27563  89%    0.62K   1236       25     19776K inode_cache
 26752  18679  69%    0.03K    209      128       836K kmalloc-32
 23552  21106  89%    0.06K    368       64      1472K anon_vma_chain
 19404  13945  71%    0.19K    924       21      3696K kmalloc-192
 18848  16906  89%    0.12K    589       32      2356K pid
 17664   5952  33%    0.06K    276       64      1104K vmap_area
 16128  15719  97%    0.02K     63      256       252K kmalloc-16
 15596   9896  63%    0.57K    557       28      8912K radix_tree_node
 13600  12393  91%    0.02K     80      170       320K lsm_file_cache
 13288  13288 100%    0.71K    604       22      9664K ovl_inode
 13040  12317  94%    0.25K    815       16      3260K filp
 11310  10758  95%    0.10K    290       39      1160K anon_vma
  9216   8247  89%    0.01K     18      512        72K kmalloc-8
```

## references
* https://www.kernel.org/doc/Documentation/sysctl/vm.txt
* https://www.kernel.org/doc/gorman/html/understand/understand011.html
* https://www.kernel.org/doc/gorman/html/understand/understand009.html
