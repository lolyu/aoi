# computer architecture

![image](https://user-images.githubusercontent.com/35479537/218302520-a7e353ce-5917-4b9d-bd55-a2ed02c29ac5.png)

* CPU communicates with other parts of system via internal bus(or local bus) consisting of:
    * address lines: used to provide the address of a memory or I/O devices
    * data lines: used to provide data transfer channels between the CPU and memory or I/O devices
    * control signal lines: used to direct specific read/write operations
* the upper controller and memory interfaces are usually integrated on a computer motherboard
* the lower part control card(or adapter) is connected to the system bus on the motherboard through an expansion slot
    * the bus slot is the standard connection interface between extended device controller and the system address bus
        * ISA (industry standard architecture)
        * EISA (extended ISA)
        * PCI (peripheral component interconnect)
        * PCIE (PCI express)
 
 ![image](https://user-images.githubusercontent.com/35479537/218302951-00157255-0aec-4322-b48e-0505c4ae3610.png)
* northbridge chips
    * interface with CPU, memory, and AGP video
    * high transmission rate
* southbridge chips
    * interface with low or medium speed components: PCI bus, IDE hard disk interface, USB port.
 

## data transmission
* data transmission between the CPU and I/O interface:
    * program loop query: consistently query the device controller of the status, waste of CPU resources
    * interrupt handling control
        * an interrupt is a request for the processor to interrupt currently running task, so that the event can be proessed in a timely manner.
        * if the interrupt request is accepted, the processor suspends the current running task, save its sate, and execute a function called an interrupt handler to deal with the event.
    * DMA transfer: used to transfer data between the I/O device and the system memory

* I/O port addressing
    * unified addressing: maps the I/O port addresses into the memory addressing address space
    * independent addressing: treat the I/O port addresse space as a separate address space

* the I/O ports access can be seen from the userspace via the `/proc/ioports` file:
```
$ cat /proc/ioports
0000-001f : dma1
0020-0021 : pic1
0040-005f : timer
0060-006f : keyboard
0070-0077 : rtc
0080-008f : dma page reg
00a0-00a1 : pic2
00c0-00df : dma2
00f0-00ff : fpu
0170-0177 : ide1
01f0-01f7 : ide0
0376-0376 : ide1
0378-037a : parport0
037b-037f : parport0
03c0-03df : vga+
03f6-03f6 : ide0
03f8-03ff : serial
...
```


## memory
![image](https://user-images.githubusercontent.com/35479537/218305460-3aa46b5b-3440-4866-a3b7-78abfb1e0e53.png)

* in order to be compatible with the original PC in software, the allocation of physical memory below the system 1MB still remains basically the same as the original PC, but the BIOS of the original system ROM has always been the highest in the memory that the CPU can address.
* all memory except 0xA0000 ~ 0xFFFFF and 0xFFFE0000 ~ 0xFFFFFFFF can be used as system memory
* 0xA0000 ~ 0xFFFFF and 0xFFFE0000 ~ 0xFFFFFFFF are used for I/O devices and BIOS programs

## BIOS

## references
* https://linux-kernel-labs.github.io/refs/pull/165/merge/labs/interrupts.html
