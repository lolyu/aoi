# linux paging
* Linux adopted a four-level paging model:
    * page global directory
    * page upper directory
    * page middle directory
    * page table

![image](https://user-images.githubusercontent.com/35479537/178270006-fd9dddb5-574c-4193-88c9-5c4875facc65.png)

* when a process is executed on an x86 CPU, the CR3 register is loaded with the physical address of that process's page global directory (PGD). This is necessary so the CPU can perform translations from virtual memory address to physical memory addresses. Since every process needs its own PGD, the value in the CR3 register will be unique for each scheduled process in the system. 


## references
* https://lwn.net/Articles/106177/
* https://www.kernel.org/doc/gorman/html/understand/understand001.html
* https://carteryagemann.com/pid-to-cr3.html
