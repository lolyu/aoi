#

## install dependencies
```
# apt install qemu-kvm libvirt-daemon-system libvirt-clients bridge-utils virtinst libguestfs-tools
# adduser lolv libvirt
```

## setup vm
* download ubuntu image from: https://cloud-images.ubuntu.com/releases/focal/
* startup the VM
```
# virt-customize -a ubuntu-20.04-server-cloudimg-amd64-disk-kvm.img --root-password password:root                # setup the root password
# virt-install \
--import \
-n dpdk_workspace \
--description "dpdk env" \
--os-type=Linux \
--os-variant=ubuntu20.04 \
--ram=8192 \
--vcpus=4 \
--disk path=/home/lolv/workspace/repo/dpdk/ubuntu-20.04-server-cloudimg-amd64-disk-kvm.img,bus=virtio \
--graphics none \
--network bridge:br1,model=e1000e \
--network bridge:br1,model=e1000e \
--network bridge:br1,model=e1000e
```
* setup the network
```
# ip addr add *.*.*.*/* dev enp3s0
# apt update -y
```
```
-netdev tap,fd=33,id=hostnet0,vhost=on,vhostfd=34
-device virtio-net-pci,netdev=hostnet0,id=net0,mac=52:54:00:22:ad:c8,bus=pci.2,addr=0x0
-netdev tap,fd=35,id=hostnet1
-device e1000,netdev=hostnet1,id=net1,mac=52:54:00:96:a9:05,bus=pci.3,addr=0x1
-netdev tap,fd=36,id=hostnet2
-device e1000,netdev=hostnet2,id=net2,mac=52:54:00:cd:ad:2f,bus=pci.3,addr=0x2
```
