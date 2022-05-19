# orchagent details

## class hierarchy

* referenced_object:
    * m_objsDependingOnMe
        * std::set<std::string>
    * m_objsReferencingByMe
        * std::map<std::string, std::string>
    * m_saiObjectId
    * m_pendingRemove

* object_reference_map:
    * std::map<std::string, referenced_object>

* type_map:
    * std::map<std::string, object_reference_map*>

* object_map:
    * std::map<std::string, sai_object_id_t>

* object_map_pair:
    * std::pair<std::string, sai_object_id_t>

* SyncMap: std::multimap<std::string, swss::KeyOpFieldsValuesTuple>

* Executor -> Selectable: adaptor class to Selectable, provide interfaces(execute() and drain()) for orchagent event poll loop
    * m_selectable
    * m_orch
    * m_name: name of the executor
    * getSelectable()
    * getFd()
    * readData()
    * hasCachedData()
    * initializedWithData()
    * updateAfterRead()
    * execute()
    * drain()

* Consumer -> Executor: adaptor class to ConsumerTableBase, provide interfaces(execute() and drain()) for orchagent event poll loop
    * m_toSync: cache of KeyOpFieldsValuesTuple events
    * getConsumerTable()
    * getTableName()
    * getDbId()
    * getDbName()
    * dumpTuple()
    * dumpPendingTasks()
    * execute()
        * keeps calling pops() to get notified DB events and add to m_toSync
        * drain()
    * drain()
        * m_orch->doTask(*this)
    * addToSync(entry)
        * add entry to m_toSync
    * addToSync(entries)
        * add entries to m_toSync
    * refillToSync(table)
        * add entries in table to m_toSync
    * refillToSync()
        * if current consumer table is SubscriberStateTable:
            * SubscriberStateTable::pops(entries)
            * addToSync(entries)
        * else:
            * construct table
            * refillToSync(table)

* Orch:
    * m_consumerMap
        * std::map<std::string, std::shared_ptr<Executor>>
    * m_publisher
    * dumpTuple(tuple)
    * dumpPendingTasks(ts)
    * addExistingData(tableName)
    * addExistingData(table)
        * tableName = table->getTableName
        * getExecutor(tableName)->refillToSync(table)
    * doTask()
        * for every evecutor stored in m_consumerMap, call its drain()
    * doTask(consumer): virtual function expected to be implemented by derived classes
    * doTask(timer)
    * dumpPendingTasks(ts)
    * addConsumer(db, tableName, pri)
        * for CONFIG_DB, STATE_DB, CHASSIS_APP_DB
            * build consumer based on SubscriberStateTable
        * else:
            * build consumer based on ConsumerStateTable
    * addExecutor(executor)
    * getExecutor(executorName)

```
Orch::doTask --> Consumer::drain --> Orch::doTask(Consumer)
Consumer::execute --> Consumer::drain --> orch::doTask(Consumer)
```

* _request_description: struct to provide a `KeyOpFieldsValuesTuple` description
```cpp
typedef enum _request_types_t                                                   // supported types
{
    REQ_T_NOT_USED,
    REQ_T_BOOL,
    REQ_T_STRING,
    REQ_T_MAC_ADDRESS,
    REQ_T_PACKET_ACTION,
    REQ_T_IP,
    REQ_T_IP_PREFIX,
    REQ_T_VLAN,
    REQ_T_UINT,
    REQ_T_SET,
    REQ_T_MAC_ADDRESS_LIST,
    REQ_T_IP_LIST,
    REQ_T_UINT_LIST,
} request_types_t;

typedef struct _request_description
{
    std::vector<request_types_t> key_item_types;                                // key components types
    std::unordered_map<std::string, request_types_t> attr_item_types;           // mapping from attribute to the expected attribute value types
    std::vector<std::string> mandatory_attr_items;                              // list of mandatory attributes
} request_description_t;
```

* Request: class to parse request(`KeyOpFieldsValuesTuple`) that follows the description

* Orch2: Add extra event checker(`Requst`) to verify the key, op, value pairs have expected types
    * request_
    * addOperation(request)
    * delOperation(request)
    * doTask(consumer)

```cpp
void Orch2::doTask(Consumer &consumer)
{
    SWSS_LOG_ENTER();

    auto it = consumer.m_toSync.begin();
    while (it != consumer.m_toSync.end())
    {
        bool erase_from_queue = true;
        try
        {
            request_.parse(it->second);
            auto table_name = consumer.getTableName();
            request_.setTableName(table_name);

            auto op = request_.getOperation();
            if (op == SET_COMMAND)
            {
                erase_from_queue = addOperation(request_);
            }
            else if (op == DEL_COMMAND)
            {
                erase_from_queue = delOperation(request_);
            }
            else
            {
                SWSS_LOG_ERROR("Wrong operation. Check RequestParser: %s", op.c_str());
            }
        }

        catch (...)
        {
            // ...
        }
        request_.clear();

        if (erase_from_queue)
        {
            it = consumer.m_toSync.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
```

