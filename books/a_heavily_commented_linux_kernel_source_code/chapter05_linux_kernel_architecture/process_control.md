# process control

* a task consists of:
    * code section
    * data section
    * stack section

* a task can be executed in either:
    * kernel mode
    * user mode

## task struct

```c
// include/linux/sched.h
struct task_struct {
/* these are hardcoded - don't touch */
	long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	long counter;
	long priority;
	long signal;
	struct sigaction sigaction[32];
	long blocked;	/* bitmap of masked signals */
/* various fields */
	int exit_code;
	unsigned long start_code,end_code,end_data,brk,start_stack;
	long pid,pgrp,session,leader;
	int	groups[NGROUPS];
	/* 
	 * pointers to parent process, youngest child, younger sibling,
	 * older sibling, respectively.  (p->father can be replaced with 
	 * p->p_pptr->pid)
	 */
	struct task_struct	*p_pptr, *p_cptr, *p_ysptr, *p_osptr;
	unsigned short uid,euid,suid;
	unsigned short gid,egid,sgid;
	unsigned long timeout,alarm;
	long utime,stime,cutime,cstime,start_time;
	struct rlimit rlim[RLIM_NLIMITS]; 
	unsigned int flags;	/* per process flags, defined below */
	unsigned short used_math;
/* file system info */
	int tty;		/* -1 if no tty, so it must be signed */
	unsigned short umask;
	struct m_inode * pwd;
	struct m_inode * root;
	struct m_inode * executable;
	struct m_inode * library;
	unsigned long close_on_exec;
	struct file * filp[NR_OPEN];
/* ldt for this task 0 - zero 1 - cs 2 - ds&ss */
	struct desc_struct ldt[3];
/* tss for this task */
	struct tss_struct tss;
};
```

![image](https://github.com/lolyu/aoi/assets/35479537/47dfa409-d598-410f-acf8-e1a89c02fccf)


* task context:
    * all CPU registers
    * process state
    * stack content

* task attributes:
    * `counter`: number of time ticks that the process can execute before it is scheduled out, initial value of `counter` is equal to the `priority`
    * `priority`: the initial value of `counter`, in Linux 0.12, it is 15.

### task states
* all five states:
    * `TASK_RUNNING`: being executed or is ready to be executed
    * `TASK_INTERRUPTABLE`: waiting for an interrupt, resource, or signal
    * `TASK_UNINTERRUPTABLE`: waiting for awake call by `wake_up()`
    * `TASK_ZOMBIE`: stopped running, but its parent doesn't call `wait()` to get its exit status
    * `TASK_STOPPED`: stopped by `SIGSTOP`, `SIGCONT` could be sent to the task to transition into a runnable state.
![image](https://github.com/lolyu/aoi/assets/35479537/dc6a650e-97ec-45e2-9e19-4c42b851e8dc)

* **NOTES**:
    * The kernel will only perform process switching operations when the process moves from "kernel running state" to "sleep state". Processes running in kernel mode cannot be preempted by other processes, and one process cannot change the state of another process.
    * In order to avoid kernel data errors during process switching, the kernel will disable all interrupts when executing critical area code.

## task init


## task creation

## task scheduling
```c
// kernel/sched.c
void schedule(void)
{
	int i,next,c;
	struct task_struct ** p;

/* this is the scheduler proper: */

	while (1) {
		c = -1;
		next = 0;
		i = NR_TASKS;
		p = &task[NR_TASKS];
                // find the task with the largest counter value to run
		while (--i) {
			if (!*--p)
				continue;
			if ((*p)->state == TASK_RUNNING && (*p)->counter > c)
				c = (*p)->counter, next = i;
		}
                // found
		if (c) break;
                // not found, reassign all tasks' counter
                // NOTE: sleep task with non-zero counter will have a larger counter after reassignment
		for(p = &LAST_TASK ; p > &FIRST_TASK ; --p)
			if (*p)
				(*p)->counter = ((*p)->counter >> 1) +
						(*p)->priority;
	}
	switch_to(next);
}
```

### task switching
![image](https://github.com/lolyu/aoi/assets/35479537/8cb88b33-4bd7-445e-959d-db076d24e73b)


## task termination

## references

