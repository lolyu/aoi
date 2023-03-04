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
