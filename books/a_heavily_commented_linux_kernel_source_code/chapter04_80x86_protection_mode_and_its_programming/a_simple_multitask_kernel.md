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
	movl $0x00080000,%eax			# segment selector as 0x0008
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
	.long idt						# This will be rewrite by code. 

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
	.word (end_gdt-gdt)-1			# so does gdt 
	.long gdt						# This will be rewrite by code.

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

## references
* https://github.com/lolyu/aoi/blob/master/books/understanding_linux_kernel/chapter02_memory_address/x86_segmentation.md
* https://wiki.osdev.org/Real_Mode#High_Memory_Area
* https://stackoverflow.com/questions/27804852/assembly-rep-movs-mechanism
* https://www.felixcloutier.com/x86/lds:les:lfs:lgs:lss
* https://wiki.osdev.org/Interrupt_Descriptor_Table
* https://www.felixcloutier.com/x86/lgdt:lidt
