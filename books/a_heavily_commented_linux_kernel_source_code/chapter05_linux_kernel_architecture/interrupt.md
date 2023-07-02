# interrupt

## interrupt basics

* the `interrupt` workflow:
```
device --> interrupt request(IRQ) --> programmable interrupt controller(PIC) --> CPU
```
* the `PIC` issues an interrupt to the processor's `INTR` pin
* the CPU read the interrupt vector from the data bus
* the CPU executes the interrupt service routine by querying the interrupt descriptor table(`IDT`)

### 80x86 PC interrupt system
![image](https://github.com/lolyu/aoi/assets/35479537/babf1679-8d4e-4ebc-891d-8340f2e66fdf)


## Linux kernel interrupt handling
* in Linux, two types of interrupt signals:
    * hardware interrupt
        * maskable interrupt
        * nonmaskable interrupt
    * software interrupt(exceptions)
        * fault
        * trap
        * abort

![image](https://github.com/lolyu/aoi/assets/35479537/3c3d6e05-962a-4a14-8fd9-6f22ab1e357d)

* for a Linux `IDT`
    * `INT0` ~ `INT31`: Intel-defined
        * `INT0` ~ `INT17`: predefined
        * `INT20` ~ `INT31`: Intel reserved
    * `INT32` ~ `INT255`: user-defined
        * `INT32` ~ `INT47`: maps to `IRQ0` ~ `IRQ15` of `8259A` interrupt controller chip
        * `INT128`(0x80): system call

![image](https://github.com/lolyu/aoi/assets/35479537/6b42847a-3e41-4927-95b5-9d48dff98c29)
![image](https://github.com/lolyu/aoi/assets/35479537/ccba010b-4582-413e-b622-81e6b8f190cc)

* hardware interrupt

![image](https://github.com/lolyu/aoi/assets/35479537/e2b6c8f3-4bf8-4478-8382-965573c90e57)
![image](https://github.com/lolyu/aoi/assets/35479537/62615ad8-f4db-4399-a3f8-8f9b3d4aa0a2)

