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
### process page table

## references
* https://lwn.net/Articles/106177/
* https://www.kernel.org/doc/gorman/html/understand/understand001.html
* https://carteryagemann.com/pid-to-cr3.html
* https://www.kernel.org/doc/gorman/html/understand/understand006.html
