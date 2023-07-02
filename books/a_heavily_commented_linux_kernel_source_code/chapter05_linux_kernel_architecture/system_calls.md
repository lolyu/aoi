# system calls
![image](https://github.com/lolyu/aoi/assets/35479537/a40cded2-6dd0-4c6c-8e59-c35b966bc49c)

* when the application issues an interrupt `INT 0x80` to the kernel via a library function, a system call is initiated:
    * the system call number is stored in `EAX`
    * the system call parameters are stored in `EBX`, `ECX`, and `EDX`

```c
#define _syscall3(type,name,atype,a,btype,b,ctype,c) \
type name(atype a,btype b,ctype c) \
{ \
long __res; \
__asm__ volatile ("int $0x80" \
	: "=a" (__res) \
	: "0" (__NR_##name),"b" ((long)(a)),"c" ((long)(b)),"d" ((long)(c))); \
if (__res>=0) \
	return (type) __res; \
errno=-__res; \
return -1; \
}
```
* the `write` definitin:

```c
#define __LIBRARY__
#include <unistd.h>

// _syscall3(int,write,int,fd,const char *,buf,off_t,count)
int write(int fd, const char *buf, off_t count)
{
    long __res;
    __asm__ volatile("int $0x80"
                     : "=a"(__res)
                     : "0"(__NR_write), "b"((long)(fd)), "c"((long)(buf)), "d"((long)(count)));
    if (__res >= 0)
        return (int)__res;
    errno = -__res;
    return -1;
}
```



## references
