# orchagent details

## class hierarchy

referenced_object:
    * m_objsDependingOnMe
        * std::set<std::string>
    * m_objsReferencingByMe
        * std::map<std::string, std::string>
    * m_saiObjectId
    * m_pendingRemove

object_reference_map:
    * std::map<std::string, referenced_object>

type_map:
    * std::map<std::string, object_reference_map*>

object_map:
    * std::map<std::string, sai_object_id_t>

object_map_pair:
    * std::pair<std::string, sai_object_id_t>

SyncMap: std::multimap<std::string, swss::KeyOpFieldsValuesTuple>

Executor -> Selectable: adaptor class to Selectable, provide interfaces(execute() and drain()) for orchagent event poll loop
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

Consumer -> Executor: adaptor class to ConsumerTableBase, provide interfaces(execute() and drain()) for orchagent event poll loop
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

Orch:
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