* Observer: observes other Orch classes for updates
    * update(SubjectType, void *) = 0

* Subject
    * m_observers
        * list of Observer objects to notify for updates
    * notify(SubjectType, void *)
        * call each observer's Observer::update
    * attach(Observer *)
        * add observer to m_observers
    * detach(Observer *)
        * remove observer to m_observers


* OrchDaemon:
    * m_applDb
    * m_configDb
    * m_stateDb
    * m_chassisAppDb
    * m_orchList
    * m_select
    * init()
        * build m_orchList
    * start()
        * select event loop to 
    * addOrchList(Orch *)
    * flush()

## orchdaemon event loop
```cpp
void OrchDaemon::start()
{
    SWSS_LOG_ENTER();

    for (Orch *o : m_orchList)
    {
        m_select->addSelectables(o->getSelectables());
    }

    auto tstart = std::chrono::high_resolution_clock::now();

    while (true)
    {
        Selectable *s;
        int ret;

        ret = m_select->select(&s, SELECT_TIMEOUT);

        auto tend = std::chrono::high_resolution_clock::now();

        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(tend - tstart);

        if (diff.count() >= SELECT_TIMEOUT)
        {
            tstart = std::chrono::high_resolution_clock::now();

            flush();                                                                    // flush sairedis
        }

        if (ret == Select::ERROR)
        {
            SWSS_LOG_NOTICE("Error: %s!\n", strerror(errno));
            continue;
        }

        if (ret == Select::TIMEOUT)
        {
            /* Let sairedis to flush all SAI function call to ASIC DB.
             * Normally the redis pipeline will flush when enough request
             * accumulated. Still it is possible that small amount of
             * requests live in it. When the daemon has nothing to do, it
             * is a good chance to flush the pipeline  */
            flush();                                                                    // flush sairedis
            continue;
        }

        auto *c = (Executor *)s;                                                        // Executor::execute: pops all events, call Executor::drain
        c->execute();                                                                   // Executor::drain calls Orch::doTask(Consumer)

        /* After each iteration, periodically check all m_toSync map to
         * execute all the remaining tasks that need to be retried. */

        /* TODO: Abstract Orch class to have a specific todo list */
        for (Orch *o : m_orchList)                                                      // there are still chances events left in m_toSync map
            o->doTask();

        /*
         * Asked to check warm restart readiness.
         * Not doing this under Select::TIMEOUT condition because of
         * the existence of finer granularity ExecutableTimer with select
         */
        if (gSwitchOrch && gSwitchOrch->checkRestartReady())
        {
            bool ret = warmRestartCheck();
            if (ret)
            {
                // Orchagent is ready to perform warm restart, stop processing any new db data.
                // Should sleep here or continue handling timers and etc.??
                if (!gSwitchOrch->checkRestartNoFreeze())
                {
                    // Disable FDB aging
                    gSwitchOrch->setAgingFDB(0);

                    // Disable FDB learning on all bridge ports
                    if (gPortsOrch)
                    {
                        for (auto& pair: gPortsOrch->getAllPorts())
                        {
                            auto& port = pair.second;
                            gPortsOrch->setBridgePortLearningFDB(port, SAI_BRIDGE_PORT_FDB_LEARNING_MODE_DISABLE);
                        }
                    }

                    // Flush sairedis's redis pipeline
                    flush();

                    SWSS_LOG_WARN("Orchagent is frozen for warm restart!");
                    sleep(UINT_MAX);
                }
            }
        }
    }
}
```


## cfgmgr

### vlanmgrd
* event loop
```cpp
        vector<string> cfg_vlan_tables = {
            CFG_VLAN_TABLE_NAME,
            CFG_VLAN_MEMBER_TABLE_NAME,
        };

        DBConnector cfgDb("CONFIG_DB", 0);
        DBConnector appDb("APPL_DB", 0);
        DBConnector stateDb("STATE_DB", 0);

        VlanMgr vlanmgr(&cfgDb, &appDb, &stateDb, cfg_vlan_tables);                     // [1]: build consumers map

        std::vector<Orch *> cfgOrchList = {&vlanmgr};

        swss::Select s;
        for (Orch *o : cfgOrchList)
            s.addSelectables(o->getSelectables());                                      // [2]: add consumers to selectable list

        SWSS_LOG_NOTICE("starting main loop");
        while (true)
        {
            Selectable *sel;
            int ret;

            ret = s.select(&sel, SELECT_TIMEOUT);                                       // [3]: check for event ready Selectable
            if (ret == Select::ERROR)
            {
                SWSS_LOG_NOTICE("Error: %s!", strerror(errno));
                continue;
            }
            if (ret == Select::TIMEOUT)
            {
                vlanmgr.doTask();                                                       // [4]: VlanMgr::doVlanTask
                continue;
            }

            auto *c = (Executor *)sel;
            c->execute();                                                               // [5]
        }
```

