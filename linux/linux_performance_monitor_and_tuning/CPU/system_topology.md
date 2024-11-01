# CPU topology

## system topology
![image](https://github.com/user-attachments/assets/2250ac7e-2bfd-4303-986f-3e8267efe0e4)


### SMP
* symmetric multi-processor
    * allows all processors to access memory in the same amount of time

### NUMA
* non-uniform memory access
    * processors are physically grouped on a socket.
    * each socket has a dedicated area of memory
    * processors that have local access to that memory are referred collectively as a node


```
$ numactl --hardware
available: 1 nodes (0)
node 0 cpus: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
node 0 size: 64298 MB
node 0 free: 15413 MB
node distances:
node   0
  0:  10
```
```
$ lstopo
Machine (63GB total)
  Package L#0
    NUMANode L#0 (P#0 63GB)
    L3 L#0 (48MB)
      L2 L#0 (1280KB) + L1d L#0 (48KB) + L1i L#0 (32KB) + Core L#0
        PU L#0 (P#0)
        PU L#1 (P#1)
      L2 L#1 (1280KB) + L1d L#1 (48KB) + L1i L#1 (32KB) + Core L#1
        PU L#2 (P#2)
        PU L#3 (P#3)
      L2 L#2 (1280KB) + L1d L#2 (48KB) + L1i L#2 (32KB) + Core L#2
        PU L#4 (P#4)
        PU L#5 (P#5)
      L2 L#3 (1280KB) + L1d L#3 (48KB) + L1i L#3 (32KB) + Core L#3
        PU L#6 (P#6)
        PU L#7 (P#7)
      L2 L#4 (1280KB) + L1d L#4 (48KB) + L1i L#4 (32KB) + Core L#4
        PU L#8 (P#8)
        PU L#9 (P#9)
      L2 L#5 (1280KB) + L1d L#5 (48KB) + L1i L#5 (32KB) + Core L#5
        PU L#10 (P#10)
        PU L#11 (P#11)
      L2 L#6 (1280KB) + L1d L#6 (48KB) + L1i L#6 (32KB) + Core L#6
        PU L#12 (P#12)
        PU L#13 (P#13)
      L2 L#7 (1280KB) + L1d L#7 (48KB) + L1i L#7 (32KB) + Core L#7
        PU L#14 (P#14)
        PU L#15 (P#15)
    Net "eth0"
  HostBridge
    PCI d6f5:00:02.0 (Ethernet)
      Net "enP55029s1"
      OpenFabrics "mlx5_0"
  Block(Disk) "sdb"
  Block(Removable Media Device) "sr0"
  Block(Disk) "sda"
```

![cpu](https://github.com/user-attachments/assets/8baf2cea-e859-4dd7-b6a7-5cbb6ff5ef1d)
