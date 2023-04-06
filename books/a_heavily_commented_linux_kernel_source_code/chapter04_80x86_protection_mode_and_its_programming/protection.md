# protection


## segmentat protection
* each memory reference is checked to verify the memory reference meets various protection requirements:
    * segment limit checks
    * segment type checks
    * privilege level checks
    * restriction of addressable domain
    * restriction of procedure entry-points
    * restriction of instruction set


### segment limit checks
* for expand-down segment, please refer to: https://www.sudleyplace.com/dpmione/expanddown.html
* segment limit check is to ensure offset is within range

### segment type check
* `S` field and `TYPE` field specifies the segment type
    * `S` field:
    * `TYPE` field:
### privilege level check

![image](https://user-images.githubusercontent.com/35479537/229528582-67590bbf-5376-4d6b-8199-7c0bbdafff7f.png)

* three types of privilege levels:
    * current privilege level(CPL)
        * current privilege level represents the privilege level of the current executing program or task
        * it is stored in the bits 0 and 1 of the `CS` and `SS` segment registers
    * descriptor privilege level(DPL)
        * descriptor privilege level represents the privilege level of a segment or gate
        * it is stored in the `DPL` field of the segment descriptor
    * request privilege level(RPL)
        * request privilege level is a privilege level associated with a segment selector
        * segment selector is from the logical address
        * it is stored in bits 0 and 1 of the segment selector
* `RPL` is to ensure the provider of the logical address has sufficient privilege to access

#### privilege level check when accessing data segment
![image](https://user-images.githubusercontent.com/35479537/230382422-2b8b3ff5-43c0-4988-a58d-650078eb7b65.png)
* To access operands in a data segment, the segment selector for the data segment must be loaded into the data-segment registers(`DS`, `ES`, `FS` or `GS`) or into the stack-segment registers(`SS`). Before the processor loads a segment selector into a segment register, it performs a privilege check by comparing `CPL`, `DPL` and `RPL`:
    * if `DPL` >= max(`CPL`, `RPL`), the segment selector is loaded into the segment register

#### privilege level check when transferring program control between code segments

##### direct calls or jumps to code segments
![image](https://user-images.githubusercontent.com/35479537/230393170-c2b70197-5940-4a5a-b8ad-688a5df15083.png)
* when transfering program control to another code segment without going through a call gate, the processor verifies four kinds of privilege level and type information:
    * caller `CPL`
    * destination code segment `DPL`
    * `RPL` in the segment selector of the destination code segment
    * the conforming flag `C` in the destination code segment descriptor
* how to check?
    * if the conforming flag `C` is set:
        * the caller `CPL` >= destination code segment `DPL`
        * `RPL` is ignored
    * if the conforming flag `C` is not set:
        * the caller `CPL` must be equal to the destination code segment `DPL`
        * `RPL` in the segment selector of the destination code segment <= `DPL`
* when the program control is transfered to a conforming code segment, the `CPL` doesn't change, this is the only case where `CPL` may not be the same as the current code segment `DPL`

##### gate descriptors
* to provide controlled access to code segment with different privilege levels, the processor provides a special set of descriptors called gate descriptors:
    * call gate(`TYPE`==12)
    * trap gate(`TYPE`==15)
    * interrupt gate(`TYPE`==14)
    * task gate(`TYPE`==5)

###### call gate
![image](https://user-images.githubusercontent.com/35479537/230399719-b2386872-f19e-40e2-8caa-89ca24342179.png)

* a call gate has the following functions:
    * segment selector to specifies the code segment to be accessed
    * offset to specifies an entry point for a procedure(program) in a specified code segment
    * `DPL` to specifies the privilege level that the caller must have
    * if a stack switch occurs, it specifies the number of optional parameters that need to be copied between the stack
    * indicates whether the call gate descriptor is valid

## references
* https://www.sudleyplace.com/dpmione/expanddown.html
* https://stackoverflow.com/questions/36617718/difference-between-dpl-and-rpl-in-x86
