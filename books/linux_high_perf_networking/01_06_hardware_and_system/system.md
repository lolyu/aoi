# linux system

## paging

![image](https://github.com/user-attachments/assets/8a8e9eec-4ca9-4739-8f03-936e4b368fe5)

* the calculation:
    * virtual address = VPN (virtual page number) + offset
    * physical address = PFN (page frame number) + offset
    * virtual address: 32 bits
    * page size = 4KB
        * 4KB = 2 ^ 12, needs 12 bits as virtual address offset
    * PTE (page table entry): 4 bytes
        * each page stores 4KB / 4 = 2 ^ 10, needs 10 bits to as the offset to reference PTE offset in one page
    * 10 bits to address the PDE offset, 10 bits to address the PTE offset, 12 bits to address the offset in page
* why use two-level page table?
    *  for a process that needs 16KB memory -> 4 pages
    * two level page table:
        * if the process takes 4 entries in the directory table -> (4 + 1) * 4 = 20KB
    * one level page table:
        * 2 ^ 20 * 4 bytes = 4 MB
    * **the key point is, for multi-level page table, if the high level page table entry is not referenced, it can be invalid and not created to save space.**


## components

![image](https://github.com/user-attachments/assets/087746e3-9ee6-4366-af26-cfe95798094e)


## network ring buffer

![image](https://github.com/user-attachments/assets/29ffe43f-1d31-4342-b431-35ba5b922940)

* NIC driver usually allocates both the ring buffer and the receive buffer.
    * the ring buffer stores the descriptors
    * the receive buffer stores the packets

## interrupt

![image](https://github.com/user-attachments/assets/dc246c2d-4c59-4903-8d6f-cb30d8932159)


### PCIe interrupt
