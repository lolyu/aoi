# paging
* paging unit translate linear addresses into physical addresses
* page: a fixed-length contiguous block of virtual memory.
* page frame: a fixed-length block of RAM
* page tables: the data structure to map linear addresses to physical addresses


## regular paging
* 4K page = 4 * 1024 = 2 ^ 12 bytes page, so offset should have 12 bits
* virtual address space: 2 ^ 32, there will be 2 ^ 32 / 4K = 2 ^ 20 pages, VPN should be of 20 bits
* linux uses multi-level paging
* the 32 bits address is divided into three fields:
    * directory: 10 bits
    * table: 10 bits
    * offset: 12 bits
* **because each page frame has 4KB capacity, its physical address must be a multipe of 4096, so the 12 least significant bits of physical address are always equal to 0.**
* each process has a page table, this is a waste
    * no need to allocate a full table at once, divide the page table by page, if there is a valid PTE inside the page, allocate the page frame, and store the page frame address into page directory
* summary:
    * the virtual address spaces determiens how many bits the address should use
    * the page frame size determines how many bits the offset should use, the left is for VPN
    * for a VPN, the PTE size determines how many bits in VPN should be used as PTE index inside a page frame

![image](https://user-images.githubusercontent.com/35479537/178130512-b1b0c85c-9e0b-4c97-a64c-f89030d202d3.png)

![image](https://user-images.githubusercontent.com/35479537/178130556-45276100-ba57-4e19-95a9-146e792b028a.png)

* PDE(page directory entry) stores the PFN of the allocated page frame storing PTEs, to get the actual PTE:
    * `PTE address = PDE PFN << offset + PTE index * sizeof(PTE)`


## extended paging
* extended paging: page frames to be 4MB instead of 4KB in size
    * offset needs 22 bits
    * VPN needs 10 bits
    * page table is removed
* the 32 bits address is divided into two fields:
    * directory
    * offset


![image](https://user-images.githubusercontent.com/35479537/178135114-df099161-80cd-490e-bb6f-b59fc8f239f7.png)

## PAE: physical address extension
* with 32 bits virtual address space, only supports up to 2 ^ 32 = 4GB ram
* to supports 2 ^ 36 = 64GB ram, PAE is introduced to address up to 64GB ran with 32 bits virtual address.
* 36 bits virtual address space, with 4KB page size, the VPN should take 24 bits, while offset takes 12 bits
    * with 4KB page size, 64GB ram will have 2 ^ 36 / 2 ^ 12 = 2 ^ 24 page frames
* PTE extends from 4 bytes to 8 bytes
    * page table should take 8 * (2 ^ 24) = 2 ^ 28 bytes space, uses 2 ^ 28 / 2 ^ 10 = 2 ^ 18 page frames
    * one page frame(4KB) could have 4 * 1024 / 8 = 512 PTEs, so 9 bits out of VPN should be used as PTE index


## hardware cache
![image](https://user-images.githubusercontent.com/35479537/178137953-560661e7-8f71-4c66-8b74-3f0bd684206c.png)

* when accessing a RAM memory cell, the CPU first checks if cache has it, if yes, the CPU has a `cache hit`
    * for a read operation, the cache controller selects the data from the cache and transfers it to a CPU register
    * for a write operation, the cache controller has two basic strategies:
        * `write-through`: the cache controller writes the update into both RAM and the cache
        * `write-back`: the cache controller writes only to the cache, and the RAM is left unchanged
            * the cache controller writes the cache back into RAM only when:
                * CPU executes an instruction requiring a cache flush
                * a `FLUSH` hardware signal occurs(usually after a cache miss)
