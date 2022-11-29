# Linux routing
* Every IP capable host knows three classes of destination:
    * locally-hosted addresses
        * Loopback device
    * locally-connected network segment
        * directly reachable IPs, directly connected link-layer medium
    * everywhere else
        * reachable through a `gateway`
* An IP address has two part:
    1. network address
        * `10.17.0.0/16`, while `10.17.0.0` is the network address.
    2. host address
        * `10.17.0.12/16`
* broadcast address: the last allowable address in a given network.
    * for `10.17.0.0/16`, it is `10.17.255.255`

## routing to locally-connected networks
* how to repsent an IP address?
    * Netmask notation(`10.17.0.126/255.255.0.0`)
    * CIDR notation(`10.17.0.126/16`)
* show routing table:
    1. `route -n`
    2. `ip route`
* for locally-connected networks:
    * kernel will find the route entry that defines the locally-connected networks.
    * kernel will use the interface listed in that route entry.
    * kernel will send packets with the source IP address within that network.

## sending through a gateway
* there usually exists a default route entry: `0.0.0.0`
    * if no more specific route exists in a routing table, a default route will be used.
```
# route -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
0.0.0.0         10.0.11.1       0.0.0.0         UG    100    0        0 eth0            # the default gateway
10.0.11.0       0.0.0.0         255.255.255.0   U     0      0        0 eth0
10.250.0.0      0.0.0.0         255.255.255.0   U     0      0        0 br1
168.63.129.16   10.0.11.1       255.255.255.255 UGH   100    0        0 eth0
169.254.169.254 10.0.11.1       255.255.255.255 UGH   100    0        0 eth0
172.17.0.0      0.0.0.0         255.255.0.0     U     0      0        0 docker0
192.168.33.0    0.0.0.0         255.255.255.0   U     0      0        0 vboxnet0
```
## linux as router
* `/net/ipv4/ip_forward`
* `/net/ipv4/config/$DEV/forward`

## route selection

### the common case
* relies on the destionation address
* the kernel will search in
    1. route cache(cached by the history of recently transmitted packet)
    2. the route table
        * look at the destionation address to find a match with `Longest prefix match`
        * route with more specific destination wins

### the whole history
* `policy-based routing`
* `RPDB`: route policy data base
    * store rules
* `policy routing rule` consists of
    * `priority`
    * `selector`
        * if the selector matches the packet, the `action` is performed
    * `action predicate`
* selectors:
    * `source address`
    * `dest address`
    * `ToS` flags(Type of Service)
    * `fwmark`
    * `iif`(input interface)
* steps:
    1. consult the route cache
        * order
            1. `dest`
            2. `source`
            3. `ToS`
            4. `fwmark`
            5. `iif`
    2. consult the `PRDB` rules to find the route table
        * rules are examined sequentially from rule 0 to rule 32767
        * selector order
            1. `source`
            2. `dest`
            3. `ToS`
            4. `fwmark`
            5. `iif`
    3. use `LPF` to find a routing path

```
# ip rule list
0:      from all lookup local
32766:  from all lookup main
32767:  from all lookup default
```

## routing cache
* Since there is a routing cache, rules in routing table might not have effect immediately.
```
# ip route flush cache
```

## routing tables
* routing entry:
    * dest
    * ToS
    * scope
    * oif(output interface)
* kernel at most keeps 256 tables(0~255)
* three pre-employed:
    * 255: loal
        * contains `local` and `broadcast` for locally-bound IP addresses
    * 254: main
        * used by `route` command and default by `ip route`
    * 253: default
```
~ cat /etc/iproute2/rt_tables
#
# reserved values
#
255     local               // hardcoded
254     main                // operated by `route` command and default by `ip route`
253     default
0       unspec
#
# local
#
#1      inr.ruhep
```
```
ip route list table all
default via 10.3.146.1 dev eth0 table default metric 201
10.3.146.0/23 dev eth0 table default scope link
10.1.0.32/30 dev Loopback0 proto kernel scope link src 10.1.0.32
10.3.146.0/23 dev eth0 proto kernel scope link src 10.3.146.241
240.127.1.0/24 dev docker0 proto kernel scope link src 240.127.1.1 linkdown
local 10.1.0.32 dev Loopback0 table local proto kernel scope host src 10.1.0.32                     // Loopback0 IP address
broadcast 10.1.0.32 dev Loopback0 table local proto kernel scope link src 10.1.0.32
broadcast 10.1.0.35 dev Loopback0 table local proto kernel scope link src 10.1.0.32
broadcast 10.3.146.0 dev eth0 table local proto kernel scope link src 10.3.146.241
local 10.3.146.241 dev eth0 table local proto kernel scope host src 10.3.146.241                    // local route, 10.3.146.241 is the mgmt ip address
broadcast 10.3.147.255 dev eth0 table local proto kernel scope link src 10.3.146.241
broadcast 127.0.0.0 dev lo table local proto kernel scope link src 127.0.0.1
local 127.0.0.0/8 dev lo table local proto kernel scope host src 127.0.0.1                          // local route
local 127.0.0.1 dev lo table local proto kernel scope host src 127.0.0.1
broadcast 127.255.255.255 dev lo table local proto kernel scope link src 127.0.0.1
broadcast 240.127.1.0 dev docker0 table local proto kernel scope link src 240.127.1.1 linkdown
local 240.127.1.1 dev docker0 table local proto kernel scope host src 240.127.1.1                   // local route, docker IP
broadcast 240.127.1.255 dev docker0 table local proto kernel scope link src 240.127.1.1 linkdown
```
* route types
    1. `unicast`: default
    2. `broadcast`: link layer devices which support broadcast
    3. `local`
        * when an IP is assigned to an interface, a `local` route is added.
    4. `NAT`
    5. `unreachable`: return an ICMP unreachable
    6. `prohibit`: return an ICMP prohibit
    7. `blackhole`: discard and do nothing
    8. `throw`: cause a route lookup to fail, kernel will keeps lookup in another route table

## routing policy database(RPDB)

## references
* https://blog.scottlowe.org/2013/05/29/a-quick-introduction-to-linux-policy-routing/
* https://man7.org/linux/man-pages/man8/ip-rule.8.html





