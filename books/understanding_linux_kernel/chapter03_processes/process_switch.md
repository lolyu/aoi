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

### task state segment descriptor
![image](https://user-images.githubusercontent.com/35479537/208221710-48cfba09-d97f-40cf-8949-da827bcb7b32.png)

![image](https://user-images.githubusercontent.com/35479537/208221776-77a0c41c-f586-4876-9877-6dd631447f93.png)



## references
* https://pdos.csail.mit.edu/6.828/2005/readings/i386/s07_02.htm
