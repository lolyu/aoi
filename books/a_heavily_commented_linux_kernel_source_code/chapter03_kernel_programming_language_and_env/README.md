## contents
* [boch](https://github.com/lolyu/aoi/blob/master/books/a_heavily_commented_linux_kernel_source_code/chapter03_kernel_programming_language_and_env/bochs.md)
* [c program](https://github.com/lolyu/aoi/blob/master/books/a_heavily_commented_linux_kernel_source_code/chapter03_kernel_programming_language_and_env/c_program.md)
* [c function call basics](https://github.com/lolyu/aoi/blob/master/books/a_heavily_commented_linux_kernel_source_code/chapter03_kernel_programming_language_and_env/c_function_call_basics.md)
* [inline assembly](https://github.com/lolyu/aoi/blob/master/books/a_heavily_commented_linux_kernel_source_code/chapter03_kernel_programming_language_and_env/inline_assembly.md)
* [linux 0.12 object file format](https://github.com/lolyu/aoi/blob/master/books/a_heavily_commented_linux_kernel_source_code/chapter03_kernel_programming_language_and_env/linux_object_file_format.md)
* [x86 assembly fundamentals](https://github.com/lolyu/aoi/blob/master/books/a_heavily_commented_linux_kernel_source_code/chapter03_kernel_programming_language_and_env/x86_assembly_fundamentals.md)
* [makefile walkthrough](https://github.com/lolyu/aoi/blob/master/books/a_heavily_commented_linux_kernel_source_code/chapter03_kernel_programming_language_and_env/makefile_walkthrough.md)

## assembly basics

```
# - Registers
# |__ General purpose (Eight 32-bit registers used for storing working data)
#     |__ EAX (RAX for 64-bit) Accumulator for operands and results data
#     |__ EBX Pointer to data in the data memory segment
#     |__ ECX Counter for string and loop operations
#     |__ EDX I/O pointer
#     |__ EDI Data pointer for destination of string operations
#     |__ ESI Data pointer for source of string operations
#     |__ ESP Stack pointer
#     |__ EBP Stack data pointer
#             ESP is the top of the stack.
#             EBP is usually set to esp at the start of the function.
#             Local variables are accessed by subtracting a constant 
#             offset from ebp. All x86 calling conventions define ebp 
#             as being preserved across function calls. ebp itself
#             actually points to the previous frame's base pointer,
#             which enables stack walking in a debugger and viewing 
#             other frames local variables to work
```
```
# With the GNU assembler, there are two ways to load the ESI and EDI values. 
# The first way is to use indirect addressing

movl $output, %edi

# Another method of specifying the memory locations is the LEA instruction. 
# The LEA instruction loads the effective address of an object.

leal output, %edi
```

# references
* https://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html
* https://github.com/lkundrak/dev86
* https://linux.die.net/man/1/as86
* https://gist.github.com/mishurov/6bcf04df329973c15044
* https://en.wikipedia.org/wiki/FLAGS_register
* https://www.felixcloutier.com/x86/
* https://dmalcolm.fedorapeople.org/gcc/2015-08-31/rst-experiment/how-to-use-inline-assembly-language-in-c-code.html
