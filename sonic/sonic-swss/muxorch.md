# muxorch
## orchagent components
* three components:
    * `MuxOrch`
    * `MuxCableOrch`
    * `MuxStateOrch`

```cpp
    vector<string> mux_tables = {
        CFG_MUX_CABLE_TABLE_NAME,
        CFG_PEER_SWITCH_TABLE_NAME
    };
    MuxOrch *mux_orch = new MuxOrch(m_configDb, mux_tables, tunnel_decap_orch, gNeighOrch, gFdbOrch);
    gDirectory.set(mux_orch);

    MuxCableOrch *mux_cb_orch = new MuxCableOrch(m_applDb, m_stateDb, APP_MUX_CABLE_TABLE_NAME);
    gDirectory.set(mux_cb_orch);

    MuxStateOrch *mux_st_orch = new MuxStateOrch(m_stateDb, STATE_HW_MUX_CABLE_TABLE_NAME);
    gDirectory.set(mux_st_orch);
```

## class MuxOrch
`MuxOrch` manages the lifecyle of `MuxCable` objects.


class `MuxOrch` subscribes to two tables:
* `CFG_MUX_CABLE_TABLE_NAME`
    * used to initialize `MuxCable` objects
* `CFG_PEER_SWITCH_TABLE_NAME`
    * used to initialize the tunnel to peer switch

class `MuxOrch` receives updates from `FdbOrch` and `NeighOrch`:
* `updateNeighbor`
* `updateFdb`

### MuxOrch::addOperation
* calls the handlers for subscribed table
    * `handlePeerSwitch`: `CFG_PEER_SWITCH_TABLE_NAME`
    * `handleMuxCfg`: `CFG_MUX_CABLE_TABLE_NAME`


## class MuxCableOrch
`MuxCableOrch` subscribes to `APP_MUX_CABLE_TABLE_NAME` for any mux state set from `linkmgrd` and call the state transition functions defined in `MuxCable` obejct.

### MuxCableOrch::addOperation
```cpp
bool MuxCableOrch::addOperation(const Request& request)
{
    auto port_name = request.getKeyString(0);

    MuxOrch* mux_orch = gDirectory.get<MuxOrch*>();
    if (!mux_orch->isMuxExists(port_name))
    {
        return false;
    }

    auto state = request.getAttrString("state");
    auto mux_obj = mux_orch->getMuxCable(port_name);

    try
    {
        mux_obj->setState(state);
    }
    catch(const std::runtime_error& error)
    {
        return true;
    }

    return true;
}
```

## class MuxStateOrch
`MuxStateOrch` subscribes to `STATE_HW_MUX_CABLE_TABLE_NAME` for any mux state set response from `ycabled` and updates `STATE_MUX_CABLE_TABLE_NAME` to notify `linkmgrd` about the mux state set response.


### MuxStateOrch::addOperation
```cpp
bool MuxStateOrch::addOperation(const Request& request)
{

    auto port_name = request.getKeyString(0);

    MuxOrch* mux_orch = gDirectory.get<MuxOrch*>();
    if (!mux_orch->isMuxExists(port_name))
    {
        SWSS_LOG_WARN("Mux entry for port '%s' doesn't exist", port_name.c_str());
        return false;
    }

    auto hw_state = request.getAttrString("state");
    auto mux_obj = mux_orch->getMuxCable(port_name);
    string mux_state;

    try
    {
        mux_state = mux_obj->getState();
    }
    catch(const std::runtime_error& error)
    {
        return false;
    }

    if (mux_obj->isStateChangeInProgress())
    {
        SWSS_LOG_INFO("Mux state change for port '%s' is in-progress", port_name.c_str());
        return false;
    }

    if (mux_state != hw_state)
    {
        if (mux_obj->isStateChangeFailed())
        {
            mux_state = MUX_HW_STATE_ERROR;
        }
        else
        {
            mux_state = MUX_HW_STATE_UNKNOWN;
        }
    }

    updateMuxState(port_name, mux_state);

    return true;
}
```




## class MuxCable
Represents a mux cable object with the methods to to state transitions.

### MuxCable::setState
* `MuxCable::setState` does two things:
    * call transition function
    * update `APP_HW_MUX_CABLE_TABLE_NAME`

