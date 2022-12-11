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


* storing the kernel stack and thread info together makes it easy to derive the current process from `esp` easily.


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

* `pid_chain` stores the the next and previous process that shares the same hashed pid value
* `pid_list` stores next and previous process that shares the same `tgid`, `PGID` or `SID`

```c
struct pid
{
	/* Try to keep pid_chain in the same cacheline as nr for find_pid */
	int nr;
	struct hlist_node pid_chain;
	/* list of pids with the same nr, only one of them is in the hash */
	struct list_head pid_list;
};
```

## how processes are organized
* runqueue lists group all processes in `TASK_RUNNING` state.
* processes in `TASK_STOPPED`, `EXIT_DEAD` or `EXIT_ZOMBIE` are not grouped by any lists.
* processes in `TASK_INTERRUPTABLE` or `TASK_UNINTERRUPTABLE` state are stored by additional lists - wait queues

### wait queues
* wait queues implements conditional waits on events
	* a process wishing to wait for a specific event places itself in the proper wait queue and relinquishes control	
* `wait_queue_head_t` represents the queue as a whole. It is the head of the waiting queue.
* `wait_queue_t` represents the item of the list - a single process waiting in the queue.
	* `flags`
		* `0`: exclusive processes, selectively woken up by the kernel when the event occurs
		* `1`: nonexclusive processes, always woken up by the kernel when the event occurs
* **QUESTION**:
	* when will the wait function be executed?
```c
struct _ _wait_queue_head {
	spinlock_t lock;
	struct list_head task_list;
};
typedef struct _ _wait_queue_head wait_queue_head_t;

struct _ _wait_queue {
	unsigned int flags;
	struct task_struct * task;
	wait_queue_func_t func;
	struct list_head task_list;
};
typedef struct _ _wait_queue wait_queue_t;
```
![image](https://user-images.githubusercontent.com/35479537/192749093-9615d382-2637-4979-b584-0740f37ef483.png)


#### wait queue ops
* `DECLARE_WAIT_QUEUE_HEAD(name)`: declare a new wait queue head variable called `name`
* `init_wait_queue_head`: initialize a wait queue head variable
* `init_wait_queue_entry(p, q)`: initialize a `wait_queue_t` structure `q` as follows:
	* `q->flags = 0;`
	* `q->task = p;`
	* `q->func = default_wake_function;`
* `DEFINE_WAIT`: declare a new `wait_queue_t` variable and initializes it with the descriptor of the process currently running on the CPU
	* use `autoremove_wake_function()` as wake function
* `init_waitqueue_func_entry()`: initialize a `wait_queue_t` structure with a custom awakening function.
* `add_wait_queue()`: inserts a nonexclusive process in the first position of a wait queue list
* `add_wait_queue_exclusive()`: inserts an exclusive process in the last position of a wait queue list
* `remove_wait_queue()`: removes a process from a wake queue list
* `waitqueue_active()`: check if a given wait queue is empty

##### how does a process wait for a specific condition
* `sleep_on()`:
	1. put the current process on a wait queue as `TASK_UNINTERRUPATBLE`
	2. schedule other processes to run
	3. when the process is awakened, remove the process from the awaken queue
```c
void sleep_on(wait_queue_head_t *wq)
{
	wait_queue_t wait;
	init_waitqueue_entry(&wait, current);
	current->state = TASK_UNINTERRUPTIBLE;
	add_wait_queue(wq,&wait); /*  wq points to the wait queue head  */
	schedule( );
	remove_wait_queue(wq, &wait);
}
```
* `interruptable_sleep_on()`
* `sleep_on_timeout()`
* `interruptable_sleep_on_timeout()`
* `prepare_to_wait()`
* `prepare_to_wait_exclusive()`
* `finish_wait()`
```c
	// it looks like prepare_to_wait/finish_wait allows the process to wait for a certain event
    DEFINE_WAIT(wait);
    prepare_to_wait_exclusive(&wq, &wait, TASK_INTERRUPTIBLE);
                                /* wq is the head of the wait queue */
    ...
    if (!condition)
        schedule();
    finish_wait(&wq, &wait);
```
* `wait_event` && `wait_event_interruptable`
```c
    DEFINE_WAIT(_ _wait);
    for (;;) {
        prepare_to_wait(&wq, &_ _wait, TASK_UNINTERRUPTIBLE);
        if (condition)
            break;
        schedule( );
    }
    finish_wait(&wq, &_ _wait);
```

#### how does a kernel awaken up process
* the kernel awakens processes in the wait queues, put them in the `TASK_RUNNING` state:
	* `wake_up`
	* `wake_up_nr`
	* `wake_up_all`
	* `wake_up_interruptable`
	* `wake_up_interruptable_nr`
	* `wake_up_interruptable_all`
	* `wake_up_interruptable_sync`
	* `wake_up_locked`
* **NOTE**:
	* `wake_up`:
		* awaken both `TASK_INTERRUPTABLE` and `TASK_UNINTERRUPTABLE`
		* awaken all nonexclusive and one exclusive
		* check if any of the woken processes is higher than that of the process currently running in the system invoke `schedule()`
	* `_interruptable`: means awaken only processes in `TASK_INTERRUPTABLE`
	* `_nr`: means awaken at most a given number of exclusive processes
	* `_all`: means awaken all exclusive processes
	* `_sync`: means no priority check
	* `_locked`: already hold the spinlock in `wait_queue_heat_t`

```c
void wake_up(wait_queue_head_t *q)
{
	struct list_head *tmp;
	wait_queue_t *curr;

	list_for_each(tmp, &q->task_list) {
		curr = list_entry(tmp, wait_queue_t, task_list);
		if (curr->func(curr, TASK_INTERRUPTIBLE|TASK_UNINTERRUPTIBLE,
					   0, NULL) && curr->flags)
			break;
	}
}
```
* because the exclusive processes are always placed at the end of the wait queue, so if any wait entry flag is `1`(exclusive process), the loop terminates.
	* so it will awaken at most one process

## references
* https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/linux/types.h?h=v6.0-rc5
* https://blog.csdn.net/shenwanjiang111/article/details/105355016
* https://stackoverflow.com/questions/19942702/the-difference-between-wait-queue-head-and-wait-queue-in-linux-kernel
