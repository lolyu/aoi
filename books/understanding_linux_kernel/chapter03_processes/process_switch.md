# process switch
* process switch(context switch): the kernel's suspension of current running process and resume
* hardware context: the set of data that mus be loaded into registers before the process resumes its execution on CPU

* the hardware context is stored either in the process descriptor or the kernel stack
* process switching occurs only in kernel mode, the contents of all registers used by a process in user mode have already been saved on the kernel stack before performing process switching.

## task state segment
* The task state segment (TSS) is a structure on x86-based computers which holds information about a task. It is used by the operating system kernel for task management. Specifically, the following information is stored in the TSS:
    * Processor register state
    * I/O port permissions
    * Inner-level stack pointers
    * Previous TSS link

* the TSS stores information about the hardware context

```c
struct tss_struct {
	u32 reserved1;
	u64 rsp0;	
	u64 rsp1;
	u64 rsp2;
	u64 reserved2;
	u64 ist[7];
	u32 reserved3;
	u32 reserved4;
	u16 reserved5;
	u16 io_bitmap_base;
	/*
	 * The extra 1 is there because the CPU will access an
	 * additional byte beyond the end of the IO permission
	 * bitmap. The extra byte must be all 1 bits, and must
	 * be within the limit. Thus we have:
	 *
	 * 128 bytes, the bitmap itself, for ports 0..0x3ff
	 * 8 bytes, for an extra "long" of ~0UL
	 */
	unsigned long io_bitmap[IO_BITMAP_LONGS + 1];
} __attribute__((packed)) ____cacheline_aligned;
```

### task state segment descriptor
* the `TSSD`: task state segment descriptor

