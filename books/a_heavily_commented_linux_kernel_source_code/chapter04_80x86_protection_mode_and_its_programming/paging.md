# paging
* paging operates on fixed-size blocks of memory(pages), it divides the linear and physical address spaces into pages.
* Any page in the linear address space can be mapped to any page in the physical address space.
* The paging mechanism operates by relocating pages in a linear address space into physical address space. Since a 4K page is mapped as a unit and aligned to a 4K boundary, the lower 12 bits of the linear address can be used as the in-page offset directly as the lower 12 bits of the physical address. The relocation function performed by the paging mechanism can be seen as converting the upper 20 bits of the linear address(VPN) to the upper 20 bits of the corresponding physical address(PFN).

* linear address is consisted of VPN and offset
    * `VPN`: virtual page number, the index of which page the address is within
    * offset: the offset of the address within a page
* physical address is consisted of PFN and offset
    * `PFN`: page frame number, the index of which page frame the address is within
    * offset: the offset of the address within a page frame

![image](https://user-images.githubusercontent.com/35479537/227755265-e205fade-9959-4108-bcbc-9a874284f243.png)

* in order to reduce the address translations, the most recently accessed page directory and page table are stored in a processor buffer, known as the translation lookaside buffer(TLB).


## page table
* page table is used to map `VPN` to `PFN`
* the linear address space is 2^32, the page size is 4KB, so there are 2 ^ 32 / 4 * (2^10) = 2 ^ 20 pages
* `PTE`: page table entry, used to store the `PFN` corresponding to the `VPN`, it is 4 bytes(32 bits), 20 bits are used to store the `PFN`, 12 bits are used to store page attributes
    * so the page table needs (2 ^ 20) * 4 = 2 ^ 22 bytes = 4MB physical memory

### page table size issue
* with page table of 4MB size, not all of the pages are allocated and used(sparse page table)

### two-level page table
* how to reduce page table size?
    * store the page table into separated pages(doesn't need a continous block of memory)
        * so total 4MB / 4KB = 2 ^ 10 pages are needed
        * one page could store 4KB / 4 = 1KB `PTE`s, so to address the `PTE` inside one page, needs 10 bits out of the 32-bits address
    * use a new page table(page directory) to store the addresses of those 2 ^ 10 pages
        * with 4 bytes `PTE`, need 4KB memory space, so one page is enough to store the page directory
        * to address th 2 ^ 10 entries inside the page directory, needs 10 bits out of the 32-bits address
    * so page table is allocated with page size, only allocated ones have entries inside the page directory 

![image](https://user-images.githubusercontent.com/35479537/227756318-f14354fd-b38c-40ba-bf61-e1f6d03ad445.png)




## references
