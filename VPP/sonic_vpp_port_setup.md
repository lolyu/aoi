# SONiC-VPP Port Setup

## Overview

When the SONiC-VPP VM boots, network ports must be transferred from the
Linux kernel (`virtio_net`) to DPDK (`uio_pci_generic`) so that VPP can
perform userspace packet I/O. After VPP starts, the `linux-cp` plugin
creates TAP interfaces so the Linux control plane can still reach each port.



---

## Stage 1: Port Discovery (`10-01-vpp-cfg-init`)

Runs on first boot as part of `config-setup.service`.

- Iterates `eth1`…`eth32` (skips `eth0` — the management NIC)
- For each port, gets the PCI address via `ethtool -i ethN | grep bus-info`
- Writes `/etc/sonic/vpp/syncd_vpp_env`:

```
DPDK_DISABLE=n
VPP_DPDK_PORTS=0000:04:00.0,0000:05:00.0,...,0000:23:00.0
SONIC_NUM_PORTS=32
NO_LINUX_NL=y
```

---

## Stage 2: Driver Rebind (`vpp_ports_setup.sh start`)
* Binds the ports to UIO(userspace I/O):
  * `ethX` -> `/dev/uioX`
* **NOTE**: vpp_ports_setup.sh does exactly the same thing as dpdk-devbind.py — it's essentially a reimplementation of the same sysfs operations in bash.

Runs via `vpp-port-config.service` — **after** `config-setup.service`,
**before** `syncd.service`.

For each PCI address in `VPP_DPDK_PORTS`:

```
1. modprobe uio_pci_generic

2. echo "0000:04:00.0" > /sys/bus/pci/drivers/virtio_net/unbind
   → eth1 disappears from Linux

3. NEWID=$(lspci -s 0000:04:00.0 -n | awk ...)
   → "1af4 1041"   (vendor:device of Red Hat virtio-net-pci)
   echo "1af4 1041" > /sys/bus/pci/drivers/uio_pci_generic/new_id

4. echo "0000:04:00.0" > /sys/bus/pci/drivers/uio_pci_generic/bind
   → /dev/uio0 created; DPDK can mmap the NIC registers
```

Special cases:
- `mlx5_core` (Mellanox) NICs are skipped — they use a different DPDK model
- veth interfaces have no PCI address — used as-is via `create host-interface`
- Set `UIO_DRV=vfio-pci` env var to use VFIO instead (requires IOMMU)

---

## Stage 3: VPP Starts (`vpp_init.sh`)

Generates startup config from `/etc/vpp/startup.conf.tmpl`:

- Adds each PCI address to the `dpdk {}` block, named `bobm0`…`bobm31`
- Computes `buffers-per-numa = 2048 × SONIC_NUM_PORTS`
- Allocates hugepages: `max(12, ports×2)` × 2MB pages
- Starts `/usr/bin/vpp -c <generated.conf>`

DPDK's virtio PMD then polls TX/RX virtqueues directly in userspace —
the kernel is not involved in data plane packet I/O.

---

## Stage 4: TAP Interface Creation (saivpp)

After `config_db.json` loads, `orchagent` issues `SAI_OBJECT_TYPE_HOSTIF`
create calls for each port. `SwitchStateBaseHostif.cpp` handles each one:

```
SAI_OBJECT_TYPE_HOSTIF create (name="Ethernet0")
  → vpp_create_tap_device("Ethernet0")         # opens /dev/net/tun
  → tap_to_hwif_name("Ethernet0") → "bobm0"   # via sonic_vpp_ifmap.ini
  → configure_lcp_interface("bobm0", "Ethernet0")
      → LCP_ITF_PAIR_ADD_DEL Binary API → VPP
          → linux-cp plugin pairs bobm0 ↔ Ethernet0 (TAP)
```

---

## Final Interface Stack

```
Hardware (virtio-net-pci)
  0000:04:00.0          ← PCI address (BDF: Bus:Device.Function)
  driver: uio_pci_generic
  /dev/uio0             ← userspace mmap target

VPP data plane
  bobm0                 ← DPDK interface name (from startup.conf)

Linux control plane
  Ethernet0             ← TAP interface (driver: tun, bus-info: tap)
  Managed by FRR, teamd, SONiC daemons

Management (untouched)
  eth0                  ← virtio_net, 0000:03:00.0, SSH/mgmt
```

---

## Packet Paths

| Type | Path |
|------|------|
| Transit (forwarded) | `bobm0` → VPP FIB → `bobmN` (never touches Linux) |
| Control (BGP, ARP, ICMP to self) | `bobm0` → VPP punt → `Ethernet0` TAP → Linux |
| Tx from Linux (FRR route reply) | `Ethernet0` TAP → VPP XC inject → `bobm0` |

---

## Key Files

| File | Purpose |
|------|---------|
| `sonic-platform-modules-vpp/files/etc/config-setup/factory-default-hooks.d/10-01-vpp-cfg-init` | Port discovery, writes `syncd_vpp_env` |
| `sonic-platform-modules-vpp/files/usr/bin/vpp_ports_setup.sh` | Driver rebind (virtio_net → uio_pci_generic) |
| `sonic-platform-modules-vpp/files/service/vpp-port-config.service` | systemd unit ordering the rebind |
| `docker-sonic-vpp/scripts/vpp_init.sh` | Generates VPP startup.conf and starts VPP |
| `saivpp/src/SwitchStateBaseHostif.cpp` | Creates TAP + registers lcp pair |
| `saivpp/src/vppxlate/SaiVppXlate.c` | `configure_lcp_interface()` → Binary API |
| `device/<platform>/<hwsku>/sonic_vpp_ifmap.ini` | Maps `EthernetX` ↔ `bobmN` |


## references
* https://www.ibm.com/docs/en/linux-on-systems?topic=hs-pci-2
* https://www.kernel.org/doc/html/v5.2/driver-api/uio-howto.html