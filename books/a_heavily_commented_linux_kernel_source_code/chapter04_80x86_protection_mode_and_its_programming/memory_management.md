# memory management
* for 80x86 cpu, the address bus width is 32 bits, so there are a total of 2^32 different physical addresses
* 80x86 cpu is small-edian: store the low-value byte first then store the high-vale byte
* any complete memory management system contains two key part:
    * address translation
    * protection

## segmentation in 80x86
* logical address
    * used in segmentation
    * 48 bits = 16-bit segment selector and 32-bit offset

* six segment registers:
    * `CS`: code segment
        * `CS` points to the current code segment
        * `EIP` gives the offset address within the code segment of current running task
        * the address of the instruction to be executed can be expressed as `CS:[EIP]`
    * `DS`: data segment
    * `ES`: extra segment
    * `SS`: stack segment
        * `SS` points to the current stack base address
        * `ESP` gives the address of the stack top
        * `SS:[ESP]` is the address of the top of stack
    * `FS`: general segment
    * `GS`: general segment

## 80x86 address translation

![image](https://user-images.githubusercontent.com/35479537/226152059-3dd0fb68-8dd9-4581-9e0b-66fb62484016.png)

![image](https://user-images.githubusercontent.com/35479537/226152317-b846ec26-5c42-47ce-8693-f4f54af9aaef.png)
* the process from logical address -> linear address -> physical address
* If there are multiple programs or tasks running in the processor, each program can allocate its own set of segments.
* logical address consists of a segment selector and an offset
    * segment selector stores the offset of segment descriptor in the segment descriptor table
        * either `gdt` or `ldt`: global descriptor table or local descriptor table
        * the segment descriptor stores information about:
            * size of the segment
            * access right and privilege level of the segment
            * the segment type
            * segment base address
* virtual address is consisted of VPN and offset
    * VPN: virtual page number, the index of which page the address is within
    * offset: the offset of the address within a page
* physical address is consisted of PFN and offset
    * PFN: page frame number, the index of which page frame the address is within
    * offset: the offset of the address within a page frame 

![image](https://user-images.githubusercontent.com/35479537/226152378-f3e7ad8c-8045-45c7-b225-676449583218.png)

## 80x86 memory protection
* two protections
    * protection between tasks
        * the principle is to give each task a different mapping of logical addresses to physical addresses
        * for each task, different segment descriptor table and page table
        * when the processor switches to perform a new task, the key part of task switching is to switch to the new task's conversion table
        * By arranging the same virtual-to-physical address mapping portion in all tasks and storing the operating system in this common virtual address space portion, the operating system can be shared by all tasks.
        * the same portion of the virtual address space that all tasks has is called global address space.
    * privilege level protection
        * the privilege level is represented by the number 0 ~ 3, 0 being the highest privilege and 3 being the lowest
        * each privilege level has its own program stack, when the program is switched from one privilege level to another, the stack segment is also changed to a new level stack


## references
* https://wiki.osdev.org/Segmentation
* https://en.wikipedia.org/wiki/FLAGS_register