* [1]: `VlanMgr` is a derived class of `Orch`[1]
    * `Orch` will build consumers map from `cfg_vlan_tables` passed from `VlanMgr` constructor
    * the consumers are built out of `SubscriberStateTable` because `cfg_vlan_tables` are all from `CONFIG_DB`
* [2]: add consumers to `Select` object's selectables list
    * all consumers are initialized with data if the table is not empty(`initializedWithData()` is True) so they could be put into `Select`'s ready queue.
* [3]: check for event ready `Selectable`
* [4]: `VlanMgr::doTask`: call corresponding handler to consume the events stored in the consumer
* [5]: try to pops out all notification events and call corresponding handler to consume the events


## muxorch
```
linkmgrd  -> APP_DB:MUX_CABLE_TABLE -> swss -> APP_DB:HW_MUX_CABLE_TABLE -> xcvrd
xcvrd -> STATE_DB:HW_MUX_CABLE_TABLE -> swss -> STATE_DB:MUX_CABLE_TABLE -> linkmgrd 

linkmgrd  -> APP_DB:MUX_CABLE_TABLE : MuxPort::setMuxState
STATE_DB:MUX_CABLE_TABLE -> linkmgrd : handleGetMuxStateNotification or  handleMuxStateNotification
```
* MuxAclHandler: adding/removing ACLs to drop packets from server for standby mux port
    * acl_table_
    * port_
        * port sai obj id
    * alias
        * port name
    * createMuxAclTable
    * createMuxAclRule
    * bindAllPorts

* MuxNbrHandler: adding/removing neighbors #TODO
    * neighbors_
    * alias_
    * enable
    * disable
    * update

* MuxOrch -> Orch2, Observer, Subject: builds and store MuxCable objects
    * mux_peer_switch
        * peer switch ip
    * mux_tunnel_id_
        * mux tunnel sai obj ID
    * mux_cable_tb_
        * mapping from port name to MuxCable objects
    * mux_tunnel_nh_
        * mapping from server IP to NHTunnel
    * mux_nexthop_tb_
    * handler_map_
        * CONFIG_DB:MUX_CABLE -> MuxOrch::handleMuxCfg
        * CONFIG_DB:PEER_SWITCH -> MuxOrch::handlePeerSwitch
    * decap_orch_
    * neighbor_orch_: MuxOrch is observer of NeighborOrch
    * fdb_orch_: MuxOrch is observer of FdbOrch
    * request_: a MuxCfgRequest object that describes the table key, attribute value types
    * addOperation(request)
        * dispatch add request event based on source table
            * handleMuxCfg
            * handlePeerSwitch
    * delOperation(request)
        * dispatch del request event based on source table
            * handleMuxCfg
            * handlePeerSwitch
    * handleMuxCfg
        * handle events from CONFIG_DB:MUX_CABLE
        * build mux_cable_tb_
    * handlePeerSwitch
        * handle events from CONFIG_DB:PEER_SWITCH
        * build the mux tunnel(mux_tunnel_id_)

* MuxCable: describe a mux cable object
    * mux_name_
        * port name
    * state_
        * current mux state
    * st_chg_in_progress
    * st_chg_failed
    * srv_ipv4_
        * server ipv4 address
    * srv_ipv6_
        * server ipv6 address
    * peer_ipv4_
        * peer switch ipv4 address
    * mux_orch_
        * MuxOrch obj
    * mux_cb_orch_
        * MuxCableOrch obj
    * mux_state_orch_
        * MuxStateOrch obj
    * acl_handler
    * nbr_handler_
    * state_machine_handlers_
        * mapping from state change to handlers
    * setState
        * call state transition functions to handle neighbor, tunnel, acl modifications
    * stateInitActive
    * stateActive
    * stateStandby

* MuxStateOrch -> Orch2: consumer of STATE_DB::HW_MUX_CABLE_TABLE and producer of STATE_DB:MUX_CABLE_TABLE
    * mux_state_table_
        * STATE_DB:MUX_CABLE_TABLE
    * request_
    * addOperation
        * verify the mux state from xcvrd is the same as the state stored in the MuxCable object
            * if same, write the state to mux_state_table_
            * if not same:
                * if last toggle failed, write mux state error to mux_state_table_
                * if last toggle succeeded, write mux state unknown to mux_state_table_

* MuxCableOrch -> Orch2: consumer of APP_DB:MUX_CABLE_TABLE and producer of APP_DB:HW_MUX_CABLE_TABLE
    * mux_table_
        * APP_DB:HW_MUX_CABLE_TABLE
    * request_
    * mux_metric_table_
    * app_tunnel_route_table_
        * APP_DB:TUNNEL_ROUTE_TABLE
    * addOperation
        * parse the state and call MuxCable::setState
    * delOperation
    * updateMuxState
        * update state to mux_table_

## references
* https://chowdera.com/2021/10/20211029112902093b.html
