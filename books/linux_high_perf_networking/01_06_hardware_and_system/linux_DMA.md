# linux DMA

<img width="599" alt="image" src="https://github.com/user-attachments/assets/05bde8ca-01bd-42ab-b466-d27536089f98" />

* terms:
    * `MMU`: translates virtual address to physical address
    * `IOMMU`: translates bus address to physical address

* three types of address:
    * virtual address
    * physical address
    * bus address

## DMA types
* two types of DMA mappings:
    * consistent(coherent) DMA mapping: a write by either the device/the CPU can immediately be read by the processor
        * `dma_alloc_coherent()` works in situation when the kernel program/driver need to allocate a DMA buffer and keep using it until the program exits.
            * it ensures coherency by flushing cache before CPU/DMA controller read from the DMA buffer.
    * streaming DMA mapping
        * `dma_map_simple()` is the streaming DMA API and works in situation when DMA buffer is single use for each call to DMA controller.

![image](https://github.com/user-attachments/assets/632b2915-c8f5-4f22-9961-0ff797e74a52)


![image](https://github.com/user-attachments/assets/1d3517ad-64fc-4efd-99d1-521ca50c46a7)


## references
* https://docs.kernel.org/core-api/dma-api-howto.html
