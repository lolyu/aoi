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
```c
static int mqnic_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	int ret = 0;
	struct mqnic_dev *mqnic;											// the mqnic device
	struct mqnic_priv *priv;
	struct device *dev = &pdev->dev;									// pdev is the struct pci_dev - pci device, pdev->dev is the generic device interface

	...
	mqnic = devm_kzalloc(dev, sizeof(*mqnic), GFP_KERNEL);

	mqnic->dev = dev;
	mqnic->pdev = pdev;
	pci_set_drvdata(pdev, mqnic);

	...
	for (k = 0; k < MQNIC_MAX_IRQ; k++) {
		ATOMIC_INIT_NOTIFIER_HEAD(&mqnic->irq_nh[k]);
	}

	for (k = 0; k < mqnic->irq_count; k++) {							// set mqnic_interrupt as the interrupt handler
		ret = pci_request_irq(pdev, k, mqnic_interrupt, NULL,
				&mqnic->irq_nh[k], "%s-%d", mqnic->name, k);

		mqnic->irq_map[k] = pci_irq_vector(pdev, k);
	}

```
