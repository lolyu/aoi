# swss-common notes

## db schema
| db id | db name         | description                                                      |
| ----- | --------------- | ---------------------------------------------------------------- |
| 0     | APP_DB          | DB used for SONiC applications                                   |
| 1     | ASIC_DB         | ASIC states and configurations                                   |
| 2     | COUNTERS_DB     | counters statistics                                              |
| 3     | LOGLEVEL_DB     | log configurations                                               |
| 4     | CONFIG_DB       | system level configurations                                      |
| 5     | FLEX_COUNTER_DB | flex counters                                                    |
| 6     | STATE_DB        | running states, stores dependencies between modules/applications |


## message definitions
```cpp
typedef std::pair<std::string, std::string> FieldValueTuple;
#define fvField std::get<0>
#define fvValue std::get<1>
typedef std::tuple<std::string, std::string, std::vector<FieldValueTuple> > KeyOpFieldsValuesTuple;
#define kfvKey    std::get<0>
#define kfvOp     std::get<1>
#define kfvFieldsValues std::get<2>
```

## redis related


* redisContext: setup connection to a Redis instance
* redisReply: Redis command reply

* RedisInstInfo: contains information of a certain Redis instance
    * unixSocketPath
    * hostname
    * port

* SonicDBInfo: contains information of a certain SONiC DB
    * instName
    * dbId
    * separator

* SonicDBConfig: read/store SONiC DB configuration
    * m_inst_info: mapping from db name to RedisInstInfo obj
    * m_db_info: mapping from db name to SonicDBInfo obj
    * m_db_separator: mapping from db name to separator

```
>>> from swsscommon import swsscommon
>>> swsscommon.SonicDBConfig
<class 'swsscommon.swsscommon.SonicDBConfig'>
">>> swsscommon.SonicDBConfig.getSeparator("APPL_DB")
':'
>>> swsscommon.SonicDBConfig.getDbInst("APPL_DB", "")
'redis'
>>> swsscommon.SonicDBConfig.getDbList()
('STATE_DB', 'APPL_DB', 'GB_FLEX_COUNTER_DB', 'APPL_STATE_DB', 'ASIC_DB', 'CONFIG_DB', 'COUNTERS_DB', 'LOGLEVEL_DB', 'GB_ASIC_DB', 'GB_COUNTERS_DB', 'PFC_WD_DB', 'FLEX_COUNTER_DB', 'RESTAPI_DB', 'SNMP_OVERLAY_DB')
```

* RedisCommand: class to format Redis command string

* RedisReply: issue command to a RedisCommand and stores the redisReply, could act as an auto_ptr for a redisReply object

* DbConnector -> RedisContext: defines common Redis functions
```
>>> db = swsscommon.DBConnector("CONFIG_DB", 0, True)
>>> db.keys("PORT|Ethernet*")
('PORT|Ethernet0', 'PORT|Ethernet40', 'PORT|Ethernet104', 'PORT|Ethernet68', 'PORT|Ethernet96', 'PORT|Ethernet44', 'PORT|Ethernet64', 'PORT|Ethernet112', 'PORT|Ethernet124', 'PORT|Ethernet80', 'PORT|Ethernet116', 'PORT|Ethernet12', 'PORT|Ethernet100', 'PORT|Ethernet4', 'PORT|Ethernet72', 'PORT|Ethernet88', 'PORT|Ethernet56', 'PORT|Ethernet84', 'PORT|Ethernet120', 'PORT|Ethernet108', 'PORT|Ethernet24', 'PORT|Ethernet92', 'PORT|Ethernet48', 'PORT|Ethernet60', 'PORT|Ethernet28', 'PORT|Ethernet36', 'PORT|Ethernet20', 'PORT|Ethernet32', 'PORT|Ethernet52', 'PORT|Ethernet76', 'PORT|Ethernet8', 'PORT|Ethernet16')
```

* RedisPipeline
    * m_db
    * m_expectedTypes: queue of pipelined Redis commands return types
    * m_remaining: pipelined commands counter
    * push(command, expectedType)
        * only `REDIS_REPLY_INTEGER` reply will be cached, others will be executed directly
    * push(command): cache the command into pipeline
    * pop(): use `redisReply` to fetch a reply from the pipelined commands
    * flush(): fetch all results of pipelined commands
    * size(): return m_remaining
```
>>> from swsscommon import swsscommon
>>> db = swsscommon.DBConnector("CONFIG_DB", 0, True)
>>> pipe = swsscommon.RedisPipeline(db)
>>> cmd0 = swsscommon.RedisCommand()
>>> cmd1 = swsscommon.RedisCommand()
>>> cmd0.formatTTL("BGP_NEIGHBOR|10.0.0.63")
>>> cmd1.formatTTL("BGP_NEIGHBOR|10.0.0.53")
>>> pipe.push(cmd0, 3)                      // REDIS_REPLY_INTEGER
>>> pipe.push(cmd1, 3)                      // REDIS_REPLY_INTEGER
>>> reply0 = pipe.pop()
>>> reply1 = pipe.pop()
```