```cpp
void MuxCable::setState(string new_state)
{
    MuxState ns = muxStateStringToVal.at(new_state);
    new_state = muxStateValToString.at(ns);

    auto it = muxStateTransition.find(make_pair(state_, ns));

    if (it ==  muxStateTransition.end())
    {
        // Update HW Mux cable state anyways
        mux_cb_orch_->updateMuxState(mux_name_, new_state);
        return;
    }

    MuxState state = state_;
    state_ = ns;

    st_chg_in_progress_ = true;

    if (!(this->*(state_machine_handlers_[it->second]))())
    {
        //Reset back to original state
        state_ = state;
        st_chg_in_progress_ = false;
        st_chg_failed_ = true;
        throw std::runtime_error("Failed to handle state transition");
    }


    st_chg_in_progress_ = false;
    st_chg_failed_ = false;

    return;
}
```

### transition functions
* four transition functions
    * Init to Active: `stateInitActive`
    * to Active: `stateActive`
    * to Standby: `stateStandby`
    * Init to Standby: `stateStandby`

#### MuxCable::nbrHandler
```cpp
bool MuxCable::nbrHandler(bool enable, bool update_rt)
{
    if (enable)
    {
        return nbr_handler_->enable(update_rt);
    }
    else
    {
        sai_object_id_t tnh = mux_orch_->createNextHopTunnel(MUX_TUNNEL, peer_ip4_);
        if (tnh == SAI_NULL_OBJECT_ID)
        {
            SWSS_LOG_INFO("Null NH object id, retry for %s", peer_ip4_.to_string().c_str());
            return false;
        }

        return nbr_handler_->disable(tnh);
    }
}
```

#### MuxCable::stateInitActive
* `nbrhandler(true, false)`
#### MuxCable::stateActive
* `nbrhandler(true)`
* `aclhandler(..., false)`
#### MuxCable::stateStandby
* `nbrhandler(false)`
* `aclhandler(..., true)`



## fdb handling
```cpp
void MuxOrch::updateFdb(const FdbUpdate& update)
{
    if (!update.add)
    {
        /*
         * For Mac aging, flush events, skip updating mux neighbors.
         * Instead, wait for neighbor update events
         */
        return;
    }

    NeighborEntry neigh;
    MacAddress mac;
    MuxCable* ptr;
    for (auto nh = mux_nexthop_tb_.begin(); nh != mux_nexthop_tb_.end(); ++nh)
    {
        // get the neighbor and mac from neigh_orch
        auto res = neigh_orch_->getNeighborEntry(nh->first, neigh, mac);

        // only process nexthop that has the same mac address as the fdb update
        if (!res || update.entry.mac != mac)
        {
            continue;
        }

        // for nexthops stored that has same mac address but different port name as the fdb update
        // 1. remove the nexthop from the old mux port
        // 2. add the nexthop to the new mux port based on the fdb update port
        if (nh->second != update.entry.port_name)
        {
            // if current nexthop port is not empty, remove it from current mux port
            if (!nh->second.empty() && isMuxExists(nh->second))
            {
                ptr = getMuxCable(nh->second);
                if (ptr->isIpInSubnet(nh->first.ip_address))
                {
                    continue;
                }
                nh->second = update.entry.port_name;
                ptr->updateNeighbor(nh->first, false);
            }
            // add the nexthop to the mux port as the fdb update
            if (isMuxExists(update.entry.port_name))
            {
                ptr = getMuxCable(update.entry.port_name);
                ptr->updateNeighbor(nh->first, true);
            }
        }
    }
}
```
## neighbor handling

