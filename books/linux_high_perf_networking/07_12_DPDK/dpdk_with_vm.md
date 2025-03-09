#

## install dependencies
```
# apt install qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils virtinst libguestfs-tools ifupdown
# adduser lolv libvirt
```

## setup vm
* download ubuntu image from: https://cloud-images.ubuntu.com/releases/focal/
* download the ubuntu iso image and install:
```
# virt-install \
--debug \
-n dpdk_workspace \
--description "dpdk env" \
--os-type=Linux \
--os-variant=ubuntu20.04 \
--ram=8192 \
--vcpus=4 \
--disk path=/home/lolv/workspace/repo/dpdk/dpdk.qcow2,bus=virtio,size=50,format=qcow2 \
--location=/home/lolv/ubuntu-20.04.6-live-server-amd64.iso,initrd=casper/initrd,kernel=casper/vmlinuz \
--graphics none \
--network bridge:br1,model=e1000e \
--network bridge:br1,model=e1000e \
--network bridge:br1,model=e1000e \
--extra-args 'console=ttyS0,115200n8 serial'
```
* setup the network
```
# ip addr add *.*.*.*/* dev enp3s0
# apt update -y
```

## build dpdk

```
# apt install build-essential python3-pyelftools libnuma-dev meson pkgconf
# git clone https://github.com/DPDK/dpdk.git
# meson -Dexamples=all build
# ninja -C build
# cd build
# meson install
# ldconfig
```

## bind interface
```
# ip link
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: enp1s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 52:54:00:77:7a:1f brd ff:ff:ff:ff:ff:ff
3: enp2s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 52:54:00:49:55:d6 brd ff:ff:ff:ff:ff:ff
4: enp3s0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 52:54:00:e6:f2:8d brd ff:ff:ff:ff:ff:ff
# dpdk-devbind.py -s

Network devices using kernel driver
===================================
0000:01:00.0 '82574L Gigabit Network Connection 10d3' if=enp1s0 drv=e1000e unused=vfio-pci *Active*
0000:02:00.0 '82574L Gigabit Network Connection 10d3' if=enp2s0 drv=e1000e unused=vfio-pci *Active*
0000:03:00.0 '82574L Gigabit Network Connection 10d3' if=enp3s0 drv=e1000e unused=vfio-pci *Active*
# modprobe uio
# modprobe uio_pci_generic
# dpdk-devbind.py -s

Network devices using kernel driver
===================================
0000:01:00.0 '82574L Gigabit Network Connection 10d3' if=enp1s0 drv=e1000e unused=vfio-pci,uio_pci_generic *Active*
0000:02:00.0 '82574L Gigabit Network Connection 10d3' if=enp2s0 drv=e1000e unused=vfio-pci,uio_pci_generic *Active*
0000:03:00.0 '82574L Gigabit Network Connection 10d3' if=enp3s0 drv=e1000e unused=vfio-pci,uio_pci_generic *Active*
# dpdk-devbind.py -b uio_pci_generic 02:00.0
# dpdk-devbind.py -s

Network devices using DPDK-compatible driver
============================================
0000:02:00.0 '82574L Gigabit Network Connection 10d3' drv=uio_pci_generic unused=e1000e,vfio-pci

Network devices using kernel driver
===================================
0000:01:00.0 '82574L Gigabit Network Connection 10d3' if=enp1s0 drv=e1000e unused=vfio-pci,uio_pci_generic *Active*
0000:03:00.0 '82574L Gigabit Network Connection 10d3' if=enp3s0 drv=e1000e unused=vfio-pci,uio_pci_generic *Active*
```

## setup hugepages
```
# mkdir -p /mnt/huge
# mount -t hugetlbfs nodev /mnt/huge
# echo 64 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages
```

## run helloworld
```
# ./dpdk-helloworld -l 0-1 -n 2
EAL: Detected CPU lcores: 4
EAL: Detected NUMA nodes: 1
EAL: Detected static linkage of DPDK
EAL: Multi-process socket /var/run/dpdk/rte/mp_socket
EAL: Selected IOVA mode 'PA'
EAL: VFIO support initialized
hello from core 0
hello from core 1
```
