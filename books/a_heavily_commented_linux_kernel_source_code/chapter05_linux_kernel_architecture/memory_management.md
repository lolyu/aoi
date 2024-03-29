# memory management
* physical memory layout

![image](https://github.com/lolyu/aoi/assets/35479537/237c81d9-7b6e-4fa5-b668-b3ba214de8bf)

* address types:
    * virtual address
        * the segment selector and the offset address
    * logical address
        * the offset address of the segment section generated by the program
        * exposed to application programmer
    * linear address
    * physical address

## memory segmentation mechanism

![image](https://github.com/lolyu/aoi/assets/35479537/9725d516-899c-458c-8448-647ccd194fd8)
* segmentation addressing:
    * real-mode
    * protected-mode

![image](https://github.com/lolyu/aoi/assets/35479537/1b9c4f25-5c10-4011-83f0-5dc23ef82cdd)


### real mode
* 16-bit mode
* address space: a 20-bit physical address = 2 ^ 20 = 1MB
* all x86 processors begin execution in real mode
* real-mode segment has fixed size: 64KB
* real-mode addressing uses segment base and offset values:
    * segment base is stored inside segment registers(`DS`)
        * six 16-bit segment registers:
            * `CS`
            * `DS`
            * `ES`
            * `FS`
            * `GS`
            * `SS`
    * offset is stored in any register that can be used for addressing(`SI`)

```
PhysicalAddress = Segment * 16 + Offset
```

### protected mode
* protected-mode addressing uses segment selector and offset values:
    * segment selector
        * index value of the segment descriptor in the segment descriptor table
        * segment table selection bit
        * privilege level
    * offset

* three segment descriptor table:
    * global descriptor table(`GDT`)
    * local descriptor table(`LDT`)
    * interrupt descriptor table(`IDT`)
* three registers to store the base address of the three segment descriptor table:
    * `GDTR`
    * `LDTR`
    * `IDTR`

![image](https://github.com/lolyu/aoi/assets/35479537/567d279a-344e-4e92-9d7f-4d9ccc967cc9)


## paging
* registers:
    * `CR0`: whether to enable paging
    * `CR3`: also as `PDTR`, stores the base physical address of the page directory table
* the linear address is divided into 4K pages
* one page table takes 4K physical memory, as `PTE` is of 4 bytes, so 1024 `PTE`s
* one page directory
![image](https://github.com/lolyu/aoi/assets/35479537/41c0786f-3aca-42db-8fa7-41b4e982f68c)


## 0.12 kernel memory layout
![image](https://github.com/lolyu/aoi/assets/35479537/8908a4da-0f26-45d0-bca6-d6876217abd7)
* 0.12 kernel supports up to `NR_TASKS` = 64 tasks
* the starting position of each task in the linear address space is (task number) * 64 MB
* in the above pic, four tasks total:
    * the kernel code segment and data segment are mapped to the beginning 16MB area of the linear address space
    * task 0 is started by the kernel, and its code segment and data segment is 640KB starting from 0

* **NOTE**: data segment and code segment used in the task logical address space is not the same concept as the code segment and data segment used in segmentation:
    * In segmentation, the code and data segments describes a segment in the linear address space with access constraints.
    * In task logical address space, the code section and data section refer to the code area and initialized && uninitialzed data area.

![image](https://github.com/lolyu/aoi/assets/35479537/4ea1112d-d46c-4309-a794-5b8d1ec2e3f6)


## reference
* https://wiki.osdev.org/Real_Mode
* https://wiki.osdev.org/Protected_Mode
