# kernel architecture

![image](https://github.com/lolyu/aoi/assets/35479537/d45fc61e-306d-4565-8490-a4bbfc23bc90)

* two kernel types:
    * micro kernel
        * in microkernels, the kernel is broken down into separate processes, known as servers. Some of the servers run in kernel space and some run in user-space. All servers are kept separate and run in different address spaces. Servers invoke "services" from each other by sending messages via IPC (Interprocess Communication).
    * monolithic kernel
        * monolithic kernel is a single large process running entirely in a single address space. It is a single static binary file. All kernel services exist and execute in the kernel address space.


## linux kernel system architecture
* the linux kernel consists of five modules:
    * process scheduling module
    * memory management module
    * file system module
    * communication module
    * network module

![image](https://github.com/lolyu/aoi/assets/35479537/47db396f-2f5b-45c1-9378-c2db3b6c96df)

![image](https://github.com/lolyu/aoi/assets/35479537/07eaa3dd-1aa9-4209-8fe9-dd124780c8f8)


## references
