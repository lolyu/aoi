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


* storing the kernel stack and thread info makes it easy to derive the current process from `esp` easily.


### doubly linked lists
```c
struct list_head {
	struct list_head *next, *prev;
};
```

## the process list
* `task_struct` structure includes a `task` field of type `list_head` whose `prev` and `next` fields point to the previous and to the next `task_struct` element

### list of `TASK_RUNNING` processes
* linux put all runnable processes in the same list called `runqueue`
* one `runqueue` has multiple lists, one priority one list
* each `task_struct` descriptor includes a `run_list` field of type `list_head`
    * if the process priority is `k`(0 ~ 139), the `run_list` field links the process to the list of priority `k`
* `prio_array_t`: describes the core structure of `runqueue`, how to store multiple process lists based on priority
```c
struct prio_array {
    int        nr_active;                       /* number of tasks */ 
    unsigned long   bitmap[BITMAP_SIZE];        /* priority bitmap */
    struct list_head queue[MAX_PRIO];           /* priority queues */
};
```
* each `task_struct` descriptor includes an `array` field of type `prio_array_t` to point to its current runqueue
* `enqueue_task(p,array)`: inserts a process descriptor into a runner queue list
```
    list_add_tail(&p->run_list, &array->queue[p->prio]);
    __set_bit(p->prio, array->bitmap);
    array->nr_active++;
    p->array = array;
```

## process relationships
* `task_struct` includes multiple fields to describe process relationships

|field name|description|
|-|-|
|real_parent|points to the process descriptor of the process that created `P` or to process of pid 1(`init`) if the parent process no longer exists|
|parent|points to the current parent of `P`|
|children|the head of the list containing all children created by `P`|
|`sibling`|the pointers to the next and previous elements in the list of the sibling processes, those that have the same parent as `P`|


## pidhash table and chained lists


## references
* https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/types.h?h=v6.0-rc5
