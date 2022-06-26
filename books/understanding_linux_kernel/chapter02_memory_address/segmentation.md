# segmentation

## address types
* logical address: consists of a `segment` and a `offset`
* linear address: an address for any byte in linear address space is called linear address
* physical address: used to address memory cells in memory chips
* virtual address:


## segmentation
* two memory address translation:
    * real mode
    * protected mode

* logical address: segment selector(16 bits) + offset(32 bits)
* segmentation registers: hold segment selectors
    * `cs`[code segment]: points to a segment containing program instructions
        * register `cs` has a two-bits field, denotes the current privilege level(CPL) of the CPU
            * 0 denotes the highest level(kernel mode)
                * `ds` wil contain the segment selector of the kernel data segment
                * `ss` will contain kernel mode stack in kernel data segment
            * 3 denotes the lowest level(user mode)
                * `ds` will contains the segment selector of the user data segment
                * `ss` will contain user mode stack in user data segment
        * when CPU switches to kernel mode, `__KERNEL_CS` will be stored in `cs`
        * when CPU switches to user mode, `__USER_CS` will be stored in `cs`
    * `ss`[stack segment]: points to a segment containing the current program stack
    * `ds`[data segment]: points to a segment containing the global and static data
    * `es`[extra segment]
    * `fs`
    * `gs`

### segment descriptor


* segment descriptor: 8 bytes to describe the segment characteristics
    * segment descriptors are stored in either global descriptor table(GDT) or local descriptor table(LDT)
    * GDT address and size are stored in `gdtr` control register
    * LDT address and size are stored in `ldtr` control register

| field      | description                                                                                       |
| ---------- | ------------------------------------------------------------------------------------------------- |
| `Base      | linear address of the first byte of the segment                                                   |
| `G`        | granularity flag: 0 means the segment size is in bytes, 1 means the segment size is in 4096 bytes |
| `Limit`    | offset of the last memory cell in the segment                                                     |
| `s`        | 0 means the segment is a system segment, 1 means normal code or data segment                      |
| `Type`     | either of code segment descriptor or data segment descriptor or task state segment descriptor     |
| `DPL`      | descriptor privilege level, the minimal CPU privilege to access the segment                       |
| `P`        | 0 means if the segment is not currently in memory                                                 |
| `D` or `B` |                                                                                                   |
| `AVL`      |                                                                                                   |

### segment selector

![image](https://user-images.githubusercontent.com/35479537/174082021-5c302bf7-3fb7-4981-b83d-da4f16b6a69e.png)

* `index`: the index of either LDT or GDT
    * the first entry of GDT is always set to 0, this ensures that the logical addresses with a null segment selector will be considered invalid, that causing a processor exception
* `TI`: table indicator, 1 means the segment descriptor is in LDT, 0 means the segment descriptor is in GDT
* `RPL`: requester privilege level: CPL of the CPU when the corresponding segment selector is loaded into the CS register

### translation of logical address into linear address
* segmentation unit

![image](https://user-images.githubusercontent.com/35479537/174083442-02629656-e29a-4e42-aad6-6d883dbade6e.png)

## segmentation in Linux
* four segments are used by Linux:
    * user code segment: `__USER_CS`
    * user data segment: `__USER_DS`
    * kernel code segment: `__KERNEL_CS`
    * kernel data segment: `__KERNEL_DS`

| segment     | base       | G   | limit   | S   | type | DPL | D/B |
| ----------- | ---------- | --- | ------- | --- | ---- | --- | --- |
| user code   | 0x00000000 | 1   | 0xfffff | 1   | 10   | 3   | 1   |
| user data   | 0x00000000 | 1   | 0xfffff | 1   | 2    | 3   | 1   |
| kernel code | 0x00000000 | 1   | 0xfffff | 1   | 10   | 0   | 1   |
| kernel data | 0x00000000 | 1   | 0xfffff | 1   | 2    | 0   | 1   |

* though all four segments start with the same base and have same limit, but they have different DPL, so some assembly op will check the DPL stored in the segment descriptor to verify the access right.
    * https://stackoverflow.com/questions/4575032/why-does-linux-on-x86-use-different-segments-for-user-processes-and-the-kernel


### GDT
* GDT: global descriptor table
* each CPU has its copy of GDT table
* each GDT has 18 segment descriptors and 14 null, unused, or reserved entries
    * `__KERNEL_CS`
    * `__KERNEL_DS`
    * `__USER_CS`
    * `__USER_DS`
    * `TSS`: task state segment
        * used for task management
    * defautl `LDT` table, shared by all processes
    * TLS: thread local storage
    * PNPBIOS
    * APMBIOS   
![image](https://user-images.githubusercontent.com/35479537/175805524-11985bde-0a40-4bc7-a463-dae95ed5e48f.png)
