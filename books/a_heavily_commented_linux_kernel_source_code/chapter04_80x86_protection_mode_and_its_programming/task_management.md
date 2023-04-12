# task management
* A task is a work unit that a processor could allocate to schedule, execute, and suspend
* two kinds of task-related descriptors in the descriptor table:
    * task state segment descriptor
    * task gate

## task structure
* a task consists of two parts:
    * the task execution space
    * the task state segment(TSS)

![image](https://user-images.githubusercontent.com/35479537/231465692-52ffc251-d3e2-4145-8575-4b9bf48f9b6e.png)

* the state of currently running task:
    * the task's current execution space, stored in the segments pointed by the segment selectors in the segment registers:
        * `CS`
        * `DS`
        * `SS`
        * `ES`
        * `FS`
        * `GS`
    * the state of general purpose registers
    * the state of `EFLAGS`, `EIP`, control register `CR3`, task register and `LDTR` register
    * the I/O map base address and I/O map(in the `TSS`)
    * stack pointers to privilege 0, 1, and 2 stacks(in the `TSS`)
    * link to priviously executed task(in the `TSS`)

## task execution
* the OS or the processor could dispatch a task for execution in one of the following methods:
    * a explicit call to a task with `CALL` instruction
    * a explicit jump to a task with `JMP` instruction
    * an implicit call to an interrupt-handler task
    * an implicit call to an exception-handler task
    * a return(`IRET`) when the `NT` flag in the `EFLAGS` register is set
* task switching uses either:
    * a selector that points to the task gate
    * a selector that points to a `TSS`
* when a task switching happens, the execution environment(task context) of the currently running task is saved to its `TSS`, and the execution of the task is suspended, the context of the newly scheduled task is then loaded into the processor and a new task is executed from the loaded `EIP`
* If the currently executing task (the caller) invokes the scheduled new task (the callee), then the caller's `TSS` segment selector is stored in the callee `TSS`, providing a link back to the caller. For all 80X86 processors, tasks are not recursively called, ie tasks cannot be called or jumped to themselves.
* as part of the task switching, the processor also switches to another `LDT`, so different task have different logical-to-physical address mapping, at the same time, the page directory register CR3 is also reloaded at the time of switching, so each task can have its own set of page tables, these protections can be used to isolate individual tasks and prevent them from interfering with each other

## task management data structures
* the processor defines the following registers and data structures that support multitasking:
    * task-state segment(`TSS`)
    * `TSS` descriptor
    * task register(`TR`)
    * task-gate descriptor
    * `NT` flag in the `EFLAGS` register

### task-state segment
![image](https://user-images.githubusercontent.com/35479537/231478532-dad1c838-c0b3-405e-950e-fcb37cf87b54.png)

* task-state segment has two parts: the dynamic fields and static fields



## references
* https://en.wikipedia.org/wiki/Control_register
