# multitask and protection
![image](https://github.com/lolyu/aoi/assets/35479537/fe5e6dcf-13aa-4388-bc55-0344dec5e753)

## mapping between virtual addresses, linear addresses and physical addresses

### kernel
![image](https://github.com/lolyu/aoi/assets/35479537/fd833aff-147d-429a-98f4-f8358b6c182f)

* for linux 0.12, both the kernel code segment and data segment are set to an area of length 16MB(`0x0` ~ `0xFFFFFF`)
    * this 16MB memory space contains the kernel code, kernel segment tables(`GDT`, `IDT`, `TSS`), page directory tables and page tables, kernel local data, and kernel temporary stack(used as user stack for task 0)
    * this 16MB memory space uses 16MB / 1024 / 4KB = 4 `PDE`s(page directory entries)
* the kernel code and data segments are mapped to the same linear address space and physical address space.


![image](https://github.com/lolyu/aoi/assets/35479537/9b9040b2-a86f-41a2-a183-5ad740133e4e)

### task0
![image](https://github.com/lolyu/aoi/assets/35479537/fad005b0-28e4-4b1e-a9e6-74f4800c714a)

* for task 0:
    * its code segment and data segment both starts from 0, but are limited to 640KB


### task1
![image](https://github.com/lolyu/aoi/assets/35479537/f065b600-ed77-4e24-8cda-4d732dbd5deb)


### other tasks
![image](https://github.com/lolyu/aoi/assets/35479537/576d83df-8117-431f-982f-d5985c647619)

## references
* 
