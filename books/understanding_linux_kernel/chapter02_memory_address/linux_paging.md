# linux paging
* Linux adopted a four-level paging model:
    * page global directory
    * page upper directory
    * page middle directory
    * page table

![image](https://user-images.githubusercontent.com/35479537/178270006-fd9dddb5-574c-4193-88c9-5c4875facc65.png)

* when a process is executed on an x86 CPU, the CR3 register is loaded with the physical address of that process's page global directory (PGD). This is necessary so the CPU can perform translations from virtual memory address to physical memory addresses. Since every process needs its own PGD, the value in the CR3 register will be unique for each scheduled process in the system. 

* for 32-bit architecture with no PAE, two page levels are used: page global directory and page table.
    * page upper directory and page middle directory are eliminated
* for 32-bit architecture with PAE, three page levels: page global directory, page middle directory and page table
    * page global directory as PDPT(page directory pointer table)
    * page upper directory is eliminated
* for 64-bit architecture, three or four levels of paging are used

![image](https://user-images.githubusercontent.com/35479537/188252339-84627342-a639-4367-9f7f-58814e6df468.png)

## physical memory layout
* during the initialization phase, the kernel must build a physical addresseses map
    * the physical addresses map specifies which address ranges could be used by the kernel, which are not.
    * unusable physical addresses:
        * memory that maps hardware devices I/O
        * kernel code and data structures
        * BIOS data
* in general, the kernel starts at RAM physical address `0x00100000`, which is the second megabyte.
    * first megabyte is often reserved for BIOS routines
* a typical configuration yields a kernel that can be loaded in less than 3 MB of RAM

* kernel boot up sequence
    * query BIOS and learn the size of physical memory
    * invokes a BIOS procedure to build a list a physical address ranges and their memory types
    * `machine_specific_memory_setup()`
        * builds the physical addresses map
    * `setup_memory`
        * analyze the table of physical memory regions and initializes kernel's physical memory layout description variables

![image](https://user-images.githubusercontent.com/35479537/188266596-2d7e088c-49b4-4e6d-ae43-6839cee3d8f0.png)


### process page table
* the virtual address space of a process
    * `0x00000000` to `0xbfffffff`: can be addressed when the process runs in either `User` or `Kernel` mode
    * `0xc0000000` to `0xffffffff`: can be addressed only when the process runs in the `Kernel` mode
* without PAE, one PDE(page global directory entry) maps to (2 ^ 12) * (2 ^ 10) = 2 ^ 22 bytes virtual memory
    * so in the page global directory, first 0xc0000000 / (2 ^ 22) = 768 entries are used to map virtual addresses lower than `0xc0000000`
* with PAE, one page global directory entry maps to 2 ^ 30 bytes virtual memory
    * so in the page global directory, first 0xc0000000 / (2 ^ 30) = 3 entries are used to map virtual addresses lower than `0xc0000000`
* **the page global directory entries higher than `0xc0000000` are the same for all processes and equal to the corresponding entries of the master kernel global directory table.**


### kernel page table
* how does kernel initialize its page tables?
    1. the kernel creates a limited address space including the kernel's code and data segments, the initial page tables and 128KB for some dynamic data structures.
        * let the kernel to install in RAM and initialize its core data structures
        * operates in real mode in this stage
    2. the kernel takes advantage of all of the existing RAM and sets up the page tables properly
        * very first stage of paging

* provisional kernel page tables
    * the objective of this phase of paging is to allow the 8MB of RAM to be addressed in both CPU real mode and protected mode
    * 8MB: `0x0` to `0x7fffff`, needs two page tables
        * one page table could reference (4KB / 4) * 4KB = 4MB virtual memory 
    * those two page tables are stored right after the end of the kernel's uninitialized data segments(`pg0`)
    * in real mode, the kernel must maps from virtual address `0x00000000` ~ `0x007fffff` into physical address `0x00000000` ~ `0x007fffff`
    * in protected mode, the kernel must maps from virtual address `0xc0000000` ~ `0xc07fffff` into physical address `0x00000000` ~ `0x007fffff`
        * this is because only page directory entries higer than `0xc00000000` are used for kernel only.
* so the initial provisional kernel page tables are filled with zeroes except for entries 0, 1, 0x300(768) and 0x301(769)
    * 0 and 1 are used in real mode
    * 768 and 769 are used in protected mode
    * 0 and 768 are set to store the physical address of `pg0`
    * 1 and 769 are set to store the physical address of the page frame following `pg0`

## references
* https://lwn.net/Articles/106177/
* https://www.kernel.org/doc/gorman/html/understand/understand001.html
* https://carteryagemann.com/pid-to-cr3.html
* https://www.kernel.org/doc/gorman/html/understand/understand006.html
