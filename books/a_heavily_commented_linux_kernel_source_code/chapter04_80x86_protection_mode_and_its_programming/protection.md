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

* call gate operation process
![image](https://user-images.githubusercontent.com/35479537/230750706-e8308064-0186-4a16-a1ba-af472d88668f.png)

* privilege level check for control transfer with call-gate
    1. privilege check between `CPL` and `DPL` of call gate
        * `CPL` <= call gate `DPL`
    2. privilege check between `RPL` and `DPL` of call gate
        * `RPL` <= call gate `DPL`
    3. privilege check between `CPL` and `DPL` o the destination code segment
        * `CALL`:
            * conforming code segment
                * destination code segment `DPL` <= `CPL`
            * nonconforming code segment
                * destination code segment `DPL` <= `CPL`
        * `JMP`:
            * conforming code segment
                * destination code segment `DPL` <= `CPL`
            * nonconforming code segment
                * destination code segment `DPL` == `CPL`
![image](https://user-images.githubusercontent.com/35479537/230750933-aa0f8973-dec3-40b2-b4bc-7c4672cb1668.png)
![image](https://user-images.githubusercontent.com/35479537/230750938-39cab9ea-a397-4420-a2ff-aa7ff2c1b351.png)

* An executable segment whose descriptor has the conforming bit set is called a conforming segment. **The conforming-segment mechanism permits sharing of procedures that may be called from various privilege levels but should execute at the privilege level of the calling procedure.** Examples of such procedures include math libraries and some exception handlers. When control is transferred to a conforming segment, the CPL does not change. This is the only case when CPL may be unequal to the DPL of the current executable segment.
* Most code segments are not conforming. The basic rules of privilege above mean that, for nonconforming segments, control can be transferred without a gate only to executable segments at the same level of privilege. There is a need, however, to transfer control to (numerically) smaller privilege levels; this need is met by the CALL instruction when used with call-gate descriptors, which are explained in the next section. The JMP instruction may never transfer control to a nonconforming segment whose DPL does not equal CPL.
* If a call transfers control to a non-conforming code segment of a higher privilege level, the CPL is set to the DPL value of the destination code segment and causes a stack switch. But if a call or jump transfers control to a higher privileged level conforming code segment, the CPL does not change and does not cause a stack switch.


###### stack switching
* when does stack switching occurs?
    * for direct `CALL` or `JMP` to conforming code segment, the `CPL` does not change
    * for direct `CALL` or `JMP` to non-conforming code segment
        * only `CALL` is able to transfer control to high privilege code segment
            * `CPL` is set to the `DPL` of the destination code segment
            * stack switch occurs

* why stack switch is needed when transfer control to a non-conforming code segment with higer privilege?
    * prevent low privilege caller to mangle the stack of higher privilege programs through shared stack
* each task must define up to 4 tasks(corresponding to 4 privilege levels), and each stack is stored in different segment
* the initial pointer values for stacks of privilege levels 0, 1, 2 are stored in `TSS`, when the privilege level 3 program is executing, the segment selector and stack pointer of the privilege level 3 stack are stored in the SS and ESP, respectively, and are saved on the stack of the called procedure when a stack 
switch occurs

* stack switch when calling between different privilege levels
![image](https://user-images.githubusercontent.com/35479537/230751830-24c1c244-5f57-463a-af02-da2d4bb279ce.png)

* operation when return from call:
    1. check `RPL` field value in the saved `CS` register to determine if the privilege level needs to be changed on return
    2. pop up saved `CS` and `EIP` from stack, and perform privilege check between `RPL`(from the code segment selector), `CPL`(from the code segment) and `DPL` of the saved `CS`
    3. if the `RET` instruction contains a parameter count operand(`N`), `ESP` is incremented by `N` to pop out passed function arguments
    4. loads the `SS` and `ESP` saved on top of the stack into `SS` and `ESP` registers to switch back to the caller's stack
    5. check the contents of segment registers `DS`, `ES`, `FS`, and `GS`, If there is a segment pointing to a DPL that is smaller than the new CPL (except for the consistent code segment), then the CPU loads the segment register with the NULL selector.

## page protection
* the read/write flags `R/W` and the user/supervisor flag `U/S` in the page directory and page table entries provide page protection
* privilege levels 0, 1, and 2 are classified as superuser level, level 3 is classified as a normal user level


## references
* https://www.sudleyplace.com/dpmione/expanddown.html
* https://stackoverflow.com/questions/36617718/difference-between-dpl-and-rpl-in-x86
* https://pdos.csail.mit.edu/6.828/2005/readings/i386/s06_03.htm
* https://wiki.osdev.org/Task_State_Segment
