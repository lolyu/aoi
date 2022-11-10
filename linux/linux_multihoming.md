# linux multihoming
In this article, we will briefly discuss the behabvior of multihomed linux device and how to configure linux policy-based routing to make it follow the strong end system model.

## multihoming
A `multihomed` host could be simply regarded as host that has multiple interfaces with IP addresses in the same subnet.

### multihoming requirements
Key requirements to multihoming:

a. A host `MUST/MUST NOT` silently discard an incoming datagram whose destination address does not correspond to the physical interface through which it is received.

b. A host `MUST/MUST NOT` restrict itself to sending (non-source-routed) IP datagrams only through the physical interface that corresponds to the IP source address of the datagrams.

### multihoming implementations
Two multihoming implementations:
* strong end system model
* weak end system model

#### strong end system model
The strong end system model uses `MUST` in the requirements `a` and `b`.

Under the Strong ES model, the route computation for an outgoing datagram is the mapping:
```
route(src IP addr, dest IP addr, TOS) -> gateway
```

The source IP address is included as a parameter in order to select a gateway that is directly reachable on the corresponding physical interface.

#### weak end system model
The weak end system model uses `MUST NOT` in the requirements `a` and `b`

Under the Weak ES model, the route computation for an outgoing datagram is the mapping:
```
route(dest IP addr, TOS) -> gateway, interface
```

## linux as a weak end system
### observations
If a linux device A is multihomed:

**RULE#1: A will have multiple routes to the same subnet destination, all the traffic to the subnet(no matter the source address) are always forwarded based by the very first route.**

**RULE#2: For other devices in the subnet, say B, will have arp entries for those addresses configured on A mapped to the same layer 2 address, which is the physical address of the interface used to forward traffic determined by RULE#1**.

### build a multihomed linux
* start a base linux container `multihome_linux`
```
$ docker run -it --name multihome_linux microdeb/bullseye:latest bash
```
* create four veth links
```
$ sudo ip link add eth0_ext type veth peer name eth0_int
$ sudo ip link add eth1_ext type veth peer name eth1_int
$ sudo ip link add eth2_ext type veth peer name eth2_int
$ sudo ip link add eth3_ext type veth peer name eth3_int                                                                                           
```
* create a bridge on the server and add external veth ports to the bridge
```
$ sudo brctl addbr demo_bridge
$ sudo brctl addif demo_bridge eth0_ext
$ sudo brctl addif demo_bridge eth1_ext
$ sudo brctl addif demo_bridge eth2_ext
$ sudo brctl addif demo_bridge eth3_ext
$ sudo ip link set eth0_ext up
$ sudo ip link set eth1_ext up
$ sudo ip link set eth2_ext up
$ sudo ip link set eth3_ext up
$ sudo brctl show demo_bridge
bridge name     bridge id               STP enabled     interfaces
demo_bridge             8000.1221250b74e6       no      eth0_ext
                                                        eth1_ext
                                                        eth2_ext
                                                        eth3_ext
```
* insert those internal veth ports to the container `multihome_linux`
```
$ docker inspect multihome_linux | grep -w Pid
            "Pid": 26700,
$ sudo ip link set netns 26700 dev eth0_int
$ sudo ip link set netns 26700 dev eth1_int
$ sudo ip link set netns 26700 dev eth2_int
$ sudo ip link set netns 26700 dev eth3_int
$ sudo nsenter -t 26700 -n ip link set eth0_int up
$ sudo nsenter -t 26700 -n ip link set eth1_int up
$ sudo nsenter -t 26700 -n ip link set eth2_int up
$ sudo nsenter -t 26700 -n ip link set eth3_int up
```
* configure the bridge and `multihome_linux` internal interfaces' addresses
```
$ sudo ip addr add 192.168.0.1/21 dev demo_bridge
$ sudo nsenter -t 26700 -n ip addr add 192.168.0.100 dev eth0_int
$ sudo nsenter -t 26700 -n ip addr add 192.168.0.101 dev eth1_int
$ sudo nsenter -t 26700 -n ip addr add 192.168.0.102 dev eth2_int
$ sudo nsenter -t 26700 -n ip addr add 192.168.0.103 dev eth3_int
```
* now, we have a multihomed container:
```
root@aa6d43237d4f:/# ip addr
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
1900: eth0@if1901: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default
    link/ether 02:42:ac:11:00:0a brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 172.17.0.10/16 brd 172.17.255.255 scope global eth0
       valid_lft forever preferred_lft forever
1902: eth0_int@if1903: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether 1a:bb:a8:10:6a:82 brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 192.168.0.100/21 scope global eth0_int
       valid_lft forever preferred_lft forever
1904: eth1_int@if1905: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether 6e:05:47:26:bc:ea brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 192.168.0.101/21 scope global eth1_int
       valid_lft forever preferred_lft forever
1906: eth3_int@if1907: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether 62:e2:68:c9:54:cb brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 192.168.0.103/21 scope global eth3_int
       valid_lft forever preferred_lft forever
1911: eth2_int@if1912: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether 2a:03:5b:e3:57:cf brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 192.168.0.102/21 scope global eth2_int
       valid_lft forever preferred_lft forever
# ip route show table main
default via 172.17.0.1 dev eth0
172.17.0.0/16 dev eth0 proto kernel scope link src 172.17.0.10
192.168.0.0/21 dev eth0_int proto kernel scope link src 192.168.0.100
192.168.0.0/21 dev eth1_int proto kernel scope link src 192.168.0.101
192.168.0.0/21 dev eth2_int proto kernel scope link src 192.168.0.102
192.168.0.0/21 dev eth3_int proto kernel scope link src 192.168.0.103
```

