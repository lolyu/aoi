# x86 assembly fundamentals


## gnu as

### as assembly syntax
* gnu as uses `AT&T` syntax, different with `Intel` syntax

#### instruction
* an instruction might consists of four components:
    * label
    * opcode
    * operand
        * at most three
        * for an instruction statement with two operands, the first is the source operand, the second is the destination operand
    * comments

* an operand can be:
    * an immediate: starting with `$`
    * register: starting with `%s`
    * memory

* `$` dollar sign before the label name instructs the assembler to use the memory address, not the data value located at the address.

* jump instruction is used to move the execution point to another location
    * unconditional jump
        * can be both direct jump(jump target is a label) and indirect jump(jump target is taken from a register or memory location)
    * conditional jump
        * direct jump only

## references
* https://users.pfw.edu/nguyent/teaching/spring20/cs271/lecture/Assembly-Fundamentals.pdf
