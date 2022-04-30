# class hierarchy

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
* RedisReply: issue command to a RedisCommand and stores the redisReply
* DbConnector -> RedisContext: defines common Redis functions
```
>>> db = swsscommon.DBConnector("CONFIG_DB", 0, True)
>>> db.keys("PORT|Ethernet*")
('PORT|Ethernet0', 'PORT|Ethernet40', 'PORT|Ethernet104', 'PORT|Ethernet68', 'PORT|Ethernet96', 'PORT|Ethernet44', 'PORT|Ethernet64', 'PORT|Ethernet112', 'PORT|Ethernet124', 'PORT|Ethernet80', 'PORT|Ethernet116', 'PORT|Ethernet12', 'PORT|Ethernet100', 'PORT|Ethernet4', 'PORT|Ethernet72', 'PORT|Ethernet88', 'PORT|Ethernet56', 'PORT|Ethernet84', 'PORT|Ethernet120', 'PORT|Ethernet108', 'PORT|Ethernet24', 'PORT|Ethernet92', 'PORT|Ethernet48', 'PORT|Ethernet60', 'PORT|Ethernet28', 'PORT|Ethernet36', 'PORT|Ethernet20', 'PORT|Ethernet32', 'PORT|Ethernet52', 'PORT|Ethernet76', 'PORT|Ethernet8', 'PORT|Ethernet16')
```

## select system
* Selectable: virtual base class to represent a selectable object
    * m_last_used_time: last used time
    * m_priority: selectable priority