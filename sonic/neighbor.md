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

```cpp
    nl_socket_modify_cb(m_socket, NL_CB_VALID, NL_CB_CUSTOM, onNetlinkMsg, this);
```

2. `Netlink::registerGroup` is used to limit the netlink message scope

3. when `select` detects the netlink fd is readable, it will poll data from the socket by calling the overriden `Selectable::readData`

4. the registered netlink callback `netlink::onNetlinkMsg` will in turn dispatch the netlink message based on its type:

```cpp
int NetLink::onNetlinkMsg(struct nl_msg *msg, void *arg)
{
    NetDispatcher::getInstance().onNetlinkMessage(msg);
    return NL_OK;
}
```

5. `NetDispatcher` will call the registered netlink message handler based on the message type
    * `NetMsg` is the handler class, and each subclass must override `NetMsg::onMsg` to implement its own message handling functionality



## neighorch

* `NeighOrch` subscribes to neighbor events from `APPL_DB` `NEIGH_TABLE` and observes `FdbOrch` for fdb changes.

```cpp
    gNeighOrch = new NeighOrch(m_applDb, APP_NEIGH_TABLE_NAME, gIntfsOrch, gFdbOrch, gPortsOrch, m_chassisAppDb);
```

