# x86 segmentation

## terms
* linear address
    * an address for any byte in linear address space
    * linear address space denotes all addresses that can be formed on the system
    * linear addresses are always represented by a fixed-bytes integer
* physical address
    * memory address the processor addresses on its bus
* virtual address
    * in paging, each program is assigned with its own linear address space, the address in this linear address space is called virtual address
* logical address
    * used in segmentation, consists of a segment selector and an offset

## history
* intel 8086 introduces real mode to address more than 64KB(2^64) memory
* intel 80286 introduces protected mode to support virtual memory and memory protection

## real mode
* in real mode, the segment size ranges from 1 to 65536(2^64) bytes memory
* the segment selector and the offset are both pointers of 16 bits
* linear address is the same as physical adderss in real mode

```
logical address -> linear address(physical address)
```

### linear address calculation
* the segment selector is interpreted as the most significant 16 bits of a linear 20-bits address

<img width="449" alt="image" src="https://user-images.githubusercontent.com/35479537/208225449-6fb0193a-969a-41f0-86e1-e6b117c30ad7.png">


## protected mode
![image](https://user-images.githubusercontent.com/35479537/208225606-ff6ff1ca-8ff8-4fcd-b5b8-be64aea66997.png)

### liear address calculation
* a logical address consists of a 16-bit segment selector and a 16-bit offset
* the segment selector is used to select the segment descriptor

* the segment selector

![image](https://user-images.githubusercontent.com/35479537/208225663-0e97da3e-cfae-4f8c-b240-7404807937ac.png)

* the segment descriptor

![image](https://user-images.githubusercontent.com/35479537/208225652-d0e815f0-d856-4978-993f-2c4731138db8.png)

#### workflow
![image](https://user-images.githubusercontent.com/35479537/208225769-e6eebe12-7036-412e-81e4-29434a0ae2cc.png)

* given a logical address:
    1. if `TI` == 0, read the segment descriptor from the global descriptor table(GDT), if `TI` == 1, read from the local descriptor table(LDT)
    2. performs the privilege check: `max(CPL, RPL) <= DPL`
        * `CPL` is the current privilege level
        * `RPL` is the privilege level from the segment selector
        * `DPL` is the privilege level of the segment descriptor
    3. takes of offset from the logical address and compares it against the segment limit in the segment descriptor
    4. calculate linear address = segment base + offset

### combination of segmentation and paging
```
logical address --> segmentation unit --> virtual address --> paging unit --> physical address
```

## references
* https://en.wikipedia.org/wiki/X86_memory_segmentation
* https://alex.dzyoba.com/blog/os-segmentation/
