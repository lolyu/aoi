# linux neighboring system

* what is a neighbor:
    * connected to the same LAN(through a shared medium or a point-to-point link)
    * configured on the same L3 network

* LANs means devices are physically connected
    * devices in the same LAN could talk to each other directly
* L3 subnets are used to divide devices virtually
    * devices in the same L3 subnet may or may not talk to each other directly
        * in the same LAN, yes
        * in different LAN, need router

## why l3 is needed?
* there are many L2 protocols to transmit date between neighbors(L2 protocols are highly related to the physical medium)
* but the L3 layer should not have to worry what medium is being used for transmission

## medium
* two types of medium:
    * shared medium
        * needs collision detection
            * Ethernet uses Carrier Sense Multiple Access with Collision Detection(CSMA/CD) Protocol
        * l2 protocol needs host addressing
            * because any frames could be received by all hosts directly connected to it
            * The addressing scheme usually also defines special addresses that can be used to address a frame to multiple hosts or to all of the hosts: the multicast and broadcast addresses.
    * point-to-point link
        * no collision detection needed
        * don't need addressing

* modern LANs are implemented with Ethernet switches
    * hosts are connected to the switch with point-to-point links
    * no collision needed

## why neighboring system and protocol is needed?
* as L3 address is virtual and dynamic, hosts needs mappings from L3 addresses to L2 addresses to communicate each other within LAN.

### corner cases
* it is not always needed to resolve the L3 address to an L2 address
    * multicast addresses(starting with `01:00:55:XX:XX:XX`)
    * broadcast addresses(`FF:FF:FF:FF:FF:FF`)

