# c function call basics

![image](https://user-images.githubusercontent.com/35479537/222956232-bdb2a51c-a853-4380-addf-ed405a03843b.png)
* the stack frame:
    * `ebp`: frame pointer
    * `esp`: stack pointer

* the stack frame is responsible for:
    * storing a function's local variables
    * storing

* when function `A` calls function `B`:
    * the parameters passed to `B` are pushed onto the stack(in the stack frame of `A`)
    * the return address of function `A` is pushed onto the stack
    * `ebp` is pushed onto the stack(`A`'s ebp value)
    * the stack frame of `B` starts from the subsequent stack section


## references
* https://thecandcppclub.com/deepeshmenon/chapter-4-the-stacks-in-c/227/