![image](https://user-images.githubusercontent.com/35479537/208221710-48cfba09-d97f-40cf-8949-da827bcb7b32.png)

* the segment descriptor
![image](https://user-images.githubusercontent.com/35479537/208221776-77a0c41c-f586-4876-9877-6dd631447f93.png)

* Linux keeps one TSS for each CPU instead one TSS for each process
* the `TSSD`s created by Linux are stored in the GDT(global descriptor table), whose base address is stored in the `gdtr` register of each CPU.
   * the `tr` register of each CPU contains the `TSSD` selector of the corresponding `TSS` along with two fields: the `Base` and `Limit`.
      * in this case, the processor could address the `TSS` directly without having to retrieve the `TSS` address from the `GDT`


## process switch

* every process switch consists of two steps:
   1. switching the `PGD` to install new address space
   2. switching the kernel mode stack and the hardware context

### `switch_to` macro
```c
// include/asm-x86_64/system.h
#define switch_to(prev,next,last) \
	asm volatile(SAVE_CONTEXT						    \
		     "movq %%rsp,%P[threadrsp](%[prev])\n\t" /* save RSP */	  \
		     "movq %P[threadrsp](%[next]),%%rsp\n\t" /* restore RSP */	  \
		     "call __switch_to\n\t"					  \
		     ".globl thread_return\n"					\
		     "thread_return:\n\t"					    \
		     "movq %%gs:%P[pda_pcurrent],%%rsi\n\t"			  \
		     "movq %P[thread_info](%%rsi),%%r8\n\t"			  \
		     LOCK "btr  %[tif_fork],%P[ti_flags](%%r8)\n\t"		  \
		     "movq %%rax,%%rdi\n\t" 					  \
		     "jc   ret_from_fork\n\t"					  \
		     RESTORE_CONTEXT						    \
		     : "=a" (last)					  	  \
		     : [next] "S" (next), [prev] "D" (prev),			  \
		       [threadrsp] "i" (offsetof(struct task_struct, thread.rsp)), \
		       [ti_flags] "i" (offsetof(struct thread_info, flags)),\
		       [tif_fork] "i" (TIF_FORK),			  \
		       [thread_info] "i" (offsetof(struct task_struct, thread_info)), \
		       [pda_pcurrent] "i" (offsetof(struct x8664_pda, pcurrent))   \
		     : "memory", "cc" __EXTRA_CLOBBER)
```

* macro `switch_to` has three parameters of `task_struct`:
    * `prev`: the descriptor address of the process being replaced
    * `next`: the descriptor address of the new process
    * `last`: the descriptor address of the process being replaced
* `prev`, `next`, `last` are all local variables stored in the kernel mode stack of the process being replaced
* `last` is used to facilicate the new process to find the process being replaced
    * before the process switching, `switch_to` saves `prev` into `EAX`
    * after the process switching, `switch_to` copies `EAX` into `last`
    * as the CPU register doesn't change across process switch

<img width="403" alt="image" src="https://user-images.githubusercontent.com/35479537/208283933-47733f91-9dfe-4bd5-8377-98d5dca9c985.png">

```assembly
switch_to:
        movl prev, %eax
        movl next, %edx
        pushfl
        pushl %ebp
        movl %esp, 484(%eax)
        movl 484(%edx), %esp
        movl $1f, 480(%eax)
        pushl 480(%eax)
        jmp __switch_to
        movl %eax, last

1:
        popl %ebp
        popfl
```

### __switch_to
```c
// arch/x86_64/kernel/process.c

struct task_struct *__switch_to(struct task_struct *prev_p, struct task_struct *next_p)
{
	struct thread_struct *prev = &prev_p->thread,
				 *next = &next_p->thread;
	int cpu = smp_processor_id();  
	struct tss_struct *tss = &per_cpu(init_tss, cpu);

	unlazy_fpu(prev_p);

	/*
	 * Reload esp0, LDT and the page table pointer:
	 */
	tss->rsp0 = next->rsp0;

	/* 
	 * Switch DS and ES.
	 * This won't pick up thread selector changes, but I guess that is ok.
	 */
	asm volatile("mov %%es,%0" : "=m" (prev->es));
	if (unlikely(next->es | prev->es))
		loadsegment(es, next->es); 
	
	asm volatile ("mov %%ds,%0" : "=m" (prev->ds));
	if (unlikely(next->ds | prev->ds))
		loadsegment(ds, next->ds);

	load_TLS(next, cpu);

	/* 
	 * Switch FS and GS.
	 */
	{ 
		unsigned fsindex;
		asm volatile("movl %%fs,%0" : "=r" (fsindex)); 
		/* segment register != 0 always requires a reload. 
		   also reload when it has changed. 
		   when prev process used 64bit base always reload
		   to avoid an information leak. */
		if (unlikely(fsindex | next->fsindex | prev->fs)) {
			loadsegment(fs, next->fsindex);
			/* check if the user used a selector != 0
	                 * if yes clear 64bit base, since overloaded base
                         * is always mapped to the Null selector
                         */
			if (fsindex)
			prev->fs = 0;				
		}
		/* when next process has a 64bit base use it */
		if (next->fs) 
			wrmsrl(MSR_FS_BASE, next->fs); 
		prev->fsindex = fsindex;
	}
	{ 
		unsigned gsindex;
		asm volatile("movl %%gs,%0" : "=r" (gsindex)); 
		if (unlikely(gsindex | next->gsindex | prev->gs)) {
			load_gs_index(next->gsindex);
			if (gsindex)
			prev->gs = 0;				
		}
		if (next->gs)
			wrmsrl(MSR_KERNEL_GS_BASE, next->gs); 
		prev->gsindex = gsindex;
	}

	/* 
	 * Switch the PDA context.
	 */
	prev->userrsp = read_pda(oldrsp); 
	write_pda(oldrsp, next->userrsp); 
	write_pda(pcurrent, next_p); 
	write_pda(kernelstack, (unsigned long)next_p->thread_info + THREAD_SIZE - PDA_STACKOFFSET);

	/*
	 * Now maybe reload the debug registers
	 */
	if (unlikely(next->debugreg7)) {
		loaddebug(next, 0);
		loaddebug(next, 1);
		loaddebug(next, 2);
		loaddebug(next, 3);
		/* no 4 and 5 */
		loaddebug(next, 6);
		loaddebug(next, 7);
	}


	/* 
	 * Handle the IO bitmap 
	 */ 
	if (unlikely(prev->io_bitmap_ptr || next->io_bitmap_ptr)) {
		if (next->io_bitmap_ptr)
			/*
			 * Copy the relevant range of the IO bitmap.
			 * Normally this is 128 bytes or less:
 			 */
			memcpy(tss->io_bitmap, next->io_bitmap_ptr,
				max(prev->io_bitmap_max, next->io_bitmap_max));
		else {
			/*
			 * Clear any possible leftover bits:
			 */
			memset(tss->io_bitmap, 0xff, prev->io_bitmap_max);
		}
	}

	return prev_p;
}
```

## references
* https://pdos.csail.mit.edu/6.828/2005/readings/i386/s07_02.htm
* https://wiki.osdev.org/Task_State_Segment
* https://en.wikipedia.org/wiki/X86_memory_segmentation
* https://stackoverflow.com/questions/6525905/how-does-scheduleswitch-to-functions-from-linux-kernel-actually-work
