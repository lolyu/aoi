# mqnic init

## driver init/exit
* `mqnic_init`
* `mqnic_exit`

```c
static const struct pci_device_id mqnic_pci_id_table[] = {
	{PCI_DEVICE(0x1234, 0x1001)},
	{PCI_DEVICE(0x5543, 0x1001)},
	{0 /* end */ }
};

static struct pci_driver mqnic_pci_driver = {
	.name = DRIVER_NAME,
	.id_table = mqnic_pci_id_table,
	.probe = mqnic_pci_probe,                      // called when a device matching an entry in the id_table is found
	.remove = mqnic_pci_remove,                    // called when the device is removed or driver is unloaded
	.shutdown = mqnic_pci_shutdown                 // called during system reboot/shutdown
};

static int __init mqnic_init(void)
{
	int rc;

	rc = pci_register_driver(&mqnic_pci_driver);
	rc = platform_driver_register(&mqnic_platform_driver);

	return rc;
}

module_init(mqnic_init);
module_exit(mqnic_exit);
```
* when `Corundum` driver module is installed/uninstalled, `mqnic_init`/`mqnic_exit` will be called.
* `mqnic_init` register the `Corundum` driver `mqnic_pci_driver`, its `id_table` is the supported device list (of the format `PCI_DEVICE(VENDOR, DEVICE)`).
* when the linux kernel boots up, the PCI core will find the driver that matches the vendor/device for each PCI device;
* when the PCI core identifies your driver as a driver that supports a device on the bus, the probe function will then be called.

## driver probe: `mqnic_pci_probe`

### reserve PCI I/O and memory resources
```c
	// Reserve regions
	ret = pci_request_regions(pdev, DRIVER_NAME);

	mqnic->hw_regs_size = pci_resource_len(pdev, 0);
	mqnic->hw_regs_phys = pci_resource_start(pdev, 0);
	mqnic->app_hw_regs_size = pci_resource_len(pdev, 2);
	mqnic->app_hw_regs_phys = pci_resource_start(pdev, 2);
	mqnic->ram_hw_regs_size = pci_resource_len(pdev, 4);
	mqnic->ram_hw_regs_phys = pci_resource_start(pdev, 4);

	mqnic->hw_addr = pci_ioremap_bar(pdev, 0);						// setup virtual -> physical address mapping for BAR0 region
```

* PCI basics:
	* each PCI device can have multiple PCI functions (PCI functions are essentially the different roles or capabilities that a PCI device can perform. Each PCI device can have multiple functions, and each function can be independently configured and managed.)
	* each PCI function can have up to 6 PCI BARs (BAR0 ~ BAR5)
 	* PCI BAR is a small 32-bit memory location that points to another memory region (BAR region), each BAR tells the CPU the base address + width + other properties.

* how to show PCI devices/functions/BARs?
	* each entry is represented in format: `<bus>:<device>.<function>`
```
# lspci -vv | grep 00:
00:00.0 Host bridge: Intel Corporation Xeon E7 v4/Xeon E5 v4/Xeon E3 v4/Xeon D DMI2 (rev 03)
00:01.0 PCI bridge: Intel Corporation Xeon E7 v4/Xeon E5 v4/Xeon E3 v4/Xeon D PCI Express Root Port 1 (rev 03) (prog-if 00 [Normal decode])
00:01.1 PCI bridge: Intel Corporation Xeon E7 v4/Xeon E5 v4/Xeon E3 v4/Xeon D PCI Express Root Port 1 (rev 03) (prog-if 00 [Normal decode])
...
```
```
00:1f.3 SMBus: Intel Corporation 8 Series/C220 Series Chipset Family SMBus Controller (rev 05)
        Subsystem: Intel Corporation 8 Series/C220 Series Chipset Family SMBus Controller
        Control: I/O+ Mem+ BusMaster- SpecCycle- MemWINV- VGASnoop- ParErr- Stepping- SERR- FastB2B- DisINTx-
        Status: Cap- 66MHz- UDF- FastB2B+ ParErr- DEVSEL=medium >TAbort- <TAbort- <MAbort- >SERR- <PERR- INTx-
        Interrupt: pin C routed to IRQ 18
        IOMMU group: 15
        Region 0: Memory at fbf14000 (64-bit, non-prefetchable) [size=256]
        Region 4: I/O ports at 1020 [size=32]
        Kernel driver in use: i801_smbus
        Kernel modules: i2c_i801
```

### read registers
```c
	mqnic->if_count = ioread32(mqnic->hw_addr + MQNIC_REG_IF_COUNT);				// interface count
	dev_info(dev, "IF count: %d", mqnic->if_count);
	mqnic->if_stride = ioread32(mqnic->hw_addr + MQNIC_REG_IF_STRIDE);				// offset between two interface regions
	dev_info(dev, "IF stride: 0x%08x", mqnic->if_stride);
	mqnic->if_csr_offset = ioread32(mqnic->hw_addr + MQNIC_REG_IF_CSR_OFFSET);
	dev_info(dev, "IF CSR offset: 0x%08x", mqnic->if_csr_offset);
```

### setup IRQ
```c
	for (k = 0; k < MQNIC_MAX_IRQ; k++) {
		ATOMIC_INIT_NOTIFIER_HEAD(&mqnic->irq_nh[k]);
	}

	for (k = 0; k < mqnic->irq_count; k++) {							// set mqnic_interrupt as the interrupt handler
		ret = pci_request_irq(pdev, k, mqnic_interrupt, NULL,
				&mqnic->irq_nh[k], "%s-%d", mqnic->name, k);

		mqnic->irq_map[k] = pci_irq_vector(pdev, k);
	}
```

### init interfaces
```c
	for (k = 0; k < mqnic->if_count; k++) {
		ret = mqnic_init_netdev(mqnic, k, mqnic->hw_addr + k * mqnic->if_stride);
	}
```

#### `mqnic_init_netdev`

##### tx queue allocation
```c
	for (k = 0; k < priv->tx_queue_count; k++) {
		ret = mqnic_create_tx_ring(
			priv,
			&priv->tx_ring[k],
			1024,														// descriptor count
			MQNIC_DESC_SIZE * desc_block_size,							// descriptor stride
			k,
			hw_addr + priv->tx_queue_offset + k * MQNIC_QUEUE_STRIDE	// current queue register base
		); // TODO configure/constant
	}
```
```c
int mqnic_create_tx_ring(struct mqnic_priv *priv, struct mqnic_ring **ring_ptr,
		int size, int stride, int index, u8 __iomem *hw_addr)
{
```

##### register device API
```c
	// entry points
	ndev->netdev_ops = &mqnic_netdev_ops;
	ndev->ethtool_ops = &mqnic_ethtool_ops;
```
```c
static const struct net_device_ops mqnic_netdev_ops = {
	.ndo_open = mqnic_open,
	.ndo_stop = mqnic_close,
	.ndo_start_xmit = mqnic_start_xmit,
	.ndo_get_stats64 = mqnic_get_stats64,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_change_mtu = mqnic_change_mtu,
	.ndo_do_ioctl = mqnic_ioctl,
};
```

## references
* https://stackoverflow.com/questions/30190050/what-is-the-base-address-register-bar-in-pcie
* https://www.kernel.org/doc/html/v4.12/driver-api/pci.html