* RedisTransactioner: defines wrapper to allow executions of commands in a single step(Redis transaction)
    * m_db
    * m_expectedResults: deque of reply types
    * m_results: deque of redisReply *
    * multi(): starts a transaction
    * enqueue(command, expectedType)
        * enqueue a command and its expected return type
    * exec(): 
```
>>> db = swsscommon.DBConnector("CONFIG_DB", 0, True)
>>> transaction = swsscommon.RedisTransactioner(db)
>>> cmd0 = swsscommon.RedisCommand()
>>> cmd1 = swsscommon.RedisCommand()
>>> cmd0.formatTTL("BGP_NEIGHBOR|10.0.0.63")
>>> cmd1.formatTTL("BGP_NEIGHBOR|10.0.0.53")
>>> transaction.multi()
>>> transaction.enqueue(cmd0, 3)
>>> transaction.enqueue(cmd1, 3)
>>> transaction._exec()
True
>>> reply0 = transaction.dequeueReply()
```

## select system
* Selectable: virtual base class to represent a selectable object
    * m_last_used_time: last used time
    * m_priority: selectable priority
    * readData
    * hasData
    * hasCachedData
    * initializedWithData

* Select: class to poll selectables
    * m_epoll_fd: epoll fd
    * m_objects: fd to Selectable mapping
    * m_ready: Selectables that are ready for IO read event, sorted
        * Selectables with higher priority || earliest last used time are ranked first
    * poll_descriptors
        * poll Selectables in m_objects and insert those ready for IO into m_ready
        * for every Seletable in m_ready(higher priority || earliest last used time)
            * remove from m_ready
            * update last used time(Selectable::updateLastUsedTime)
            * if Selectable not has data(Selectable::hasData), continue to handle next Selectable
            * if Selectable has cached data(Selectable::hasCachedData), put back into m_ready
            * Selectable::updateAfterRead

* RedisSelect -> Selectable: redis subscribe client that is selectable

```python
>>> from swsscommon import swsscommon
>>> r = swsscommon.RedisSelect(10)
>>> db = swsscommon.DBConnector("CONFIG_DB", 0, True)
>>> r.subscribe(db, "hellochannel")
>>>
>>> r.readData()
0
>>> r.hasData()
True
```

## table class hierachy

* TableBase: defines table name and table separator
    * TABLE_NAME_SEPARATOR_COLON: `:`
    * TABLE_NAME_SEPARATOR_VBAR: `|`
    * tableNameSeparatorMap: mapping from db id to db separator
    * m_tableName:
    * m_tableSeparator
    * getTableName
    * getKeyName(const std::string &key)
        * m_tableName + m_tableSeparator + key
    * getTableNameSeparator
    * getChannelName
        * m_tableName + "_CHANNEL"

### table operation interface virtual classes

* TableEntryWritable: defines writable tables
    * set(key, values, op, prefix)
    * del(key, op, prefix)

* TableEntryPoppable: defineds consumerable tables(popable), could pops out multiple results
    * pops(keys, ops, values, prefix)
    * pops(vkco, prefix)

* TableEntryEnumerable: defines readable tables
    * get(key, values)
    * getKeys(keys)
    * getContent

* TableName_KeyValueOpQueues
    * m_keyvalueop: tableName + "_KEY_VALUE_OP_QUEUE"
    * getKeyValueOpQueueTableName()

* TableName_KeySet
    * m_key: tableName + "_KEY_SET"
    * m_delkey: tableName + "_DEL_SET"
    * getKeySetName()
    * getDelKeySetName()
    * getStateHashPrefix(): "_"

### table interface classes
* TableConsumable -> TableBase, TableEntryPoppable, RedisSelect

* ConsumerTableBase -> TableConsumable, RedisTransactioner: defines consumerable base class
    * m_buffer: deque of KeyOpFieldsValuesTuple, stores poped-out results
    * pop(kco, prefix)
    * pop(key, op, fvs, prefix)

* Table -> TableBase, TableEntryEnumerable: defines what a db table
    * m_buffered: true to enable pipeline
    * m_pipeowned: true to own m_pipe
    * m_pipes: RedisPipeline object
    * m_shaDump: Lua script SHA to dump a table
        * table_dump.lua

