# swss common table

## table classes
* `TableBase`: defines the base class for a redis table, a table consists of table name and table separator
    * `m_tableName`
    * `m_tableSeparator`
    * `getTableName()`
    * `getKeyName(key)`: `m_tableName + m_tableSeparator + key`
    * `getTableNameSeparator()`
    * `getChannelName()`: `m_tableName + "_CHANNEL"`
    * `getChannelName(tag)`: `m_tableName + "_CHANNEL" + "@" + tag`

* `TableEntryWritable`: defines writable APIs
    * `set`
    * `del`

* `TableEntryPoppable`: defines popable table
    * `pop`
    * `pops`

* `TableEntryEnumerable`: defines enumable table
    * `get`
    * `getKeys`
    * `getContent`

* `TableName_KeyValueOpQueues`: defines a list(queue) name for a table
    * `m_keyvalueop`: `tableName + "_KEY_VALUE_OP_QUEUE"`
    * `getKeyValueOpQueueTableName()`

* `TableName_KeySet`: defines names of two sets, used for key set and key del
    * `m_key`: `tableName + "_KEY_SET"`
    * `m_delkey`: `tableName + "_DEL_SET"`
    * `getKeySetName()`
    * `getDelKeySetName()`
    * `getStateHashPrefix()`: returns `"_"`

* `TableConsumable`(`TableBase`, `TableEntryPoppable`, `RedisSelect`): base selectable table class

* `Table`(`TableBase`, `TableEntryEnumerable`): class to represent a redis table
    * `Table(db, tableName)`
        * when init with a db connector, the self-owned `m_pipe` buffer size is 1 and `m_buffered` is `false`
    * `Table(pipeline, tableName, buffered)`
        * when init with a shared pipeline, user could use buffer with `m_buffered` as `true`
    * `m_buffered`
    * `m_pipeowned`
    * `m_pipe`
    * `m_shaDump`
    * `set(key, values)`
    * `get(key, values)`
    * `del(key)`
    * `hdel(key, field)`
    * `hget(key, field, value)`
    * `hset(key, field, value)`
    * `ttl(key, ttl_time)`
    * `getKeys(keys)`: return all keys defined in the table(stripped the table name and separator)
    * `getContent(tuples)`: return the table content
    * `dump(tableDump)`: dump the table content with lua script `table_dump.lua`
 
 
