# process creation

* traditionally, resouces owned by the parent process are duplicated in the child process, which is not efficient
* three different mechanisms to resolve the inefficiency:
    * copy on write
    * lightweight process
    * `vfork`

## clone
```c
#include <sched.h>

int clone(int (*fn)(void *), void *stack, int flags, void *arg, ...
         /* pid_t *parent_tid, void *tls, pid_t *child_tid */ );
```
* arguments:
    * `fn`: a function to be executed by the new process
    * `stack`: specifies the user mode stack pointer to be assigned to the `esp` register of the child process
        * Stacks grow downward on all processors that run Linux (except the HP PA processors), so stack usually points to the topmost address of the memory space set up for the child stack
    * `flags`: the low byte specifies the signal number to be sent to the parent process when the child terminates
    * `arg`: points to the data passed to `fn`
    * `parent_tid`:
    * `tls`: address of a data structure that defines a thread local storage segment for the new lightweight process
    * `child_tid`:
## references
