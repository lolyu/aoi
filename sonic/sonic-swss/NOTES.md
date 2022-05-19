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
    * doTask(consumer)
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

* Orch2:
    * request_

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

* MuxOrch -> Orch2, Observer, Subject:
    * mux_peer_switch
    * mux_tunnel_id_
    * mux_cable_tb_
    * mux_tunnel_nh_
    * mux_nexthop_tb_
    * handler_map_
        * CONFIG_DB:MUX_CABLE -> MuxOrch::handleMuxCfg
        * CONFIG_DB:PEER_SWITCH -> MuxOrch::handlePeerSwitch
    * decap_orch_
    * neighbor_orch_
    * fdb_orch_
    * request_: a MuxCfgRequest object that describes the table key, attribute value types

## references
* https://chowdera.com/2021/10/20211029112902093b.html