### packet forwarding in the multihomed linux
* start a tcp server on the server
```
$ sudo nc -kl 192.168.0.1 9000
```
* start a client in the multihomed container `multihome_linux`
```
# nc -s 192.168.0.100 192.168.0.1 9000
hello
```
* check the tcp connection inside `multihome_linux`
```
# netstat -ant
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State
tcp        0      0 192.168.0.100:40145     192.168.0.1:9000        ESTABLISHED
```
* dump the packets from `eth0_int` as it is the interface used to talk to `192.168.0.1`, the arp reply is telling `192.168.0.1` that `192.168.0.100` is viable via l2 address of `eth0_int`: `1a:bb:a8:10:6a:82`
```
# tcpdump -i eth0_int -nev
tcpdump: listening on eth0_int, link-type EN10MB (Ethernet), snapshot length 262144 bytes
04:30:21.779245 1a:bb:a8:10:6a:82 > 12:21:25:0b:74:e6, ethertype IPv4 (0x0800), length 74: (tos 0x0, ttl 64, id 18769, offset 0, flags [DF], proto TCP (6), length 60)
    192.168.0.100.40145 > 192.168.0.1.9000: Flags [S], cksum 0x81e4 (incorrect -> 0x9edd), seq 1369364050, win 64240, options [mss 1460,sackOK,TS val 181680567 ecr 0,nop,wscale 13], length 0
04:30:21.779347 12:21:25:0b:74:e6 > ff:ff:ff:ff:ff:ff, ethertype ARP (0x0806), length 42: Ethernet (len 6), IPv4 (len 4), Request who-has 192.168.0.100 tell 192.168.0.1, length 28
04:30:21.779358 1a:bb:a8:10:6a:82 > 12:21:25:0b:74:e6, ethertype ARP (0x0806), length 42: Ethernet (len 6), IPv4 (len 4), Reply 192.168.0.100 is-at 1a:bb:a8:10:6a:82, length 28
04:30:21.779361 12:21:25:0b:74:e6 > 1a:bb:a8:10:6a:82, ethertype IPv4 (0x0800), length 74: (tos 0x0, ttl 64, id 0, offset 0, flags [DF], proto TCP (6), length 60)
    192.168.0.1.9000 > 192.168.0.100.40145: Flags [S.], cksum 0x81e4 (incorrect -> 0xa26f), seq 447571422, ack 1369364051, win 65160, options [mss 1460,sackOK,TS val 560420562 ecr 181680567,nop,wscale 13], length 0
04:30:21.779393 1a:bb:a8:10:6a:82 > 12:21:25:0b:74:e6, ethertype IPv4 (0x0800), length 66: (tos 0x0, ttl 64, id 18770, offset 0, flags [DF], proto TCP (6), length 52)
    192.168.0.100.40145 > 192.168.0.1.9000: Flags [.], cksum 0x81dc (incorrect -> 0xcfc2), ack 1, win 8, options [nop,nop,TS val 181680567 ecr 560420562], length 0
04:30:26.876467 1a:bb:a8:10:6a:82 > 12:21:25:0b:74:e6, ethertype ARP (0x0806), length 42: Ethernet (len 6), IPv4 (len 4), Request who-has 192.168.0.1 tell 192.168.0.100, length 28
04:30:26.876552 12:21:25:0b:74:e6 > 1a:bb:a8:10:6a:82, ethertype ARP (0x0806), length 42: Ethernet (len 6), IPv4 (len 4), Reply 192.168.0.1 is-at 12:21:25:0b:74:e6, length 28
04:30:31.697109 1a:bb:a8:10:6a:82 > 12:21:25:0b:74:e6, ethertype IPv4 (0x0800), length 72: (tos 0x0, ttl 64, id 18771, offset 0, flags [DF], proto TCP (6), length 58)
    192.168.0.100.40145 > 192.168.0.1.9000: Flags [P.], cksum 0x81e2 (incorrect -> 0x651a), seq 1:7, ack 1, win 8, options [nop,nop,TS val 181690485 ecr 560420562], length 6
04:30:31.697191 12:21:25:0b:74:e6 > 1a:bb:a8:10:6a:82, ethertype IPv4 (0x0800), length 66: (tos 0x0, ttl 64, id 37468, offset 0, flags [DF], proto TCP (6), length 52)
    192.168.0.1.9000 > 192.168.0.100.40145: Flags [.], cksum 0x81dc (incorrect -> 0x8240), ack 7, win 8, options [nop,nop,TS val 560430480 ecr 181690485], length 0
```
* start a client inside `multihome_linux` with source address `192.168.0.103`
```
# nc -s 192.168.0.103 192.168.0.1 9000
hello
```
* verify the tcp connection has the source address as `192.168.0.103`
```
# netstat -ant
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State
tcp        0      0 192.168.0.103:37377     192.168.0.1:9000        ESTABLISHED
```
* check that the packets is still forwarding through `eth0_int`, and the arp reply is telling `192.168.0.1` that `192.168.0.103` is viable with l2 address of port `eth0_int`: `1a:bb:a8:10:6a:82`
```
# tcpdump -i eth0_int -nev
tcpdump: listening on eth0_int, link-type EN10MB (Ethernet), snapshot length 262144 bytes
04:35:27.482424 1a:bb:a8:10:6a:82 > 12:21:25:0b:74:e6, ethertype IPv4 (0x0800), length 74: (tos 0x0, ttl 64, id 41141, offset 0, flags [DF], proto TCP (6), length 60)
    192.168.0.103.37377 > 192.168.0.1.9000: Flags [S], cksum 0x81e7 (incorrect -> 0x4dce), seq 1930260842, win 64240, options [mss 1460,sackOK,TS val 4264420786 ecr 0,nop,wscale 13], length 0
04:35:27.482511 12:21:25:0b:74:e6 > ff:ff:ff:ff:ff:ff, ethertype ARP (0x0806), length 42: Ethernet (len 6), IPv4 (len 4), Request who-has 192.168.0.103 tell 192.168.0.1, length 28
04:35:27.482523 1a:bb:a8:10:6a:82 > 12:21:25:0b:74:e6, ethertype ARP (0x0806), length 42: Ethernet (len 6), IPv4 (len 4), Reply 192.168.0.103 is-at 1a:bb:a8:10:6a:82, length 28
04:35:27.482527 12:21:25:0b:74:e6 > 1a:bb:a8:10:6a:82, ethertype IPv4 (0x0800), length 74: (tos 0x0, ttl 64, id 0, offset 0, flags [DF], proto TCP (6), length 60)
    192.168.0.1.9000 > 192.168.0.103.37377: Flags [S.], cksum 0x81e7 (incorrect -> 0x2333), seq 248477312, ack 1930260843, win 65160, options [mss 1460,sackOK,TS val 26583053 ecr 4264420786,nop,wscale 13], length 0
04:35:27.482557 1a:bb:a8:10:6a:82 > 12:21:25:0b:74:e6, ethertype IPv4 (0x0800), length 66: (tos 0x0, ttl 64, id 41142, offset 0, flags [DF], proto TCP (6), length 52)
    192.168.0.103.37377 > 192.168.0.1.9000: Flags [.], cksum 0x81df (incorrect -> 0x5086), ack 1, win 8, options [nop,nop,TS val 4264420786 ecr 26583053], length 0
```
* reorder the routes to subnet `192.168.0.0/21` by down/up `eth0_int`
```
$ sudo nsenter -t 26700 -n ip link set eth0_int down
$ sudo nsenter -t 26700 -n ip link set eth0_int up
```
* the route table reordered
```
# ip route show table main
default via 172.17.0.1 dev eth0
172.17.0.0/16 dev eth0 proto kernel scope link src 172.17.0.10
192.168.0.0/21 dev eth1_int proto kernel scope link src 192.168.0.101
192.168.0.0/21 dev eth2_int proto kernel scope link src 192.168.0.102
192.168.0.0/21 dev eth3_int proto kernel scope link src 192.168.0.103
192.168.0.0/21 dev eth0_int proto kernel scope link src 192.168.0.100
```
* start a client inside `multihome_linux` with source address `192.168.0.103`
```
# nc -s 192.168.0.103 192.168.0.1 9000
```
* verify that the traffic now is forwarded via interface `eth1_int` as it is the output device in the very first route now
```
# tcpdump -i eth1_int -nev
tcpdump: listening on eth1_int, link-type EN10MB (Ethernet), snapshot length 262144 bytes
07:16:22.708703 6e:05:47:26:bc:ea > ff:ff:ff:ff:ff:ff, ethertype ARP (0x0806), length 42: Ethernet (len 6), IPv4 (len 4), Request who-has 192.168.0.1 tell 192.168.0.103, length 28
07:16:22.708753 12:21:25:0b:74:e6 > 6e:05:47:26:bc:ea, ethertype ARP (0x0806), length 42: Ethernet (len 6), IPv4 (len 4), Reply 192.168.0.1 is-at 12:21:25:0b:74:e6, length 28
07:16:22.708758 6e:05:47:26:bc:ea > 12:21:25:0b:74:e6, ethertype IPv4 (0x0800), length 74: (tos 0x0, ttl 64, id 5297, offset 0, flags [DF], proto TCP (6), length 60)
    192.168.0.103.33395 > 192.168.0.1.9000: Flags [S], cksum 0x81e7 (incorrect -> 0x6e74), seq 963462565, win 64240, options [mss 1460,sackOK,TS val 4274076012 ecr 0,nop,wscale 13], length 0
07:16:22.708788 12:21:25:0b:74:e6 > 6e:05:47:26:bc:ea, ethertype IPv4 (0x0800), length 74: (tos 0x0, ttl 64, id 0, offset 0, flags [DF], proto TCP (6), length 60)
    192.168.0.1.9000 > 192.168.0.103.33395: Flags [S.], cksum 0x81e7 (incorrect -> 0xb6e3), seq 3867006841, ack 963462566, win 65160, options [mss 1460,sackOK,TS val 36238279 ecr 4274076012,nop,wscale 13], length 0
07:16:22.708801 6e:05:47:26:bc:ea > 12:21:25:0b:74:e6, ethertype IPv4 (0x0800), length 66: (tos 0x0, ttl 64, id 5298, offset 0, flags [DF], proto TCP (6), length 52)
    192.168.0.103.33395 > 192.168.0.1.9000: Flags [.], cksum 0x81df (incorrect -> 0xe436), ack 1, win 8, options [nop,nop,TS val 4274076012 ecr 36238279], length 0
07:16:27.836450 12:21:25:0b:74:e6 > 6e:05:47:26:bc:ea, ethertype ARP (0x0806), length 42: Ethernet (len 6), IPv4 (len 4), Request who-has 192.168.0.103 tell 192.168.0.1, length 28
07:16:27.836487 6e:05:47:26:bc:ea > 12:21:25:0b:74:e6, ethertype ARP (0x0806), length 42: Ethernet (len 6), IPv4 (len 4), Reply 192.168.0.103 is-at 6e:05:47:26:bc:ea, length 28
# ip neighbor
192.168.0.1 dev eth1_int lladdr 12:21:25:0b:74:e6 STALE
172.17.0.1 dev eth0 lladdr 02:42:26:a8:ac:8d STALE
```


## references
* https://www.rfc-editor.org/rfc/rfc1122
