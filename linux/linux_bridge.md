# introduction to linux bridging
```
# ip link add br0 type bridge
# # ip -j -d -p link show br0           # -j json -d dump -p pretty
[ {
        "ifindex": 2,
        "ifname": "br0",
        "flags": [ "BROADCAST","MULTICAST" ],
        "mtu": 1500,
        "qdisc": "noop",
        "operstate": "DOWN",
        "linkmode": "DEFAULT",
        "group": "default",
        "txqlen": 1000,
        "link_type": "ether",
        "address": "d6:e7:43:e6:b2:7e",
        "broadcast": "ff:ff:ff:ff:ff:ff",
        "promiscuity": 0,
        "min_mtu": 68,
        "max_mtu": 65535,
        "linkinfo": {
            "info_kind": "bridge",
            "info_data": {
                "forward_delay": 1500,
                "hello_time": 200,
                "max_age": 2000,
                "ageing_time": 30000,
                "stp_state": 0,
                "priority": 32768,
                "vlan_filtering": 0,
                "vlan_protocol": "802.1Q",
                "bridge_id": "8000.0:0:0:0:0:0",
                "root_id": "8000.0:0:0:0:0:0",
                "root_port": 0,
                "root_path_cost": 0,
                "topology_change": 0,
                "topology_change_detected": 0,
                "hello_timer": 0.00,
                "tcn_timer": 0.00,
                "topology_change_timer": 0.00,
                "gc_timer": 0.00,
                "vlan_default_pvid": 1,
                "vlan_stats_enabled": 0,
                "vlan_stats_per_port": 0,
                "group_fwd_mask": "0",
                "group_addr": "01:80:c2:00:00:00",
                "mcast_snooping": 1,
                "mcast_router": 1,
                "mcast_query_use_ifaddr": 0,
                "mcast_querier": 0,
                "mcast_hash_elasticity": 16,
                "mcast_hash_max": 4096,
                "mcast_last_member_cnt": 2,
                "mcast_startup_query_cnt": 2,
                "mcast_last_member_intvl": 100,
                "mcast_membership_intvl": 26000,
                "mcast_querier_intvl": 25500,
                "mcast_query_intvl": 12500,
                "mcast_query_response_intvl": 1000,
                "mcast_startup_query_intvl": 3124,
                "mcast_stats_enabled": 0,
                "mcast_igmp_version": 2,
                "mcast_mld_version": 1,
                "nf_call_iptables": 0,
                "nf_call_ip6tables": 0,
                "nf_call_arptables": 0
            }
        },
        "inet6_addr_gen_mode": "eui64",
        "num_tx_queues": 1,
        "num_rx_queues": 1,
        "gso_max_size": 65536,
        "gso_max_segs": 65535
    } ]
```

## sonic vlanmgr

### initialization
```
    // Initialize Linux dot1q bridge and enable vlan filtering
    // The command should be generated as:
    // /bin/bash -c "/sbin/ip link del Bridge 2>/dev/null ;
    //               /sbin/ip link add Bridge up type bridge &&
    //               /sbin/ip link set Bridge mtu {{ mtu_size }} &&
    //               /sbin/ip link set Bridge address {{gMacAddress}} &&
    //               /sbin/bridge vlan del vid 1 dev Bridge self;
    //               /sbin/ip link del dummy 2>/dev/null;
    //               /sbin/ip link add dummy type dummy &&
    //               /sbin/ip link set dummy master Bridge"
    // /sbin/ip link add Bridge up type bridge vlan_filtering 1 
```
* create a virtual bridge called `Bridge`
* delete vlan vid 1, which is the default value
* create a dummy interface called `dummy`, which is enslaved to `Bridge`
* enable vlan filtering on `Bridge`

### add host vlan
```
    // /bin/bash -c "/sbin/bridge vlan add vid {{vlan_id}} dev Bridge self &&
    //               /sbin/ip link add link Bridge up name Vlan{{vlan_id}} address {{gMacAddress}} type vlan id {{vlan_id}}"
```
* create a vlan sub interface `Vlan{{vlan_id}}` on device `Bridge`

### add host vlan member
```
    // /bin/bash -c "/sbin/ip link set {{port_alias}} master Bridge &&
    //               /sbin/bridge vlan del vid 1 dev {{ port_alias }} &&
    //               /sbin/bridge vlan add vid {{vlan_id}} dev {{port_alias}} {{tagging_mode}}"
```
* enslave the port to device `Bridge`
* delete the default vlan vid 1
* if the port is untagged, `tagging mode` will be `pvid untagged`
    * `pvid` means that the vlan is considered a `PVID` at ingress, any untagged frames will be assigned to this vlan
    * `untagged` means that the vlan specified is to be treated as untagged on egress

## references
* https://man7.org/linux/man-pages/man8/bridge.8.html
* https://github.com/sonic-net/sonic-swss/blob/master/cfgmgr/vlanmgr.cpp
* https://man7.org/linux/man-pages/man8/ip-link.8.html
