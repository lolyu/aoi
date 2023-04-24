# a simple multitask kernel
* the kernel consists of:
    * bootloader `boot.s`
    * kernel program `head.s`

![image](https://user-images.githubusercontent.com/35479537/233770738-2578e5ce-ec51-4529-ac90-19b3940de67c.png)

![image](https://user-images.githubusercontent.com/35479537/233770748-192096b5-3c0a-45b4-8dc7-161e5d2057eb.png)

* how it runs?
    * when the PC is powered on, the program in the ROM BIOS will load the first sector on the boot disk to the beginning of the physical memory `0x7c00`(31KB) position, and transfer the execution control to `0x7c00` to start running the boot code

![image](https://user-images.githubusercontent.com/35479537/233771049-a171077e-633e-40e2-8994-baed4873fa4b.png)

![image](https://user-images.githubusercontent.com/35479537/233772418-5bf64419-5e0b-48fc-972f-adb0806994fe.png)


## references


