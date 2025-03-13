# dpdk introduction

## MPPS

```
MPPS = bitRate / 8 * (IPG + preamble + SFD + packet size)
```
* terms:
    * MPPS: million packet per sec
    * IPG: inter packet gap
    * SFD: start frame delimiter

![image](https://github.com/user-attachments/assets/5fd1b194-1d5a-4df9-bed9-ebfebd1b7b97)


* MPPS with 10Gbit/s throughput:

|packet size|MPPS|arrival (ns)|
|-|-|-|
|64|10000/(8*84)=14.88|67.20|
|128|8.45|118.40|
|256|4.53|220.80|
|512|2.35|425.69|

* ideally, if the switch could retransmit the 64B packets within 67.20ns, there will be no switching latency.

## helloworld

```
# ./dpdk-helloworld -l 0-3 -n 4
EAL: Detected CPU lcores: 4
EAL: Detected NUMA nodes: 1
EAL: Detected static linkage of DPDK
EAL: Multi-process socket /var/run/dpdk/rte/mp_socket
EAL: Selected IOVA mode 'PA'
EAL: VFIO support initialized
hello from core 1
hello from core 2
hello from core 3
hello from core 0
```


## references

