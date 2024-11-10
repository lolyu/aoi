# Linux kernel receive path


## init igb driver
```c
static struct pci_driver igb_driver = {
	.name     = igb_driver_name,
	.id_table = igb_pci_tbl,
	.probe    = igb_probe,
	.remove   = igb_remove,
	.shutdown = igb_shutdown,
	.sriov_configure = igb_pci_sriov_configure,
	.err_handler = &igb_err_handler
};

static int __init igb_init_module(void)
{
	int ret;
	ret = pci_register_driver(&igb_driver);
	return ret;
}

module_init(igb_init_module);
```
* `igb_probe` is called to initialize the `igb` driver:
    * init DMA
    * hook `ethtool` implementations
    * hool device operation implementations
    * init `NAPI`
```c
// drivers/net/ethernet/intel/igb/igb_ethtool.c
static const struct ethtool_ops igb_ethtool_ops = {
	.get_settings		= igb_get_settings,
	.set_settings		= igb_set_settings,
	.get_drvinfo		= igb_get_drvinfo,
	.get_regs_len		= igb_get_regs_len,
  ...
};
```
```c
// drivers/net/ethernet/intel/igb/igb_main.c
static const struct net_device_ops igb_netdev_ops = {
	.ndo_open		= igb_open,
	.ndo_stop		= igb_close,
	.ndo_start_xmit		= igb_xmit_frame,
	.ndo_get_stats64	= igb_get_stats64,
	.ndo_set_rx_mode	= igb_set_rx_mode,
	.ndo_set_mac_address	= igb_set_mac,
	.ndo_change_mtu		= igb_change_mtu,
	.ndo_do_ioctl		= igb_ioctl,
	.ndo_tx_timeout		= igb_tx_timeout,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_vlan_rx_add_vid	= igb_vlan_rx_add_vid,
	.ndo_vlan_rx_kill_vid	= igb_vlan_rx_kill_vid,
	.ndo_set_vf_mac		= igb_ndo_set_vf_mac,
	.ndo_set_vf_vlan	= igb_ndo_set_vf_vlan,
	.ndo_set_vf_tx_rate	= igb_ndo_set_vf_bw,
	.ndo_set_vf_spoofchk	= igb_ndo_set_vf_spoofchk,
	.ndo_get_vf_config	= igb_ndo_get_vf_config,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= igb_netpoll,
#endif
	.ndo_fix_features	= igb_fix_features,
	.ndo_set_features	= igb_set_features,
};

static int igb_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct net_device *netdev;
	struct igb_adapter *adapter;

  ...

	err = -ENOMEM;
	netdev = alloc_etherdev_mq(sizeof(struct igb_adapter),
				   IGB_MAX_TX_QUEUES);

	netdev->netdev_ops = &igb_netdev_ops;              // set the device operation implementations
	igb_set_ethtool_ops(netdev);                       // set the device nettool implementations
```

## references
* 
