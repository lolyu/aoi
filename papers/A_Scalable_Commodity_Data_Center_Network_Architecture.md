# A scalable commodity data center network architecture

## oversubscription
* oversubscription = how many hosts are sharing the uplink
* 1:1 subscription = uplink bandwidth / downlink bandwidth

| Ratio | Meaning                                    |
| ----- | ------------------------------------------ |
| 1:1   | Full bisection bandwidth — Fat-Tree ideal  |
| 2:1   | 2 hosts compete for 1 host-worth of uplink |
| 4:1   | Common in older enterprise DCs             |

* dualtor oversubscription is 6:1 ~ 3:1

## FAT tree

* K-ary fat tree:
    * pods: contains two layer switches:
        * edge (k/2)
        * aggregation (k/2)

* Parameter: k = number of ports on each switch
Structure

| Layer                          | Count  | Connections                                        |
| ------------------------------ | ------ | -------------------------------------------------- |
| Pods                           | k      | —                                                  |
| Edge switches (per pod)        | k/2    | Each connects to k/2 hosts + k/2 aggregation ports |
| Aggregation switches (per pod) | k/2    | Each connects to k/2 edge + k/2 core switches      |
| Core switches (total)          | (k/2)² | Each has 1 port into each of the k pods            |

* **Within a pod, the aggregation switches each connect to a disjoint group of core switches**

<img width="680" height="304" alt="image" src="https://github.com/user-attachments/assets/27e0a1c0-be10-4584-8d65-e17dd62fef0a" />

## references
* http://ccr.sigcomm.org/online/files/p63-alfares.pdf
