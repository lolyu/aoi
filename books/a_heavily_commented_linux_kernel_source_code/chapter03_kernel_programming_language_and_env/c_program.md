# c program
![image](https://user-images.githubusercontent.com/35479537/222869124-aec1db65-855b-4301-a7f0-dbe516b22d0e.png)

```
# gcc -o hello hello.c              # compile to generate the execution file
# gcc -S -o hello.s hello.c         # compile to generate corresponding assembly
# gcc -c -o hello.o hello.c         # compile but not linking
```
## inline assembly
```c
#include <stdlib.h>
#include <stdio.h>

#define multi_five(x, y) \
({asm("leal (%1, %1, 4), %0;" \
      :"=r"(y) \
      :"0"(x) \
);})

int main()
{
    int x = 10;
    int y;
    multi_five(x, y);
    printf("%d * 5 = %d\n", x, y);
    return 0;
}
```
* `asm` keyword is reserved to identify the inline assembly
* `__asm__` keyword is reserved by `ANSI C` to as the inline assembly keyword

### asm format
```
asm ("assembly code" : output locations : input operands : changed registers);
```
* components:
    1. `assembly code`
    2. `output locations`: a list of registers and memory locations that will contain the output values from the inline assembly code
    3. `input operand`: a list of registers and memory locations that contain input valus for the inline assembly code
    4. `changed registers`: a list of any additiona registers that are changed by the inline code
* the format of `2`, `3`, and `4`: `"constraints"(variable)`
* **NOTE**: if the constraints of input register is `0` or empty, the same register as the corresponding output is used.

![image](https://user-images.githubusercontent.com/35479537/222896163-3cf8d53a-4cdc-4e04-b382-5936e80ac3b5.png)

#### placeholds
```
# Using placeholders
# ------------------

# For example, the following inline code:
asm ("assembly code"
     : "=r"(result)
     : "r"(data1), "r"(data2));

# Will produce the following placeholders:
# - %0 will represent the register containing the result variable value.
# - %1 will represent the register containing the data1 variable value.
# - %2 will represent the register containing the data2 variable value.
```
* the embedded assembler program specifies that the output and input registers are numbered sequentially, starting from the left and right top to bottom of the output register sequence starting with `%0`, respectively, denoted as `%0`, `%1`, ... `%9`. 

#### strncpy
```c
#include <stdio.h>

// gcc -m32 strncmp.c

int strncmp(const char * cs, const char * ct, size_t count)
{
    register int __res;
    asm volatile(
        "cld;"                      // clear up flags
        "1: decl %3;"               // #1: --count
        "js 2f;"                    // if count < 0, jump to #2
        "lodsb;"                    // load a byte from esi into al, which is ct
        "scasb;"                    // compare al with edi, which is cs
        "jne 3f;"                   // if not equal, jump to #3
        "testb %%al,%%al;"          // test if al is NULL
        "jne 1b;"                   // if al is not NULL, got to #1
        "2: xorl %%eax,%%eax;"      // #2: eax = 0
        "jmp 4f;"                   // jump to #4, return 0
        "3: movl $1,%%eax;"         // #3: eax = 1
        "jl 4f;"                    // if al < edi, jump to #4, return 1
        "negl %%eax;"               // #4: eax = -1
        "4:;"                       // #4: return eax
        :"=a" (__res): "D" (cs), "S" (ct), "c" (count));
    return __res;
}

int main()
{
    const char *a = "helloworld";
    const char *b = "hellowerld";
    printf("%d\n", strncmp(a, b, 9));
    return 0;
}
```
