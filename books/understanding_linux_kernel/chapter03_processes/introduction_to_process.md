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
* the process list contains all the processes in the system
* the list head is `sched`(or `swapper`), so called process 0.

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
|parent|points to the current parent of `P`, this is the process that must be signaled when the child process terminated|
|children|the head of the list containing all children created by `P`|
|`sibling`|the pointers to the next and previous elements in the list of the sibling processes, those that have the same parent as `P`|

* `parent` vs `real_parent`
	* in normal scenario, `parent` == `real_parent`
	* if the process is attached by a debugger(GDB), the `parent` will be the debugger so it could receive the process termination signal.
	* if the process is an orphan, its `parent` and `real_parent` will be `1` in such case. 

## pidhash table and chained lists
* linux uses hash tables to retrieve the `task_struct` pointer from PID
* four hash tables, and their addresses are stored in `pid_hash` array

|hash table type|field name|description|
|-|-|-|
|`PIDTYPE_PID`|`pid`|PID of the process|
|`PIDTYPE_TGID`|`tgid`|PID of thread group leader process|
|`PIDTYPE_PGID`|`pgrp`|PID of the group leader process|
|`PIDTYPE_SID`|`session`|PID of the session leader process|

### hash calculation
```c
#define pid_hashfn(x) hash_long((unsigned long) x, pidhash_shift)

unsigned long hash_long(unsigned long val, unsigned int bits)
{
	unsigned long hash = val * 0x9e370001UL;
	return hash >> (32 - bits);
}
```

### collision
![image](https://user-images.githubusercontent.com/35479537/192210516-e8f1876c-54e9-4af2-af14-ed4d022a6cdc.png)


### how to get all processes in a thread group/group/session
* `pid` structure

|type|name|description|
|-|-|-|
|int|nr|the pid number|
|struct hlist_node|pid_chain|the links to the next and previous elements in the hash chain list|
|struct list_head|pid_list| the head of the per-PID list|

* `task_struct` has a field `pids`, which is an array of four struct `pid` structures

![image](https://user-images.githubusercontent.com/35479537/192212692-a6eaa3e4-dc46-41bf-b7c9-0d9bb5ff7f96.png)

## how processes are organized
* runqueue lists group all processes in `TASK_RUNNING` state.
* processes in `TASK_STOPPED`, `EXIT_DEAD` or `EXIT_ZOMBIE` are not grouped by any lists.
* processes in `TASK_INTERRUPTABLE` or `TASK_UNINTERRUPTABLE` state are stored by additional lists - wait queues

### wait queues
* wait queues implements conditional waits on events
	* a process wishing to wait for a specific event places itself in the proper wait queue and relinquishes control	

```c
struct _ _wait_queue_head {
	spinlock_t lock;
	struct list_head task_list;
};
typedef struct _ _wait_queue_head wait_queue_head_t;
```

## references
* https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/types.h?h=v6.0-rc5
* https://blog.csdn.net/shenwanjiang111/article/details/105355016