### neighbor update struct
```cpp
typedef NextHopKey NeighborEntry;

struct NextHopKey
{
    IpAddress           ip_address;     // neighbor IP address
    string              alias;          // incoming interface alias
    uint32_t            vni;            // Encap VNI overlay nexthop
    MacAddress          mac_address;    // Overlay Nexthop MAC.
    LabelStack          label_stack;    // MPLS label stack
    uint32_t            weight;         // NH weight for NHGs
    string              srv6_segment;   // SRV6 segment string
    string              srv6_source;    // SRV6 source address
    ...
};

struct NeighborUpdate
{
    NeighborEntry entry;
    MacAddress mac;
    bool add;
};

```
* for neighbor learnt from vlan device, `NexthopKey::alias` stores the vlan interface name
### MuxOrch::updateNeighbor
```cpp
void MuxOrch::updateNeighbor(const NeighborUpdate &update)
{
    if (mux_cable_tb_.empty())
    {
        return;
    }

    string port, old_port;
    if (update.add)
    {
        auto standalone_tunnel_neigh_it = standalone_tunnel_neighbors_.find(update.entry.ip_address);
        if (!update.mac)
        {
            if (standalone_tunnel_neigh_it == standalone_tunnel_neighbors_.end())
            {
                createStandaloneTunnelRoute(update.entry.ip_address);
            }
            return;
        }

        if (standalone_tunnel_neigh_it != standalone_tunnel_neighbors_.end())
        {
            removeStandaloneTunnelRoute(update.entry.ip_address);
        }

        for (auto it = mux_cable_tb_.begin(); it != mux_cable_tb_.end(); it++)
        {
            MuxCable* ptr = it->second.get();
            if (ptr->isIpInSubnet(update.entry.ip_address))
            {
                ptr->updateNeighbor(update.entry, update.add);
                return;
            }
        }

        // get the port by checking the neighbor update mac in FDB
        if (!getMuxPort(update.mac, update.entry.alias, port))
        {
            return;
        }

        old_port = getNexthopMuxName(update.entry);
        if (port.empty() || old_port == port)
        {
            addNexthop(update.entry, old_port);
            return;
        }

        // no fdb update yet, omit the port
        addNexthop(update.entry);
    }
    else 
    {
        auto it = mux_nexthop_tb_.find(update.entry);
        if (it != mux_nexthop_tb_.end())
        {
            port = it->second;
            removeNexthop(update.entry);
        }
    }

    // remove from the old mux port
    MuxCable* ptr;
    if (!old_port.empty() && old_port != port && isMuxExists(old_port))
    {
        ptr = getMuxCable(old_port);
        ptr->updateNeighbor(update.entry, false);
        addNexthop(update.entry);
    }

    // add to the current mux port
    if (!port.empty() && isMuxExists(port))
    {
        ptr = getMuxCable(port);
        ptr->updateNeighbor(update.entry, update.add);
    }
}
```

### MuxCable::updateNeighbor
* if `add`:
    * register nexthop with port as current mux port in `MuxOrch`
* if not `add`:
    * unregister the nexthop in `MuxOrch` if the port stored is this mux port
```cpp
void MuxCable::updateNeighbor(NextHopKey nh, bool add)
{
    sai_object_id_t tnh = mux_orch_->getNextHopTunnelId(MUX_TUNNEL, peer_ip4_);

    nbr_handler_->update(nh, tnh, add, state_);
    if (add)
    {
        mux_orch_->addNexthop(nh, mux_name_);
    }
    else if (mux_name_ == mux_orch_->getNexthopMuxName(nh))
    {
        mux_orch_->removeNexthop(nh);
    }
}
```
### MuxNbrHandler
```cpp
class MuxNbrHandler
{
public:
    MuxNbrHandler() = default;

    bool enable(bool update_rt);
    bool disable(sai_object_id_t);
    void update(NextHopKey nh, sai_object_id_t, bool = true, MuxState = MuxState::MUX_STATE_INIT);

    sai_object_id_t getNextHopId(const NextHopKey);

private:
    MuxNeighbor neighbors_;             // stores the mapping from the neighbor address and the nexthop id(tunnel id or vlan id)
    string alias_;                      // stores the port alias
};
```

#### MuxNbrHandler::update
* if add:
    * if mux state init
        * add the new neighbor with nexthop id as NULL
    * if mux state active
        * add the new neighbor with nexthop id as the local vlan device
        * `NeighOrch::enableNeighbor`
    * if mux state standby
        * add the new neighbor with nexthop id as the tunnel device
        * `NeighOrch::disableNeighbor`
        * `addTunnelRoute`
* if remove:
    * remove the neighbor
    * if mux state standby
        * `removeTunnelRoute` 

#### MuxNbrHandler::enable
* for every neighbor stored in `MuxNbrHandler::neighbors_`:
    * `NeighOrch::enableNeighbor`
    * change the nexthop id stored in `MuxNbrHandler::neighbors_` as the local vlan id
    * invalid current tunnel nexthop and valid the vlan nexthop
    * if `update_rt`: notify `TunnelMgrd` to remove the kernel tunnel route


#### MuxNbrHandler::disable
* for every neighbor stored in `MuxNbrHandler::neighbors_`:
    * change the nexthop id stored in `MuxNbrHandler::neighbors_` as the tunnel id
    * invalid current vlan nexthop and valid the tunnel nexthop
    * `NeighOrch::disableNeighbor`
    * notify `TunnelMgrd` to add the kernel tunnel route

