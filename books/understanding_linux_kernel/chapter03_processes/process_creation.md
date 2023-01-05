# process creation

* traditionally, resouces owned by the parent process are duplicated in the child process, which is not efficient
* three different mechanisms to resolve the inefficiency:
    * copy on write
        * allows both the parent and the child task to read the same physical pages, whenever either one tries to write on a physical page, the kernel copies its contents into a new physical page that is assigned to the writing process.
    * lightweight process
        * allows both the parent and the child to share many per-process kernel data structure, such as page tables, the open file tables, and the signal positions
    * `vfork`
        * allow process creation that shares the memory address space of its parent
        * the parent's execution is blocked until the child exists or executes a new program, this is to prevent the parent from overwriting data needed by the child.

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
    * `parent_tid`: stores the child thread ID with `CLONE_PARENT_SETTID` in both parent memory
    * `tls`: address of a data structure that defines a thread local storage segment for the new lightweight process
    * `child_tid`: stores child thread ID with `CLONE_CHILD_SETTID` in child memory


## do_fork
```c
long do_fork(unsigned long clone_flags,
	      unsigned long stack_start,
	      struct pt_regs *regs,
	      unsigned long stack_size,
	      int __user *parent_tidptr,
	      int __user *child_tidptr)
{
	struct task_struct *p;
	int trace = 0;
	long pid = alloc_pidmap();

	if (pid < 0)
		return -EAGAIN;
	if (unlikely(current->ptrace)) {
		trace = fork_traceflag (clone_flags);
		if (trace)
			clone_flags |= CLONE_PTRACE;
	}

	p = copy_process(clone_flags, stack_start, regs, stack_size, parent_tidptr, child_tidptr, pid);
	/*
	 * Do this prior waking up the new thread - the thread pointer
	 * might get invalid after that point, if the thread exits quickly.
	 */
	if (!IS_ERR(p)) {
		struct completion vfork;

		if (clone_flags & CLONE_VFORK) {
			p->vfork_done = &vfork;
			init_completion(&vfork);
		}

		if ((p->ptrace & PT_PTRACED) || (clone_flags & CLONE_STOPPED)) {
			/*
			 * We'll start up with an immediate SIGSTOP.
			 */
			sigaddset(&p->pending.signal, SIGSTOP);
			set_tsk_thread_flag(p, TIF_SIGPENDING);
		}

		if (!(clone_flags & CLONE_STOPPED))
			wake_up_new_task(p, clone_flags);
		else
			p->state = TASK_STOPPED;

		if (unlikely (trace)) {
			current->ptrace_message = pid;
			ptrace_notify ((trace << 8) | SIGTRAP);
		}

		if (clone_flags & CLONE_VFORK) {
			wait_for_completion(&vfork);
			if (unlikely (current->ptrace & PT_TRACE_VFORK_DONE))
				ptrace_notify ((PTRACE_EVENT_VFORK_DONE << 8) | SIGTRAP);
		}
	} else {
		free_pidmap(pid);
		pid = PTR_ERR(p);
	}
	return pid;
}
```
* if `CLONE_STOPPED` flag is not set, `do_fork` invokes the `wake_up_new_task()`

### wake_up_new_task
* `wake_up_new_task` basically performs the following operations
    * adjust the scheduling parameters of both the parent and the child
    * if the child will run on the same CPU as the parent and no `CLONE_VM` flag set, it will force the child to run before the parent by inserting the child into the parent's runqueue right before the parent
        * because if the parent run first, the copy on write would give rise to a series of unnecessary page duplications
    * otherwise, if the child will not be run on the same CPU as the aprent or `CLONE_VM` is set, it inserts the child in the last position of the parent's runqueue

```c
void fastcall wake_up_new_task(task_t * p, unsigned long clone_flags)
{
    ...
	if (likely(cpu == this_cpu)) {
		if (!(clone_flags & CLONE_VM)) {
			/*
			 * The VM isn't cloned, so we're in a good position to
			 * do child-runs-first in anticipation of an exec. This
			 * usually avoids a lot of COW overhead.
			 */
			if (unlikely(!current->array))
				__activate_task(p, rq);
			else {
				p->prio = current->prio;
				list_add_tail(&p->run_list, &current->run_list);
				p->array = current->array;
				p->array->nr_active++;
				rq->nr_running++;
			}
			set_need_resched();
		} else
			/* Run child last */
			__activate_task(p, rq);
		/*
		 * We skip the following code due to cpu == this_cpu
	 	 *
		 *   task_rq_unlock(rq, &flags);
		 *   this_rq = task_rq_lock(current, &flags);
		 */
		this_rq = rq;
	}
    ...
}

static inline void __activate_task(task_t *p, runqueue_t *rq)
{
	enqueue_task(p, rq->active);
	rq->nr_running++;
}

static void enqueue_task(struct task_struct *p, prio_array_t *array)
{
	sched_info_queued(p);
	list_add_tail(&p->run_list, array->queue + p->prio);
	__set_bit(p->prio, array->bitmap);
	array->nr_active++;
	p->array = array;
}
```
* note that `enqueue_task` simply puts the child task into the last position of the parent's runqueue



## references
