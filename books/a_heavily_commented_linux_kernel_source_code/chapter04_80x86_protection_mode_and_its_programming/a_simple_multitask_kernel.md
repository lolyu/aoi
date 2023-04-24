# a simple multitask kernel
* the kernel consists of:
    * bootloader `boot.s`
    * kernel program `head.s`

![image](https://user-images.githubusercontent.com/35479537/233770738-2578e5ce-ec51-4529-ac90-19b3940de67c.png)

![image](https://user-images.githubusercontent.com/35479537/233770748-192096b5-3c0a-45b4-8dc7-161e5d2057eb.png)

* how it runs?
    * when the PC is powered on, the program in the ROM BIOS will load the first sector on the boot disk to the beginning of the physical memory `0x7c00`(31KB) position, and transfer the execution control to `0x7c00` to start running the boot code
    * `boot.s` loads the kernel code(`head.s`) into memory location `0x10000`, then copy to `0x0`
    * enter protected mode, then jump to `0x0` to start running `head.s`

![image](https://user-images.githubusercontent.com/35479537/233771049-a171077e-633e-40e2-8994-baed4873fa4b.png)

![image](https://user-images.githubusercontent.com/35479537/233772418-5bf64419-5e0b-48fc-972f-adb0806994fe.png)


## real mode
* the segment selector and the offset are both pointers of 16 bits
* linear address is the same as physical adderss in real mode
* the segment selector is interpreted as the most significant 16 bits of a linear 20-bits address
![image](https://user-images.githubusercontent.com/35479537/233884440-5d234ef2-fa2b-4f29-aaad-74c7d72f6c6f.png)


## references
* https://github.com/lolyu/aoi/blob/master/books/understanding_linux_kernel/chapter02_memory_address/x86_segmentation.md
* https://wiki.osdev.org/Real_Mode#High_Memory_Area
* https://stackoverflow.com/questions/27804852/assembly-rep-movs-mechanism
