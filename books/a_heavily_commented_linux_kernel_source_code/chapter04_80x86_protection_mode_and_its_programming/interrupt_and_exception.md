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


## references
*
