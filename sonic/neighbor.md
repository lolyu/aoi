# SONiC neighbor system

## basic

## neighsyncd

* key functionality:
    * update the `APPL_DB` `NEIGH_TBLE` based on the kernel neighbor entries

```cpp
    NeighSync sync(&pipelineAppDB, &stateDb, &cfgDb);

    NetDispatcher::getInstance().registerMessageHandler(RTM_NEWNEIGH, &sync);
    NetDispatcher::getInstance().registerMessageHandler(RTM_DELNEIGH, &sync);

    NetLink netlink;
    Select s;

    netlink.registerGroup(RTNLGRP_NEIGH);
    cout << "Listens to neigh messages..." << endl;
    netlink.dumpRequest(RTM_GETNEIGH);

    s.addSelectable(&netlink);
    while (true)
    {
        Selectable *temps;
        s.select(&temps);
    }
```

### how does netlink work?

1. when `NetLink` is created, it will register the netlink socket callback as `netlink::onNetlinkMsg`
    * **NOTE**: the netlink socket is connected to `NETLINK_ROUTE`, the callback will be called for the following events:
        * Interface state changes (up/down, link status)
        * IP address additions/deletions
        * Routing table updates (route add/delete/modify)
        * Neighbor table changes (ARP/ND entries)
        * Network device creation/removal
        * VLAN configuration changes
        * Bridge/bonding interface updates

```cpp
NetLink::NetLink(int pri) :
    Selectable(pri), m_socket(NULL)
{
    m_socket = nl_socket_alloc();

    nl_socket_disable_seq_check(m_socket);
    nl_socket_modify_cb(m_socket, NL_CB_VALID, NL_CB_CUSTOM, onNetlinkMsg, this);

    int err = nl_connect(m_socket, NETLINK_ROUTE);

    nl_socket_set_nonblocking(m_socket);
    nl_socket_set_buffer_size(m_socket, 16777216, 0);
}```

2. `Netlink::registerGroup` is used to limit the netlink message scope

3. when `select` detects the netlink fd is readable, it will poll data from the socket by calling the overriden `NetLink::readData`

```cpp
uint64_t NetLink::readData()
{
    int err;

    do
    {
        err = nl_recvmsgs_default(m_socket);
    }
    while(err == -NLE_INTR); // Retry if the process was interrupted by a signal

    return 0;
}
```

4. `Netlink::readData` will keep reading available messages from the socket, validating each message, and calling the callback function.


4. the registered netlink callback `netlink::onNetlinkMsg` will in turn dispatch the netlink message based on its type:

```cpp
int NetLink::onNetlinkMsg(struct nl_msg *msg, void *arg)
{
    NetDispatcher::getInstance().onNetlinkMessage(msg);
    return NL_OK;
}
```

5. the singleton class `NetDispatcher` will call the registered netlink message handler based on the message type
    * `NetMsg` is the handler class, and each subclass must override `NetMsg::onMsg` to implement its own message handling functionality
    * `NeighSync::onMsg` implements how SONiC handles neighbor events

```cpp
struct rtnl_neigh
{
	NLHDR_COMMON
	uint32_t	n_family;
	uint32_t	n_ifindex;
	uint16_t	n_state;
	uint8_t		n_flags;
	uint8_t		n_type;
	struct nl_addr *n_lladdr;
	struct nl_addr *n_dst;
	uint32_t	n_probes;
	struct rtnl_ncacheinfo n_cacheinfo;
	uint32_t                n_state_mask;
	uint32_t                n_flag_mask;
	uint32_t		n_master;
	uint16_t	n_vlan;
};
```


## neighorch

* `NeighOrch` subscribes to neighbor events from `APPL_DB` `NEIGH_TABLE` and observes `FdbOrch` for fdb changes.

```cpp
    gNeighOrch = new NeighOrch(m_applDb, APP_NEIGH_TABLE_NAME, gIntfsOrch, gFdbOrch, gPortsOrch, m_chassisAppDb);
```

* `NeighOrch::doTask`:
    * neighbor add -> `addNeighbor`
    * neighbor del -> `removeNeighbor`

### `addNeighbor`
* `addNeighbor`:
    * program neighbor to ASIC
    * program nexthop to ASIC

### `removeNeighbor`
