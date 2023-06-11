# multitask and protection
![image](https://github.com/lolyu/aoi/assets/35479537/fe5e6dcf-13aa-4388-bc55-0344dec5e753)

## mapping between virtual addresses, linear addresses and physical addresses

### kernel
![image](https://github.com/lolyu/aoi/assets/35479537/fd833aff-147d-429a-98f4-f8358b6c182f)

* for linux 0.12, both the kernel code segment and data segment are set to an area of length 16MB(`0x0` ~ `0xFFFFFF`)
    * this 16MB memory space contains the kernel code, kernel segment tables(`GDT`, `IDT`, `TSS`), page directory tables and page tables, kernel local data, and kernel temporary stack(used as user stack for task 0)
    * this 16MB memory space uses 16MB / 1024 / 4KB = 4 `PDE`s(page directory entries)
* the kernel code and data segments are mapped to the same linear address space and physical address space.
* linux 0.12 kernel can manage 16MB physical memory by default, so 4096 page frames(4KB)
    * kernel code and data segment are mapped to the same area in the linear address space, and this area in the linear address space is mapped to the same range in the physical address space.

![image](https://github.com/lolyu/aoi/assets/35479537/9b9040b2-a86f-41a2-a183-5ad740133e4e)

### task0
![image](https://github.com/lolyu/aoi/assets/35479537/fad005b0-28e4-4b1e-a9e6-74f4800c714a)

* for task 0:
    * its code segment and data segment both starts from 0, but are limited to 640KB
        * so it share the same mapping from linear address space to the physical address space


### task1
![image](https://github.com/lolyu/aoi/assets/35479537/f065b600-ed77-4e24-8cda-4d732dbd5deb)


### other tasks
![image](https://github.com/lolyu/aoi/assets/35479537/576d83df-8117-431f-982f-d5985c647619)
* for other tasks(pid >= 2), its parent task is always `init`(task 1).
* When the code of task 2 (init()) calls the execve() system call to start loading and executing the shell program, the system call releases the page directory and page table entries and corresponding memory pages copied from task 1. Then re-create the relevant page directory and page table entries for the new executor shell


### memory dynamic allocation for user application
* `malloc()` is used for application user to apply for memory, and as long as the task execution is within the 64MB range, the kernel will automatically allocate the physical memory page and map the operation for the corresponding page through page fault management mechanism.
* the kernel maintains a current position variable `brk`(program break) to indicate the current end position of the task code and data(including the dynamic allocated memories)
    * the new allocated linear address space by `malloc()` is not actually mapped to physical page frames until page fault occurs.

## references
* https://man7.org/linux/man-pages/man2/brk.2.html
* https://man7.org/linux/man-pages/man3/malloc.3.html
