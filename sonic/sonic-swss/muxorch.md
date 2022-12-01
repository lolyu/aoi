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

#### MuxCable::stateInitActive
* `nbrhandler(true, false)`
#### MuxCable::stateActive
* `nbrhandler(true)`
* `aclhandler(..., false)`
#### MuxCable::stateStandby
* `nbrhandler(false)`
* `aclhandler(..., true)`

## fdb handling

## neighbor handling
