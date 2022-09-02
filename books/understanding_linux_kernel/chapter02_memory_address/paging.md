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
* PAE is primarily designed to allow larger physical address space, but each single process is still exposed with 2 ^ 32 = 4GB bytes virtual address space.
* with 32 bits virtual address space, only supports up to 2 ^ 32 = 4GB ram
* to supports 2 ^ 36 = 64GB ram, PAE is introduced to address up to 64GB ram with 32 bits virtual address.
* without PAE, the PTE and PDE size is 4 bytes, with 4KB page frame, each page frame could store up to 1024 entries.
    * without PAE, the page table needs total 2 ^ 20 * 4 bytes = 4MB memory space
    * for linux, it is two level page table, so page directory has 4MB / 4KB = 1024 entries, which needs 4 * 1024 = 4KB memory in total
    * to summarize, page directory needs 4KB in memory, each page table is allocated as 4KB in size.
* with PAE, the PTE and PDE size becomes 8 bytes instead of 4 bytes, so PTE and PDE now supports larger physical address.
    * with PAE, it still wants to have page directory as 4KB in size and each page table is allocated as 4KB in size.
    * but as PTE and PDE size is now changed to 8 bytes, one page frame could only holds up to half number of PTEs, and one page directory could only holds up to half number of PDEs -> one page directory could holds up to one fourth number of page frames
    * need to introduce an extra level of hierarchy of table(2 bits) needed

* **NOTE: the virtual address space of PAE is still 2 ^ 32 = 4GB, but the addressable physical addresses are extended to 64GB because of the increase of PTE.**

* NO PAE, 4KB pages

![image](https://user-images.githubusercontent.com/35479537/188088428-729e9339-52cc-4d04-99e4-ea038972bf12.png)

* NO PAE, 4MB pages

![image](https://user-images.githubusercontent.com/35479537/188088488-b3c49780-df62-45d8-88ce-78f26ccbf8b3.png)

* PAE, 4KB pages

![image](https://user-images.githubusercontent.com/35479537/188088543-899a21e3-30ce-439c-b9b0-4f21a8a5cd1e.png)

* PAE, 2MB pages

![image](https://user-images.githubusercontent.com/35479537/188088598-6a1fa09a-8554-4c38-ba82-20559ea2bca0.png)


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
