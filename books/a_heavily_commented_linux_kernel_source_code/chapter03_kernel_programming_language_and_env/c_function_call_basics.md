# c function call basics

![image](https://user-images.githubusercontent.com/35479537/222956232-bdb2a51c-a853-4380-addf-ed405a03843b.png)
* the stack frame:
    * `ebp`: frame pointer
    * `esp`: stack pointer

* the stack frame is responsible for:
    * storing a function's local variables
    * storing parameters passed to the callee function
    * storing return address to the callee function
    * for temporary storage used for computing intermediate expression's results
    * for saving processor registers across function calls and returns

* when function `A` calls function `B`:
    * the parameters passed to `B` are pushed onto the stack(in the stack frame of `A`)
    * the return address of function `A` is pushed onto the stack
    * `ebp` is pushed onto the stack(`A`'s ebp value)
    * the stack frame of `B` starts from the subsequent stack section

## x86 function calling conventions
* few basics:
    * stack grows downwards, from higher address space to lower address space
    * `esp` points to the stack top
    * the size of data moved in/out during stack operations depends on the processor's word length(x86, it is 32-bits)
* `ebp` indicates to the origin of current stack frame
* register conventions:
    * `eax`, `edx`, and `ecx`: must be save onto the stack by the caller to avoid modification by callee function
    * `ebx`, `esi`, and `edi`: if any of them is modified by the callee, it should be saved onto the stack by the callee and restored when callee function returns.

### main() as a function
* `main` is also a function, called by the stub program
* more on the stub: `crt(c run-time)`:
    * https://dev.gentoo.org/~vapier/crt.txt
    * https://stackoverflow.com/questions/1271248/c-when-and-how-are-c-global-static-constructors-called
    * https://stackoverflow.com/questions/3047518/gcc-crtbegin-crtend

### function prologue and epilogue
```asm
    push ebp
    mov ebp, esp
    sub esp, N
```
```asm
    mov esp, ebp
    pop ebp
    ret
```

## gdb to inspect stack
```c
#include <stdlib.h>

int f(int a, int b)
{
    int c = a + b;
    return c;
}

int g()
{
    int sum = f(10, 10);
    return sum;
}

int main()
{
    g();
    return 0;
}
```
```
(gdb) x/10x $esp
0xffffd160:     0xf7fb63fc      0x00000001      0x56558fdc      0x00000014
0xffffd170:     0xffffd190      0x565561ce      0x0000000a      0x0000000a
0xffffd180:     0xf7fe3230      0x00000000
(gdb) print $esp
$11 = (void *) 0xffffd160
(gdb) print $eip
$12 = (void (*)()) 0x565561af <f+34>
(gdb) print $ebp
$13 = (void *) 0xffffd170
```

## call c function from assembly
* to call a c function from assembly:
    * reversely push function parameters onto stack
    * use `CALL` to execute target function
    * target function executes
    * cleanup target function parameters on the stack

## call assembly from c function

## references
* https://thecandcppclub.com/deepeshmenon/chapter-4-the-stacks-in-c/227/
   
