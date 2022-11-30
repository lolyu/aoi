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
`MuxCableOrch` subscribes to `APP_TUNNEL_ROUTE_TABLE_NAME` for any mux state changes from `linkmgrd` and call the state transition functions defined in `MuxCable` obejct.

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
`MuxStateOrch` subscribes to `




## class MuxCable
