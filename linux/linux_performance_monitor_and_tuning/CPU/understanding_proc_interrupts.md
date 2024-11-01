# /proc/interrupts
```
# cat /proc/interrupts
           CPU0       CPU1       CPU2       CPU3
  0:          9          0          0          0   IO-APIC   2-edge      timer
  4:          0          0          0        220   IO-APIC   4-edge      ttyS0
  7:          0          0          0          0   IO-APIC   7-edge      scd
  8:          0          0          0          0   IO-APIC   8-edge      rtc0
  9:          0          0          0          0   IO-APIC   9-fasteoi   acpi
 16:   51747655          0          0          3   IO-APIC  16-fasteoi   scd, linux-kernel-bde
 18:          0          0         45          0   IO-APIC  18-fasteoi   ehci_hcd:usb3, ehci_hcd:usb4
 24:          0          0          0          0  DMAR-MSI   0-edge      dmar0
 26:          0          0          0          0   PCI-MSI 16384-edge      PCIe PME, aerdrv
 27:          0          0          0          0   PCI-MSI 18432-edge      PCIe PME, aerdrv
 28:          0          0          0          0   PCI-MSI 32768-edge      PCIe PME, aerdrv
 29:          0          0          0          0   PCI-MSI 36864-edge      PCIe PME, aerdrv
 30:          0          0          0          0   PCI-MSI 49152-edge      PCIe PME, aerdrv
 31:          0          0          0          0   PCI-MSI 458752-edge      PCIe PME, pciehp
 32:          0          0          0          0   PCI-MSI 466944-edge      PCIe PME, pciehp
 33:          0          0          0          0   PCI-MSI 327680-edge      xhci_hcd
 34:          0     110680          0          0   PCI-MSI 512000-edge      ahci[0000:00:1f.2]
 35:          0          0          0          2   PCI-MSI 1572864-edge      ioat-msix
 36:          2          0          0          0   PCI-MSI 1574912-edge      ioat-msix
 37:          0          2          0          0   PCI-MSI 1576960-edge      ioat-msix
 38:          0          0          2          0   PCI-MSI 1579008-edge      ioat-msix
 39:          0          0          0     192604   PCI-MSI 1048576-edge      eth0-tx-0
 40:          0          0     191707          0   PCI-MSI 1048577-edge      eth0-rx-1
 41:          0          0          0       6438   PCI-MSI 1048578-edge      eth0-rx-2
 42:        500          0          0          0   PCI-MSI 1048579-edge      eth0-rx-3
 43:          0      53069          0          0   PCI-MSI 1048580-edge      eth0-rx-4
NMI:       1690       1530       1683       1351   Non-maskable interrupts
LOC:   35561655   34245052   35172662   36192627   Local timer interrupts
SPU:          0          0          0          0   Spurious interrupts
PMI:       1690       1530       1683       1351   Performance monitoring interrupts
IWI:         28          1          2          3   IRQ work interrupts
RTR:          1          0          0          0   APIC ICR read retries
RES:    5637845    4282075    3831194    6406361   Rescheduling interrupts
CAL:    5395819    5119749    4555211    3790330   Function call interrupts
TLB:    3001688    2883811    2968342    2974692   TLB shootdowns
TRM:          0          0          0          0   Thermal event interrupts
THR:          0          0          0          0   Threshold APIC interrupts
DFR:          0          0          0          0   Deferred Error APIC interrupts
MCE:          0          0          0          0   Machine check exceptions
MCP:        303        304        304        304   Machine check polls
ERR:          0
MIS:          0
PIN:          0          0          0          0   Posted-interrupt notification event
NPI:          0          0          0          0   Nested posted-interrupt event
PIW:          0          0          0          0   Posted-interrupt wakeup event
```
* first column: registered IRQ number
* second column: CPU0 statistics
* the last two give information on the programmable interrupt controller that handles the interrupt and the name of devices that have registered handlers for the interrupt.
