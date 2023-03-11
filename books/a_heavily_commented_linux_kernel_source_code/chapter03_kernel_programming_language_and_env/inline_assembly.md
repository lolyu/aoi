# inline assembly
* two types:
    * basic `asm`
        * can be outside a C function
    * extended `asm`
        * must inside a C function
        * support jumps from `asm` to C labels 

## extended `asm`
```
asm [volatile] ( ``AssemblerTemplate``
                 : ``OutputOperands``
                 [ : ``InputOperands``
                 [ : ``Clobbers`` ] ])

asm [volatile] goto ( ``AssemblerTemplate``
                      :
                      : ``InputOperands``
                      : ``Clobbers``
                      : ``GotoLabels``)
```
* qualifiers:
    * `volatile`: disable `gcc` optimizations
        * GCC’s optimizers sometimes discard asm statements if they determine there is no need for the output variables.
        * GCC’s optimizers may move code out of loops if they believe that the code will always return the same result
    * `goto`
* parameters:
    * `AssemblerTemplate`
        * The compiler replaces tokens in the template that refer to inputs, outputs, and goto labels, and then outputs the resulting string to the assembler.
        * special format strings
            * `%%` -> `%`
    * `OutputOperands`: a comma-separated list of the C variables modified by the assembly instructions
    * `InputOperands`: a comma-separated list of C expressions read by the assembly instructions
    * `Clobbers`: A comma-separated list of registers or other values changed by the AssemblerTemplate, beyond those listed as outputs. An empty list is permitted
    * `GotoLabels`: When you are using the goto form of asm, this section contains the list of all C labels to which the code in the AssemblerTemplate may jump

![image](https://user-images.githubusercontent.com/35479537/224479521-3cd13d0c-7bdf-4837-8cec-7c406bf987d0.png)

### output operands
* format
```
[ [``asmSymbolicName``] ] ``constraint`` (``cvariablename``)
```
* `asmSymbolicName`: specifies a symbolic name for the operand
    * reference the name in the assembly template by `%[Value]`
* `constraint`:
    * output operands constraints must begin with either `=` or `+`
    * after, there must be one or more additional constraints describing where the value resides
        * `r` for register
        * `m` for memory
* `cvariablename`: specifies a C lvalue expression to hold the output
* When the compiler selects the registers to use to represent the output operands, it does not use any of the clobbered registers
#### difference between `r` and `m`
```c
    uint32_t c = 1;
    uint32_t d;
    uint32_t *e = &c;

    asm ("mov %[e], %[d]"
    : [d] "=m" (d)
    : [e] "m" (*e));
```
* with `m`: in the assembly code in `asm`, use memory address to replace the operand
```asm
	xorl	%eax, %eax
	movl	$1, -24(%ebp)
	leal	-24(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-16(%ebp), %eax
#APP
# 10 "inline_asm.c" 1
	mov (%eax), -20(%ebp)
# 0 "" 2
#NO_APP
```
* with `r`: in the assembly code in `asm`, use a free register to hold the value of the operand
```asm
	xorl	%eax, %eax
	movl	$1, -24(%ebp)
	leal	-24(%ebp), %eax
	movl	%eax, -20(%ebp)
	movl	-20(%ebp), %eax
	movl	(%eax), %eax
#APP
# 10 "inline_asm.c" 1
	mov %eax, %eax
# 0 "" 2
#NO_APP
	movl	%eax, -16(%ebp)
```

#### early clobber
* GCC compiler assumes all inputs will be consumed before any output registers are written to, so it is allowed to use the same register for both
* the `early clobber` marker is a way to tell the compiler that this output will be written to before all the input has been consumed, so it cannot share a register with any input

### input operands
* format
```
[ [``asmSymbolicName``] ] ``constraint`` (``cexpression``)
```
* `asmSymbolicName`: specifies a symbolic name for the operand
    * reference the name in the assembly template by `%[Value]`
* `constraint`:
    * input operands constraints should not begin with either `=` or `+`
    * `"0"` means the input operand will use the same set of registers as the first output operand
    * `"k"` means the input operand must be in the same place as the `k`th output operand

* `cvariablename`: specifies a C lvalue expression to hold the output
* When the compiler selects the registers to use to represent the output operands, it does not use any of the clobbered registers
* The compiler assumes that on exit from the asm statement these operands contain the same values as they had before executing the statement.

### clobbers
* While the compiler is aware of changes to entries listed in the output operands, the inline asm code may modify more than just the outputs.
* In order to inform the compiler of these changes, list them in the clobber list.
* When the compiler selects which registers to use to represent input and output operands, it does not use any of the clobbered registers. As a result, clobbered registers are available for any use in the assembler code.

* special clobbers:
	* `"cc"`
	* `"memory"`
## references
* https://stackoverflow.com/questions/15819794/when-to-use-earlyclobber-constraint-in-extended-gcc-inline-assembly
* https://dmalcolm.fedorapeople.org/gcc/2015-08-31/rst-experiment/how-to-use-inline-assembly-language-in-c-code.html