#### multicast address formula
![image](https://user-images.githubusercontent.com/35479537/221767051-fb6cc4ac-c1ad-4632-a3c2-c42449c1c6a8.png)


## neighboring protocol

### terminology
* `solicitation request`(aka `neighbor solicitation`)
    * transmission of a packet on the network to ask all of the hosts if any knows the L2 address associated with a given L3 address.
* `solicitation reply`(aka `neighbor advertisement`)
    * reply to a solicitation request

### the big picture
![image](https://user-images.githubusercontent.com/35479537/221770854-55e6e5d7-5fa1-4d2f-88e6-81f6f3199b24.png)

* When a packet is sent:
    1. the routing subsystem of the local host selects the L3 destination address
    2. if according to the routing table, the nexthop is on the same network, the neighboring layer resolvs the L3 address to its L2 address.
    3. after neighbor resolvement, handle the packet to the Traffic Control or Quality of Service(QoS) layer
 
### protocols 
![image](https://user-images.githubusercontent.com/35479537/221771714-2a90206d-2f4c-4663-9afc-093ae3bccb11.png)
* decision tree to send solicitation request
![image](https://user-images.githubusercontent.com/35479537/221773849-ffc877a0-0986-4367-b0e7-7c828da2a11d.png)

* decision tree to process solicitation request
![image](https://user-images.githubusercontent.com/35479537/221773934-7c5a767d-242b-41dc-918f-967c10031457.png)


### neighbor states and network unreachability detection
* what's reachability?
    * bidirectional reachability is needed for the kernel to consider a neighbor reachable

* neighbor creation can be created by:
    * request to transmit a packet to a neighbor
    * reception of a solicitation request from a neighbor

* kernel puts a newly created entry directly into `NUD_STALE` for IPv4, but not IPv6

#### state machine

![image](https://user-images.githubusercontent.com/35479537/221787789-b5c5556d-71ce-4599-af0b-905727b4acec.png)


* `NUD_NONE`
    * the neighbor entry is just created and no state is available yet
* `NUD_INCOMPLETE`
    * solicitation request sent, but no reply
    * in this state, no hardware address to use
* `NUD_REACHABLE`
    * solicitation reply received, neighbor address cached, and `reachability confirmation`
* `NUD_STALE`
    * the neighbour entry is valid but suspicious(not used in a period of time)
* `NUD_DELAY`
    * neighbor entry validation is currently delayed, waiting for a reachability confirmation
* `NUD_PROBE`
    * neighbor is being probed
* `NUD_NOARP`
    * mark neighbors that don't need any protocol to resolve the l2 address(multicast, broadcast)
* `NUD_PERMANENT`
    * statically configured

|derived state|states|description|
|-|-|-|
|`NUD_VALID`|`NUD_PERMANENT`, `NUD_NOARP`, `NUD_REACHABLE`, `NUD_PROBE`, `NUD_STALE`, `NUD_DELAY`|neighbors are believed to have an available l2 address|
|`NUD_CONNECTED`|`NUD_PERMANENT`, `NUD_NOARP`, `NUD_REACHABLE`|subsets of `NUD_VALID` that do not have a `reachability confirmation` pending|
|`NUD_IN_TIMER`|`NUD_INCOMPLETE`, `NUD_DELAY`, `NUD_PROBE`|the neighboring subsystem is running a timer for this entry, means the status is unclear|

#### reachability confirmation
* states `NUD_STALE`, `NUD_DELAY` and `NUD_PROBE` are used to do reachability confirmation
* the introduction of those three states is because there is no need to start a reachability confirmation process until a packet needs to be sent to the associated neighbor
* states details:
    * `NUD_STALE`
        * the cache contains the l2 address of the neighbor, which is not confirmed for a certain period of time
        * the next time a l4 packet is sent to the neighbor, the reachability verification process will be started
    * `NUD_DELAY`
        * `NUD_DELAY` is entered when a l4 packet is sent to the neighbor whose associated entry is in `NUD_STALE`
        * `NUD_DELAY` represents a window of time where external sources could confirm the reachablity of the neighbor
        * this state gives some time tothe upper network layers to provide a reachability confirmation, which may relieve the kernel from sending a solicitation request and thus save both bandwidth and CPU usage.
        * if no confirmation, the entry is moved into `NUD_PROBE`
    * `NUD_PROBE`
        * solicitation process starts, which will resolves the status of the neighbor through explicit solicitation requests

* reachability confirmation could be achieved in two main ways:
    * confirmation from a unicast solicitation's reply
        * **NOTE**: must be an unicast reply, broadcast reply will move the state into `NUD_STALE` instead of `NUD_REACHABLE`
    * confirmation from l4 layer packet sent by the neighbor
        * a user-space application can confirm the reachability of the gateway by using the `MSG_CONFIRM` option with transmission calls such as `send` and `sendmsg`.
##### reachability confirmation by l4 layer packet
![image](https://user-images.githubusercontent.com/35479537/221804359-30b6946c-f03d-48c1-b945-8f0b01d17edd.png)
* why l4 not l3?
    * because tcp is a stream oriented protocol
* can UDP be the reachability confirmation?
    * yes
```
$ ip neighbor | grep 192.168.0.40
192.168.0.40 dev Vlan1000 lladdr 02:a2:6b:18:93:14 STALE
$ echo -n 'helloworld' > /dev/udp/192.168.0.40/4210
$ ip neighbor | grep 192.168.0.40
192.168.0.40 dev Vlan1000 lladdr 02:a2:6b:18:93:14 DELAY
$ ip neighbor | grep 192.168.0.40
192.168.0.40 dev Vlan1000 lladdr 02:a2:6b:18:93:14 DELAY
$ ip neighbor | grep 192.168.0.40
192.168.0.40 dev Vlan1000 lladdr 02:a2:6b:18:93:14 PROBE
$ ip neighbor | grep 192.168.0.40
192.168.0.40 dev Vlan1000  FAILED
```

## references
* http://www.embeddedlinux.org.cn/linux_net/0596002556/understandlni-CHP-26.html
* https://man7.org/linux/man-pages/man7/arp.7.html
