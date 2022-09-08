# process
* process is an instance of program in execution, from kernel's point of view, process is an entity to allocate system resources(CPU, memory, I/O, etc)
* in Linux, a thread group is a set of lightweight processes that implement a multithread application and act as a whole with regards to some system calls such as `getpid()`, `kill()`, and `_exit()`.

## task_struct

![image](https://user-images.githubusercontent.com/35479537/189126501-0718beaf-9e3c-4538-b0ee-e6ca250db3c1.png)

* `state`
    * `TASK_RUNNING`
        * the process is runnable, either running or on a run queue waiting to run
        * the only possible state for a process running in the userspace
    * `TASK_INTERRUPTABLE`
        * the process is sleeping until some conditions are true
            * hardware interrupt
            * signal delivery
            * released system resources
        * if the waiting condition is true, the process will come back to `TASK_RUNNING`
    * `TASK_UNINTERRUPTABLE`
        * like `TASK_INTERRUPTABLE`, but not honor a signal delivery
    * `TASK_STOPPED`
        * process execution is stopped
    * `TASK_TRACED`
        * process execution is stopped by a debugger
* `exit_state`
    * `EXIT_ZOMBIE`
        * process execution is completed, but its parent process is not aware of the finish
        * so kernel will keep the zombie process in the process table
    * `EXIT_DEAD`
        * the final state, the process is being removed by the system because the parent process has just issued `wait4()` or `waitpid()` system call for it.

## PID
* each process/thread has its own `task_struct` structure, and each `task_struct` has different `PID` number, stored in `pid`
* threads in the same thread group shares the same value that identify this thread group, which is the `PID` of the first thread in this group, and this value is stored in the `tgid`.
* `getpid()` system call retrieves `tpid` from the `task_struct`
```
$ cat /proc/sys/kernel/pid_max
32768
```

## process descriptor handling
```c
    union thread_union {
        struct thread_info thread_info;
        unsigned long stack[2048]; /* 1024 for 4KB stacks */
    };
```
![image](https://user-images.githubusercontent.com/35479537/189140071-f57600ab-397e-4dd5-9df7-702a1c69ca6e.png)
