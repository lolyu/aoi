# interrupt and exception
* `interrupt` and `exception` are events that occurs somewhere in the system, processor, or current executor(or task) that need to be processed by the processor
* when an interrupt is received or an exception is detected, the processor automatically suspends the currently executing program or task and begins running the interrupt or exception handler, the recovery process of the interrupted program does not lose the continuity of program execution unless it is impossible to recover from the exception or the interrupt causes the current program to be terminated.

## vector
* `vector`: each defined exception and interrupt that needs processed by the processor is given an idenfication number
* `vector` is used by the processor as an index number in the Interrupt Descriptor Table(IDT) to locate an exception or interrupt handler entry point location

![image](https://user-images.githubusercontent.com/35479537/230762332-8d38e604-4108-4d89-b10a-69723dbfb7d8.png)
![image](https://user-images.githubusercontent.com/35479537/230762342-9e36166f-c280-40b1-b7a0-8e7fc5a405b4.png)


## interrupt and exception source

### interrupt source
* the processor receives interrupts from two places:
    * external(hardware generated) interrupts
        * received by two pins (`INTR` and `NMI`)
            * `NMI` receives non-maskable interrupt
    * software generated interrupts
        * `INT` instruction

### exception sources
* two sources of exceptions:
    * processor-detected program error exceptions
    * software-generated exceptions

#### exception category
* three types of exception:
    * `fault`
        * a fault is an exception that can usually be corrected and be continued to run once it has been corrected
        * the return address of the exception handler will point to the instruction that generates the fault, so the instruction that generated the fault could be re-executed
        * page fault
    * `trap`
        *  A trap occurs as a result of a userspace (CPL=3) program execution actions (see the table above) that result in a “trap” to the highest privilege level. A trap is an event (`fault`) that is reported immediately after the trapping instruction.
        *  the return address of the exception handler will point to the next instruction
    * `abort`
        * Abort is an exception that does not always report the exact location of the instruction that caused the exception, and does not allow the program that caused the exception to resume execution


## program or task restart
* three situations:
    * fault
        * as after the exception handler returns to the instruction that generates the fault, fault is usually used to handle the case that the instruction operation is blocked
        * page fault: when the program references an operand that is not on a page in memory
            * the page fault handler will load the page into memory and resume program execution by re-execute the faulting instruction
    * trap
        * if a trap is detected during the execution of an instruction that performs a control transfer, the return instruction pointer will store the target location of the control transfer
    * abort
        * a handler that aborts an exception is typically used to collect diagnostic information about the state of the processor when the exception occurred, and close the program and system as appropriately as possible

## exception and interrupt priority
* if there are multiple exceptions or interrupts, the processor will process them in a specified order
    * The processor first processes exceptions or interrupts in the highest priority class. Low priority exceptions are discarded, while low priority interrupts are held waiting.

![image](https://user-images.githubusercontent.com/35479537/230772397-55509b5d-42df-4449-a32f-3a7cbd860356.png)
![image](https://user-images.githubusercontent.com/35479537/230772409-7c883800-65a1-4d6a-bb5c-6ce45b703804.png)


## interrupt descriptor table
* interrupt descriptor table(IDT) stores the mapping between exception or interrupt vector with a gate descriptor, which is used to handle related exceptions or interrupts

![image](https://user-images.githubusercontent.com/35479537/230772546-7c238764-3955-476d-867a-ed98f1b16701.png)

### IDT descriptor
* three kinds of gate descriptors:
    * interrupt gate descriptor
    * trap gate descriptor
    * task gate descriptor

* the interrupt gate and trap gate are used to transfer the program execution to exception or interrupt handler code segment

![image](https://user-images.githubusercontent.com/35479537/230772659-1a76ba2b-9928-4907-97b6-b1fb99a42ec3.png)

### exception and interrupt handling
![image](https://user-images.githubusercontent.com/35479537/230772766-34f01431-9ff8-4013-bc4f-d4ddb8680c13.png)

* the invoking of exception or interrupt handlers is similar to calling a procedure using `CALL` instruction
    * when responding to an exception or interrupt, processor uses the vector as an index in the `LDT` table:
        * if the index value points to an interrupt gate or trap gate, the processor invokes an exception or interrupt handler using a method similar to the `CALL` instruction operation call gate
        * if the index value points to a task gate, the processor performs a task switching using a method similar to the `CALL` instruction operation task gate and performs an exception or interrupt processing task
* an exception or interrupt gate references an exception or interrupt handler that runs in the context of the current task.
* two situations:
    * handler runs in the same privilege level of the current task
    * handler runs in a higher privilege level

![image](https://user-images.githubusercontent.com/35479537/230775273-b08744c6-0a2c-4251-843a-f9fbd79d15e9.png)

* The only difference between an interrupt gate and a trap gate is the way the processor operates the IF flag of the EFLAGS register. When an exception or interrupt handler is accessed through the interrupt gate, the processor resets the IF flag to prevent other interrupts from interfering with the current interrupt handler. Subsequent `IRET` instructions will restore the IF flag of the EFLAGS register with the contents stored on the stack. Accessing the handler procedure through the trap gate does not affect the IF flag.


### interrupt handler tasks
* interrupt handler task is not used in Linux

### error code
* When an exception condition is associated with a particular segment, the processor pushes an error code onto the stack of exception handler

![image](https://user-images.githubusercontent.com/35479537/230776125-e97dc8cd-24a0-417f-98a3-4ecfa50f1a66.png)
* three flags:
    * `EXT`: external event, indicating that an event external to the program caused an exception
    * `IDT`: when set, the index is used to index a gate descriptor in the `IDT`, when reset, the index is to index a descriptor in `LDT`/`GDT`
    * `TI`: `GDT`/`LDT` select flag, if set, the index is to index `LDT`, if reset, the index is to index `GDT`
 
 #### page fault error code
 
 ![image](https://user-images.githubusercontent.com/35479537/230776362-ca6d5c77-fe24-47ea-bf29-f8e7f3502783.png)
 
 * three flags:
    * `P`: if set, page fault is due to page-level protection check failure, if reset, page fault is due to page does not exist
    * `W/R`: if set, the page fault is caused by a write op, if reset, the page fault is caused by a read op
    * `U/S`: if set, the CPU is executing the super user code, if reset, the CPU is executing the general user code
 
* the linear address causing the page fault is stored in `CR2` by the processor


## references
* https://en.wikipedia.org/wiki/FLAGS_register
* https://wiki.osdev.org/Task_State_Segment
* https://en.wikipedia.org/wiki/Control_register