```
>>> db = swsscommon.DBConnector("CONFIG_DB", 0, True)
>>> port_table = swsscommon.Table(db, "PORT")
>>> port_table.getKeys()
('Ethernet0', 'Ethernet40', 'Ethernet104', 'Ethernet68', 'Ethernet96', 'Ethernet44', 'Ethernet64', 'Ethernet112', 'Ethernet124', 'Ethernet80', 'Ethernet116', 'Ethernet12', 'Ethernet100', 'Ethernet4', 'Ethernet72', 'Ethernet88', 'Ethernet56', 'Ethernet84', 'Ethernet120', 'Ethernet108', 'Ethernet24', 'Ethernet92', 'Ethernet48', 'Ethernet60', 'Ethernet28', 'Ethernet36', 'Ethernet20', 'Ethernet32', 'Ethernet52', 'Ethernet76', 'Ethernet8', 'Ethernet16')
```



## SONiC message systems
| type                                       | summary                                                                                                   |
| ------------------------------------------ | --------------------------------------------------------------------------------------------------------- |
| ProducerTable && ConsumerTable             | ProducerTable use list to queue (key, values op) and<br /> use pub/sub to notify ConsumerTable key events |
| ProducerStateTable && SubscriberStateTable |                                                                                                           |


### ProducerTable && ConsumerTable

* ProducerTable -> TableBase, TableNameKeyValueOpQueues
    * m_dumpFile
    * m_firstItem
    * m_pipeowned
    * m_pipe
    * m_shaEnque
        * "redis.call('LPUSH', KEYS[1], ARGV[1], ARGV[2], ARGV[3]); redis.call('PUBLISH', KEYS[2], ARGV[4]);";
        * KEYS[1]: tableName + "_KEY_VALUE_OP_QUEUE"
        * KEYS[2]: tableName + "_CHANNEL"
        * ARGV[1]: key
        * ARGV[2]: value
        * ARGV[3]: op
        * ARGV[4]: "G"
    * set(key, values, op, prefix)
        * LPUSH (tableName + "_KEY_VALUE_OP_QUEUE") key (json(values)) ("S" + op)
        * PUBLISH (tableName + "_CHANNEL") "G"
    * del(key, op, prefix)
        * LPUSH (tableName + "_KEY_VALUE_OP_QUEUE") key "{}" ("D" + op)
        * PUBLISH (tableName + "_CHANNEL") "G"

```
>>> db = swsscommon.DBConnector("CONFIG_DB", 0, True)
>>> p = swsscommon.ProducerTable(db, "EMPLOYEE")
>>> values = swsscommon.FieldValuePairs([('name', 'alice'), ('age', '18')])
>>> p.set("ALICE", values)
```
```
$ redis-cli monitor | grep EMPLOYEE
1651572314.750379 [4 127.0.0.1:39780] "EVALSHA" "d171e04fd79e95ca2287f3b067c46ae76a82208b" "2" "EMPLOYEE_KEY_VALUE_OP_QUEUE" "EMPLOYEE_CHANNEL@4" "ALICE" "[\"name\",\"alice\",\"age\",\"18\"]" "SSET" "G"
1651572314.750580 [4 lua] "LPUSH" "EMPLOYEE_KEY_VALUE_OP_QUEUE" "ALICE" "[\"name\",\"alice\",\"age\",\"18\"]" "SSET"
1651572314.751067 [4 lua] "PUBLISH" "EMPLOYEE_CHANNEL@4" "G"
```

* ConsumerTable -> ConsumerTableBase, TableName_KeyValueOpQueues
    * m_shaPop: consumer_table_pops.lua
    * m_modifyRedis: true to modify db
    * pops(vkco, prefix)
        * consumer_table_pops.lua
        * KEYS[1]: tableName + "_KEY_VALUE_OP_QUEUE"
        * KEYS[2]: prefix + tableName
        * ARGV[1]: POP_BATCH_SIZE
        * ARGV[2]: m_modifyRedis ? 1 : 0;
```
>>> db = swsscommon.DBConnector("CONFIG_DB", 0, True)
>>> p = swsscommon.ProducerTable(db, "EMPLOYEE")
>>> values = swsscommon.FieldValuePairs([('name', 'alice'), ('age', '18')])
>>> c = swsscommon.ConsumerTable(db, "EMPLOYEE")
>>> p.set("ALICE", values)
>>> c.pops()
[('ALICE',), ('SET',), ((('name', 'alice'), ('age', '18')),)]
```

### ProducerStateTable && SubscriberStateTable

* ProducerStateTable -> TableBase, TableName_KeySet
    * m_buffered
    * m_pipeowned
    * m_tempViewActive
    * m_pipe
    * m_shaSet
    * m_shaDel
    * m_shaBatchedSet
    * m_shaBatchedDel
    * m_shaClear
    * m_shaApplyView
    * m_tempViewState
    * set(key, values, op, prefix)
    * del(key, op, prefix)

* SubscriberStateTable -> ConsumerTableBase
    * m_keyspace
    * m_keyspace_event_buffer: std::deque<std::shared_ptr<RedisReply>>
    * m_table