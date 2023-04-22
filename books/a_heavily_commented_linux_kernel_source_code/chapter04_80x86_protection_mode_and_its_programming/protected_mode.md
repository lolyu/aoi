# protected mode
* When the machine is powered on or hardware reset, the processor operates in the 8086-compatible real-address mode and executes the software initialization code starting at physical address `0xFFFFFFF0` (usually in EPROM).

## initialization operation entering protection mode
* before the processor can be switched to protected mode, the operating system's loading and initialization software(`bootsect.s`, `setup.s`, and `head.s`) must first set the basics of the data structure used in protected mode in memory:
    * a protected-mode `IDT`
    * `GDT`
    * `TSS`
    * `LDT`
    * if paging is enabled, at least one page directory and one page table need to be set
    * a code segment containing execution code for the processor to switch to the protected mode
    * code modules that contain interrupts and exception handlers

* the software initialization code must set the following system registers before switching to protected mode:
    * global descriptor table base address register `GDTR`
    * interrupt descriptor table base address register `IDTR`
    * control registers:
        * `CR1`
        * `CR2`
        * `CR3`


## references
* 
