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

* variables describing the kernel physical memory layout

|variable name|description|
|-|-|
|num_phy

![image](https://user-images.githubusercontent.com/35479537/188266596-2d7e088c-49b4-4e6d-ae43-6839cee3d8f0.png)


### process page table
* the virtual address space of a process
    * `0x00000000` to `0xbfffffff`: can be addressed when the process runs in either `User` or `Kernel` mode
    * `0xc0000000` to `0xffffffff`: can be addressed only when the process runs in the `Kernel` mode
        * the kernel address space is divided into low memory and high memory
        * addresses in the low memory could be statically mapped at the kernel boot time
        * addresses in the high memory could not
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

#### final kernel page table
* `paging_init()` initialize the kernel page global directory stored in `swapper_pg_dir`:
    * invokes `pagetable_init()` to set up the page table entries
    * write the physical address of `swapper_pg_dir` into `cr3`
    * set `PAE` flag in the `cr4` control register if `PAE` is enabled
    * invokes `__flush_tlb_all()` to invalidate all TLB entries.

* because kernel reserves the last 128MB virtual memory space for other mappings, so the virtual address space is at most `0xc0000000` ~ `0xffff7fff`, total 896MB

* `pagetable_init()`
* if the physical memory is less than 896MB
```c
    pgd = swapper_pg_dir + pgd_index(PAGE_OFFSET); /* 768 */
    phys_addr = 0x00000000;
    while (phys_addr < (max_low_pfn * PAGE_SIZE)) {
        pmd = one_md_table_init(pgd); /* returns pgd itself */
        set_pmd(pmd, _ _pmd(phys_addr | pgprot_val(_ _pgprot(0x1e3))));
        /* 0x1e3 == Present, Accessed, Dirty, Read/Write,
                Page Size, Global */
                phys_addr += PTRS_PER_PTE * PAGE_SIZE; /* 0x400000 */
         ++pgd;
  }
```
* if the physical memory is between 896MB and 4096MB
    * kernel could only maps a section of physical memory, needs dynamic remapping to access other parts of RAM
* if the physical memory is larger than 4096MB
    * **Question**
        * the kernel virtual address space could not address all the physical addresses with PAE enabled in such case?
        * so is dynamic remapping still needed?
```c
    pgd_idx = pgd_index(PAGE_OFFSET); /* 3 */
    for (i=0; i<pgd_idx; i++)
        set_pgd(swapper_pg_dir + i, _ _pgd(_ _pa(empty_zero_page) + 0x001));            // initialize first three entries in the page global directory(user linear address space) with the address of an empty page
        /* 0x001 == Present */
    pgd = swapper_pg_dir + pgd_idx;
    phys_addr = 0x00000000;
    for (; i<PTRS_PER_PGD; ++i, ++pgd) {
        pmd = (pmd_t *) alloc_bootmem_low_pages(PAGE_SIZE);                             // allocate a page middle directory
        set_pgd(pgd, _ _pgd(_ _pa(pmd) | 0x001)); /* 0x001 == Present */
        if (phys_addr < max_low_pfn * PAGE_SIZE)
            for (j=0; j < PTRS_PER_PMD /* 512 */
                  && phys_addr < max_low_pfn*PAGE_SIZE; ++j) {
                set_pmd(pmd, _ _pmd(phys_addr |
                               pgprot_val(_ _pgprot(0x1e3))));
                /* 0x1e3 == Present, Accessed, Dirty, Read/Write,
                        Page Size, Global */
                phys_addr += PTRS_PER_PTE * PAGE_SIZE; /* 0x200000 */
          }
    }
    swapper_pg_dir[0] = swapper_pg_dir[pgd_idx];
```

## references
* https://lwn.net/Articles/106177/
* https://www.kernel.org/doc/gorman/html/understand/understand001.html
* https://carteryagemann.com/pid-to-cr3.html
* https://www.kernel.org/doc/gorman/html/understand/understand006.html
* https://unix.stackexchange.com/questions/4929/what-are-high-memory-and-low-memory-on-linux#:~:text=The%20High%20Memory%20is%20the,its%20own%20address%20space%20first.
