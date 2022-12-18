
## list_head vs hlist_head
* `list_head` is a typical doubly linked list
* `list_head` could be stored in user-defined structure with any name
* `list_head` is used to implement queues in the kernel
* with `list_head` structure, it is easy to iterate the list from the list head
* with `list_head` structure, it is easy to iterate the list reversely from the list head
* `hlist_head` is used to implement chaining hash tables in the kernel
    * the list won't be too long and there is no need to iterate the list reversely
* `hlist_head` is half size of `list_head` structure, thus saves memory compared to hash table with `list_head`

## core data structures
### struct `task_struct`
```c
// include/linux/sched.h
struct task_struct {
	volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	struct thread_info *thread_info;
	atomic_t usage;
	unsigned long flags;	/* per process flags, defined below */
	unsigned long ptrace;
   ...

};
```

### struct `thread_info`
```c
// include/asm-x86_64/thread_info.h
struct thread_info {
	struct task_struct	*task;		/* main task structure */
	struct exec_domain	*exec_domain;	/* execution domain */
	__u32			flags;		/* low level flags */
	__u32			status;		/* thread synchronous flags */
	__u32			cpu;		/* current CPU */
	int 			preempt_count;

	mm_segment_t		addr_limit;	
	struct restart_block    restart_block;
};
```

### struct `tss_struct`
```c
// include/asm-x86_64/processor.h
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

## references
* https://tupelo-shen.github.io/2020/02/29/Linux%E5%86%85%E6%A0%B810-list_head%E5%92%8Chlist_head%E7%9A%84%E7%90%86%E8%A7%A3/
* https://www.eecg.utoronto.ca/~amza/www.mindsec.com/files/x86regs.html
