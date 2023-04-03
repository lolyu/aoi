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
        * it is stored in bits 0 and 1 of the segment selector
* `RPL` is to ensure the provider of the logical address has sufficient privilege to access


## references
* https://www.sudleyplace.com/dpmione/expanddown.html
* https://stackoverflow.com/questions/36617718/difference-between-dpl-and-rpl-in-x86
