# segmentation

## segment definition
* segment is defined by three parameters:
    * base address: specifies the starting address of the segment in the linear address space
    * limit: the maximum offset within the segment
    * attributes: specifies the characteristics of the segment

* segment descriptor: data structure to store the base address, limit and attributes
* descriptor table: a continous block of memory to store the segment descriptors
![image](https://user-images.githubusercontent.com/35479537/226616285-297456f6-ac2a-4f6e-a912-22e87fa21b2b.png)

* logical address: consisted of a segment selector and offset
    * for 80x86, 16-bit segment selector and 32-bit offset

### conversion from logical address to linear address
* linear address is a 32-bit address in the processor's linear address space
    * 32-bit address space is 4GB address space from 0 to `0xFFFFFFFF`
![image](https://user-images.githubusercontent.com/35479537/226617720-1d1cd4e0-a3cb-4491-90ad-de0f818e4dc7.png)
* **NOTE**: if paging is not enabled, the processor directly maps the linear address to the physical address(the linear address is sent to the processor's address bus)

## descriptor table
* the entire logical address space contains 2^14 segments
* `GDT`: global descriptor table, shared by all tasks
* `LDT`: local descriptor table, local only to a task
* the logical address space is divided into two halves of equal size:
    * half is mapped to a linear address by the `GDT`
        * 2^13 segments
    * half is mapped to a linear address by the `LDT`
        * 2^13 segments

![image](https://user-images.githubusercontent.com/35479537/226618542-074a0a32-f805-431a-b14b-496739ac550a.png)

![image](https://user-images.githubusercontent.com/35479537/226620196-06c22c1a-e37f-41d2-b682-e3f6dd91a6ac.png)

* when task A is running, segments `CodeA`, `DataA`, `CodeOS` and `DataOS` are accessible, while task B is running, segments `CodeB`, `DataB`, `CodeOS` and `DataOS` are accessible.
* task isolation:
    * when `A` is running, the segment of task `B` is not part of the logical address space, so task `A` could not access task `B`'s memory

### GDT and GDTR
* the base address and limit of `GDT` is stored in a register called `GDTR`
* the base address of `GDT` should be aligned with 8 bytes(as the segment descriptor is of 8 bytes)
* the base address + limit = the last byte of the `GDT`
* limit == 0 means 1 bytes long
* for a `GDT` that contains `N` segments, its limit is `8N - 1` bytes
* the first descriptor is a `NULL` descriptor, used to init segment registers.
    * but when reference segment registers with `NULL` segment descriptor, an exception is throw
### LDT and LDTR
* `LDT` is stored in `GDT` as a segment descriptor
* to convert a logical address to linear address with `LDT`:
    * use the `LDT` segment selector to get the segment descriptor corresponding to `LDT` out of `GDT`
    * get the base address and limit from the segment descriptor
    * check access rights and get the linear address by adding the base address in the segment descriptor and the offset
* to retrieve `LDT` easily, `LDTR` is used to store the segment selector to get the `LDT` in `GDT`, along with its base address, limit and attributes

## segment selector
![image](https://user-images.githubusercontent.com/35479537/226631931-a789c466-6f94-4977-a952-dd6175b1fe17.png)

* segment selector is used to get the segment descriptor from the descriptor table:
    * index
    * table index (TI)
    * requested privilege level
* `null selector`: index as 0 and `TI` as 0
    * when `null selector` is loaded into a segment register(other than `CS` and `SS`), the processor doesn't generate an exception, but an exception is raised when using a segment register with `null selector` to access memory

* To reduce address translation time and programming complexity, the processor provides registers that hold up to six segment selectors:
    * `CS`: code segment
    * `DS`: data segment
    * `SS`: stack segment
    * `ES`
    * `FS`
    * `GS`

![image](https://user-images.githubusercontent.com/35479537/226635093-58630969-408c-4021-84b1-427ed8ecf591.png)

* each segment register has a "visible" part and a "hidden" part (the hidden part is also called " Descriptor Buffer" or "Shadow Register"). When a segment selector is loaded into the visible portion of a segment register, the processor also loads the segment address, segment length, and access control information in the segment descriptor pointed to by the segment selector to the hidden portion of the segment register. 

## segment descriptor
![image](https://user-images.githubusercontent.com/35479537/227751465-ef7d07d6-4a68-4ca4-b206-bf92b878ef52.png)
* fields:
    * limit
        * the segment size is always `(limit + 1) * granularity`
    * base
        * base address is always aligned at 16 bytes for better performance
    * type field(`TYPE`): for data segment, the type field is interpreted as `E`, `W`, `A`; for code segment, the type field is interpreted as `C`, `R`, `A`.
        * conforming(`C`)
            * code in this segment may be called from less-privileged levels
        * expand-down(`E`)
            * If clear, the segment expands from base address up to base+limit. If set, it expands from maximum offset down to limit, a behavior usually used for stacks.
        * readable(`R`)
            * If clear, the segment may be executed but not read from.
        * writable(`W`)
            * If clear, the data segment may be read but not written to.
        * accessed(`A`)
            * This bit is set to 1 by hardware when the segment is accessed, and cleared by software.
    * descriptor type flag(`S`)
        * `S == 0`: internal system segment descriptor: `LDT`, `LSS` and gates
        * `S == 1`: code and data segment descriptor
    * descriptor privilege level(`DPL`)
    * segment present(`P`)
        * `P == 1`: segment present in memory
        * `P == 0`: segment not present in memory
    * `D/B`
        * `D`: default operand size, if clear, 16-bit code segment, if set, 32-bit code segment
        * `B`: If set, the maximum offset size for a data segment is increased to 32-bit 0xffffffff. Otherwise it's the 16-bit max 0x0000ffff. Essentially the same meaning as "D".
    * granularity(`G`)
        * if clear, offset is of bytes
        * if set, offset is of 4KB
    * available(`AVL`)

![image](https://user-images.githubusercontent.com/35479537/227752559-d84e72aa-4be6-4e85-ba48-77dd4c643d59.png)

![image](https://user-images.githubusercontent.com/35479537/227752566-eba15776-6d92-4410-8cdc-4efc9a31e150.png)

### segment descriptor types
* when the descriptor type flag(`S`) is cleared in a reset state(`0`), the descriptor is a system descriptor:
    * local descriptor table segment descriptor
    * task-state segment descriptor
    * call-gate descriptor
    * interrupt-gate descriptor
    * trap-gate descriptor
    * task-gate descriptor


![image](https://user-images.githubusercontent.com/35479537/227753271-254bed60-a1f9-4d03-943c-33c666c8f02e.png)


## reference
* https://en.wikipedia.org/wiki/Segment_descriptor
