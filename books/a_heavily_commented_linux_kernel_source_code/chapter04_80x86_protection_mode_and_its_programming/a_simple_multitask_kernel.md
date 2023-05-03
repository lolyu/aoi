# a simple multitask kernel
* the kernel consists of:
    * bootloader `boot.s`
    * kernel program `head.s`

![image](https://user-images.githubusercontent.com/35479537/233770738-2578e5ce-ec51-4529-ac90-19b3940de67c.png)

![image](https://user-images.githubusercontent.com/35479537/233770748-192096b5-3c0a-45b4-8dc7-161e5d2057eb.png)

* how it runs?
    * when the PC is powered on, the program in the ROM BIOS will load the first sector on the boot disk to the beginning of the physical memory `0x7c00`(31KB) position, and transfer the execution control to `0x7c00` to start running the boot code
    * `boot.s` loads the kernel code(`head.s`) into memory location `0x10000`, then copy to `0x0`
        * why `boot.s` simply copies the kernel code into `0x0` directly?
            * the interrupt vector table used by the `BIOS` is loaded into `0x0`
            * the load process requires the interrupt vector table provided by the `BIOS`
            * if directly load the `head.s` into `0x0`, this will destroy the interrupt vector table
    * enter protected mode, then jump to `0x0` to start running `head.s`, `head.s` operates in 32-bit protected mode, and it mainly does the followings:
        * reset the `GDT` table
        * set the system timer chip
        * reset the `IDT` table and set the clock and system call interrupt gate
        * move to task `A` for execution

![image](https://user-images.githubusercontent.com/35479537/233771049-a171077e-633e-40e2-8994-baed4873fa4b.png)

![image](https://user-images.githubusercontent.com/35479537/233772418-5bf64419-5e0b-48fc-972f-adb0806994fe.png)


## real mode
* the segment selector and the offset are both pointers of 16 bits
* linear address is the same as physical adderss in real mode
* the segment selector is interpreted as the most significant 16 bits of a linear 20-bits address
![image](https://user-images.githubusercontent.com/35479537/233884440-5d234ef2-fa2b-4f29-aaad-74c7d72f6c6f.png)


## head.s explained

## the memory layout
![simple_multitask_kernel](https://user-images.githubusercontent.com/35479537/235480894-b53f6774-85ef-46c1-80e9-e6fc5fe080be.jpg)

### `lss`
```assembly
lss init_stack, %esp
...
init_stack:                          # Will be used as user stack for task0.
	.long init_stack
	.word 0x10
```
* `LSS` loads `SS:r32` with far pointer from memory.
* so `ss` will have value 0x10, and `esp` will have value `init_stack`

### setup_idt
![image](https://user-images.githubusercontent.com/35479537/235299109-ae1a4006-e50a-4c58-a6c6-c53d675bc26a.png)
* `setup_idt`:
	* use `eax` and `edx` to store gate descriptor 0-3 bytes and 4-7 bytes, and the gate descriptor points to `ignore_int`
	* all 256 gate descriptors have the same gate descriptor that points to `ignore_int`

```assembly
setup_idt:
	lea ignore_int,%edx				# edx as the address of procedure ignore_int, stored in the lower 16 bits
	movl $0x00080000,%eax				# segment selector as 0x0008
	movw %dx,%ax					# store the address of ignore_int in the lower 16 bits of eax
	movw $0x8E00,%dx				# 1000 1110: 1110 as 32 bit gate descriptor, 0 as DPL, 1 as P(present)
	lea idt,%edi
	mov $256,%ecx
rp_sidt:
	movl %eax,(%edi)
	movl %edx,4(%edi)
	addl $8,%edi
	dec %ecx
	jne rp_sidt
	lidt lidt_opcode				# lidt loads six bytes into IDTR, 2 bytes limit and 4 bytes base address
	ret

...

lidt_opcode:
	.word 256*8-1					# idt contains 256 entries
	.long idt					# This will be rewrite by code. 

...

idt:
	.fill 256,8,0					# idt is uninitialized, 256 gate descriptors, 8 bytes each, 2KB total
```

### setup_gdt
```assembly
setup_gdt:
	lgdt lgdt_opcode
	ret

...

lgdt_opcode:
	.word (end_gdt-gdt)-1				# so does gdt 
	.long gdt					# This will be rewrite by code.

gdt:
	.quad 0x0000000000000000	/* NULL descriptor */
	.quad 0x00c09a00000007ff	/* 8Mb 0x08, base = 0x00000 */
	.quad 0x00c09200000007ff	/* 8Mb 0x10 */
	.quad 0x00c0920b80000002	/* screen 0x18 - for display */

	.word 0x0068, tss0, 0xe900, 0x0	# TSS0 descr 0x20
	.word 0x0040, ldt0, 0xe200, 0x0	# LDT0 descr 0x28
	.word 0x0068, tss1, 0xe900, 0x0	# TSS1 descr 0x30
	.word 0x0040, ldt1, 0xe200, 0x0	# LDT1 descr 0x38
end_gdt:
	.fill 128,4,0
```

### timer_interrupt
* `timer_interrupt` is acted as a task scheduled in this kernel.
* when working in protected mode, `jmp` could be used to do a task switch:
	* Executing a task switch with the JMP instruction is somewhat similar to executing a jump through a call gate. Here the target operand specifies the segment selector of the task gate for the task being switched to (and the offset part of the target operand is ignored). The task gate in turn points to the TSS for the task, which contains the segment selectors for the task’s code and stack segments. The TSS also contains the EIP value for the next instruction that was to be executed before the task was suspended. This instruction pointer value is loaded into the EIP register so that the task begins executing again at this next instruction.
```assembly
timer_interrupt:
	push %ds					# as ds and eax are modified in the interrupt handler
	pushl %eax					# push onto the stack to store its original values
	movl $0x10, %eax
	mov %ax, %ds
	movb $0x20, %al
	outb %al, $0x20
	movl $1, %eax
	cmpl %eax, current				# check if the current task is #1
	je 1f						# if task is #1, goto 1f
	movl %eax, current				# if the current task is #0, assign current with 1f
	ljmp $TSS1_SEL, $0				# long jump to the tss segment selector for TSS1
	jmp 2f						# goto 2f
1:	movl $0, current				# if the current task is #1, assign current with 0f
	ljmp $TSS0_SEL, $0				# long jump to the tss segment selector for TSS0
2:	popl %eax					# restore original eax
	pop %ds						# restore original ds
	iret
```
### system_interrupt
* the caller must save the register to be used, so the `system_interrupt` should save those registers changed by function `write_char`
```assembly
write_char:
	push %gs
	pushl %ebx
#	pushl %eax
	mov $SCRN_SEL, %ebx
	mov %bx, %gs
	movl scr_loc, %bx
	shl $1, %ebx
	movb %al, %gs:(%ebx)
	shr $1, %ebx
	incl %ebx
	cmpl $2000, %ebx
	jb 1f
	movl $0, %ebx
1:	movl %ebx, scr_loc	
#	popl %eax
	popl %ebx
	pop %gs
	ret
...

/* system call handler */
.align 2
system_interrupt:
	push %ds
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl %eax
	movl $0x10, %edx
	mov %dx, %ds
	call write_char
	popl %eax
	popl %ebx
	popl %ecx
	popl %edx
	pop %ds
	iret
```

### use `IRET` to start task execution
```
# Move to user mode (task 0)
	pushfl
	andl $0xffffbfff, (%esp)
	popfl
	movl $TSS0_SEL, %eax
	ltr %ax
	movl $LDT0_SEL, %eax
	lldt %ax 
	movl $0, current
	sti
	pushl $0x17					# SS
	pushl $init_stack				# ESP
	pushfl						# EFLAGS
	pushl $0x0f					# CS
	pushl $task0					# EIP
	iret
```

![image](https://user-images.githubusercontent.com/35479537/235851114-1e0c607d-9cbf-47e1-9ce5-3a0c999919af.png)


## workflow
* the workflow of the this multitask kernel
	* `boot.s` jumps to the code segment of `head.s`
		* `jmpi 0,8` transfer the execution to the segment descriptor `0x8` with offset `0`
			* so `CS` will be loaded with the code segment located as the second entry in `GDT`
			* `EIP` will be loaded with value `0`
	* setup `IDT`
	* setup `GDT`
	* setup general purpose registers and stack
		* `DS` as `0x10`
		* `ES` as `0x10`
		* `FS` as `0x10`
		* `GS` as `0x10`
		* `SS` as `0x10`
		* `ESP` as `init_stack`
	* setup 8253 chip with interrupt interval as 10ms
	* setup IDT interrupt handler and trap handler entry
		* 8th entry as `timer_interrupt` to handle interrupt
		* 128th entry as `system_interrupt` to handle trap
	* setup the `IRET` environment(`TR`, `LDTR` and stack) and use `IRET` to start executing `task0`
		* `TR` as `tss0`
		* `LDTR` as `ldt0`
		* push `SS`, `ESP`, `EFLAGS`, `CS`, and `EIP` onto the stack so `IRET` will restore those registers for `task0`
	* `task0` execution
		* load `DS` as it is not initialized yet
		* print `A`
	* `timer_interrupt` execution
		* x86 processors will load `CS` and `EIP` from the `IDT` entry
			* from `CS` and `EIP`, OS could locate the handler procedue
		* long jump with a TSS segment selector will conduct a task switch
		* if `current` == 1, `ljmp $TSS0_SEL`
		* if `current` == 0, `ljmp $TSS1_SEL`

## references
* https://github.com/lolyu/aoi/blob/master/books/understanding_linux_kernel/chapter02_memory_address/x86_segmentation.md
* https://wiki.osdev.org/Real_Mode#High_Memory_Area
* https://stackoverflow.com/questions/27804852/assembly-rep-movs-mechanism
* https://www.felixcloutier.com/x86/lds:les:lfs:lgs:lss
* https://wiki.osdev.org/Interrupt_Descriptor_Table
* https://www.felixcloutier.com/x86/lgdt:lidt
* https://www.felixcloutier.com/x86/jmp
* https://iitd.github.io/os/2022/lec/l7.html
