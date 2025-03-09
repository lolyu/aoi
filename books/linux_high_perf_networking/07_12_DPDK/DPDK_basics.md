# DPDK basics
* DPDK bypasses kernel and process packet in the user space.
* key terms:
    * EAL: environment abstraction layer
    * PMD: poll mode driver

## DPDK vs kernel
![image](https://github.com/user-attachments/assets/9af7e33f-965b-407c-8ef7-931af5aea3c0)



## core components

![image](https://github.com/user-attachments/assets/b7674caa-504a-4f76-b9db-208e19ef17ef)

* EAL hides the environmental specifics from the applications/libraries and provide a generic interface.
* EAL provides services:
   * DPDK loading and launching
   * Support for multi-process and multi-thread execution types
   * Core affinity/assignment procedures
   * System memory allocation/de-allocation
   * Atomic/lock operations
   * Time reference
   * PCI bus access
   * Trace and debug functions
   * CPU feature identification
   * Interrupt handling
   * Alarm operations
   * Memory management (malloc)

## poll mode driver

### models
* PMD has two models:
    * run-to-completion
        * synchronous
        * In the run-to-completion model, a specific port’s RX descriptor ring is polled for packets through an API. Packets are then processed on the same core and placed on a port’s TX descriptor ring through an API for transmission.
    * pipeline
        * asynchronous
        * In the pipe-line model, one core polls one or more port’s RX descriptor ring through an API. Packets are received and passed to another core via a ring. The other core continues to process the packet which then may be placed on a port’s TX descriptor ring through an API for transmission.
