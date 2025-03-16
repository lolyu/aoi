# skeleton notes

## topology
<img width="449" alt="image" src="https://github.com/user-attachments/assets/8a71fb83-85ee-41b2-86b6-72bec8c8d518" />


## lab setup


* binds the ports to dpdk
```
lolv@dpdk:~$ sudo dpdk-devbind.py -s

Network devices using kernel driver
===================================
0000:02:00.0 '82574L Gigabit Network Connection 10d3' if=enp2s0 drv=e1000e unused=vfio-pci,uio_pci_generic *Active*
0000:03:00.0 '82574L Gigabit Network Connection 10d3' if=enp3s0 drv=e1000e unused=vfio-pci,uio_pci_generic
0000:04:00.0 '82574L Gigabit Network Connection 10d3' if=enp4s0 drv=e1000e unused=vfio-pci,uio_pci_generic
lolv@dpdk:~$ sudo dpdk-devbind.py -b uio_pci_generic 03:00.0
lolv@dpdk:~$ sudo dpdk-devbind.py -b uio_pci_generic 04:00.0
lolv@dpdk:~$ sudo dpdk-devbind.py -s

Network devices using DPDK-compatible driver
============================================
0000:03:00.0 '82574L Gigabit Network Connection 10d3' drv=uio_pci_generic unused=e1000e,vfio-pci
0000:04:00.0 '82574L Gigabit Network Connection 10d3' drv=uio_pci_generic unused=e1000e,vfio-pci

Network devices using kernel driver
===================================
0000:02:00.0 '82574L Gigabit Network Connection 10d3' if=enp2s0 drv=e1000e unused=vfio-pci,uio_pci_generic *Active*

```

## lab run
* run the skeleton inside the VM
```
lolv@dpdk:~/dpdk/build/examples$ sudo ./dpdk-skeleton -l 1 -n 4
EAL: Detected CPU lcores: 4
EAL: Detected NUMA nodes: 1
EAL: Detected static linkage of DPDK
EAL: Multi-process socket /var/run/dpdk/rte/mp_socket
EAL: Selected IOVA mode 'PA'
EAL: 2 hugepages of size 1073741824 reserved, but no mounted hugetlbfs found for that size
EAL: VFIO support initialized
Port 0 MAC: 52 54 00 57 46 25
Port 1 MAC: 52 54 00 f5 c2 fc

Core 1 forwarding packets. [Ctrl+C to quit]
^C
```
* send 1000 packets on host `vnet0` and capture on `vnet1`:
```
$ sudo python3
Python 3.8.10 (default, Mar 11 2025, 17:45:31)
[GCC 9.4.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> from scapy.all import *
>>> from ptf import testutils
Using packet manipulation module: ptf.packet_scapy
>>> pkt = testutils.simple_ip_packet()
>>> sendp(pkt, iface="vnet1", count=1000)
........................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................
Sent 1000 packets.
```
```
$ sudo tcpdump -i vnet2 -nev -w demp.pcap
tcpdump: listening on vnet2, link-type EN10MB (Ethernet), capture size 262144 bytes
^C1000 packets captured
1000 packets received by filter
0 packets dropped by kernel
```
