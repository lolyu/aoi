# ProducerStateTable and ConsumerStateTable
* **terms:**
    * table entry key: a table entry name
    * table key: table name + table separator + table entry key

## ProducerStateTable
* `ProducerStateTable`(`TableBase`, `TableName_KeySet`):
    * `m_buffered`: `false` to flush the `m_pipe` every time pushing commands to it
    * `m_pipeowned`: `true` if `m_pipe` is self-owned
    * `m_tempViewActive`: `true` to modify `m_tempViewState` instead modify DB
    * `m_pipe`: `RedisPipeline` object
    * `m_shaSet`: set operation Lua script SHA value
    * `m_shaDel`: del operation Lua script SHA value
    * `m_shaBatchedSet`: batched set operation Lua script SHA value
    * `m_shaBatchedDel`: batched del operation Lua script SHA value
    * `m_shaClear`: clear operation Lua script SHA value
    * `m_shaApplyView`: apply view operation Lua script SHA value
    * `m_tempViewState`: a `TableDump` object as in-memory temporary view of the table
    * `set(key, values, op, prefix)`
    * `set(values)`
    * `del(key, op, prefix)`
    * `del(keys)`
    * `clear()`
    * `count()`: returns the key set size

### set
```lua
local added = redis.call('SADD', KEYS[2], ARGV[2])
for i = 0, #KEYS - 3 do
    redis.call('HSET', KEYS[3 + i], ARGV[3 + i * 2], ARGV[4 + i * 2])
end
if added > 0 then
    redis.call('PUBLISH', KEYS[1], ARGV[1])
end;
```
```
1665211803.270162 [0 127.0.0.1:58720] "EVALSHA" "6875900592cdd1621c6191fe038ec3b29775aa13" "4" "PSEUDOTABLE_CHANNEL@0" "PSEUDOTABLE_KEY_SET" "_PSEUDOTABLE:ENTRY0" "_PSEUDOTABLE:ENTRY0" "G" "ENTRY0" "key0" "value0" "key1" "value1"
1665211803.270207 [0 lua] "SADD" "PSEUDOTABLE_KEY_SET" "ENTRY0"
1665211803.270255 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY0" "key0" "value0"
1665211803.270271 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY0" "key1" "value1"
1665211803.270282 [0 lua] "PUBLISH" "PSEUDOTABLE_CHANNEL@0" "G"
```
* the set op:
    1. add the table entry key to the key set
    2. store its field/value pairs into a temporary table key
    3. publish this key change to the channel if the table entry key is new to the key set in step#1
* `mShaSet` parameters:
    * `KEYS[1]`: the to-publish event channel name
        * `"PSEUDOTABLE_CHANNEL@0"`
    * `KEYS[2]`: the set name that stores table entry keys
        * `"PSEUDOTABLE_KEY_SET"`
    * `KEYS[3]` ~ `KEYS[N + 2]`(`N` is `values.size()`): the temporary table name
        * `"_PSEUDOTABLE:ENTRY0"`
    * `ARGV[1]`: the message to publish
        * `"G"`
    * `ARGV[2]`: the table entry key
        * "ENTRY0"
    * `ARGV[3]` ~ `ARGV[N * 2 + 2`]`: the content of key value pairs
        * `"key0" "value0" "key1" "value1"`

#### batched set
```lua
local added = 0
local idx = 2
for i = 0, #KEYS - 4 do
    added = added + redis.call('SADD', KEYS[2], KEYS[4 + i])
    for j = 0, tonumber(ARGV[idx]) - 1 do
        local attr = ARGV[idx + j * 2 + 1]
        local val = ARGV[idx + j * 2 + 2]
        redis.call('HSET', KEYS[3] .. KEYS[4 + i], attr, val)
    end\n"
    idx = idx + tonumber(ARGV[idx]) * 2 + 1
end\n"
if added > 0 then
    redis.call('PUBLISH', KEYS[1], ARGV[1])
end
```
```
1665222210.783518 [0 127.0.0.1:57046] "EVALSHA" "bc885d7acbeaba07a97ca9fa2cae169a5d63270a" "5" "PSEUDOTABLE_CHANNEL@0" "PSEUDOTABLE_KEY_SET" "_PSEUDOTABLE:" "ENTRY1" "ENTRY2" "G" "2" "key0" "value0" "key1" "value1" "2" "key0" "value0" "key1" "value1"
1665222210.783544 [0 lua] "SADD" "PSEUDOTABLE_KEY_SET" "ENTRY1"
1665222210.783584 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY1" "key0" "value0"
1665222210.783597 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY1" "key1" "value1"
1665222210.783608 [0 lua] "SADD" "PSEUDOTABLE_KEY_SET" "ENTRY2"
1665222210.783618 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY2" "key0" "value0"
1665222210.783629 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY2" "key1" "value1"
1665222210.783639 [0 lua] "PUBLISH" "PSEUDOTABLE_CHANNEL@0" "G"
```
* `mShaBatchedSet` parameters:
    * `KEYS[1]`: the to-publish event channel name
        * `"PSEUDOTABLE_CHANNEL@0"`
    * `KEYS[2]`: the set name that stores table entry keys
        * `"PSEUDOTABLE_KEY_SET"`
    * `KEYS[3]`: the temporary table prefix
        * `"_PSEUDOTABLE:"`
    * `KEYS[4]` ~ `KEYS[N + 3]`(`N` is `values.size()`): the temporary table entry name
        * `"ENTRY1" "ENTRY2"`
    * `ARGV[1]`: the message to publish
        * `"G"`
    * the following `ARGV` are packed into sequences like: `M key[0] value[0] ... key[M] value[M]` where `M = key value pair count for i-th table entry key`

### del
```lua
local added = redis.call('SADD', KEYS[2], ARGV[2])
redis.call('SADD', KEYS[4], ARGV[2])
redis.call('DEL', KEYS[3])
if added > 0 then
    redis.call('PUBLISH', KEYS[1], ARGV[1])
end
```
```
1665220278.129774 [0 127.0.0.1:51858] "EVALSHA" "88ba6312b8de850b3506966425174d8899aadd93" "4" "PSEUDOTABLE_CHANNEL@0" "PSEUDOTABLE_KEY_SET" "_PSEUDOTABLE:ENTRY0" "PSEUDOTABLE_DEL_SET" "G" "ENTRY0" "''" "''"
1665220278.129793 [0 lua] "SADD" "PSEUDOTABLE_KEY_SET" "ENTRY0"
1665220278.129797 [0 lua] "SADD" "PSEUDOTABLE_DEL_SET" "ENTRY0"
1665220278.129815 [0 lua] "DEL" "_PSEUDOTABLE:ENTRY0"
```
* the del op:
    * add the table entry key to the key set
    * add the table entry key to the del set
    * delete the temporary table key
    * publish this key change to the channel if the table entry key is new to the key set in step#1
* `m_shaDel` parameters:
    * `KEYS[1]`: the to-publish event channel name
        * `"PSEUDOTABLE_CHANNEL@0"`
    * `KEYS[2]`: the set name that stores table entry keys
        * `"PSEUDOTABLE_KEY_SET"`
    * `KEYS[3]`: the temporary table name
        * `"_PSEUDOTABLE:ENTRY0"`
    * `KEYS[4]`: the set name that stores del table entry keys
        * `"PSEUDOTABLE_DEL_SET"`
    * `ARGV[1]`: the message to publish
        * `"G"`
    * `ARGV[2]`: the table entry key
        * "ENTRY0"
    * `ARGV[3]`: empty string
        * `"''"`
    * `ARGV[4]`: empty string
        * `"''"`

#### batch del
```lua
local added = 0
for i = 0, #KEYS - 5 do
    added = added + redis.call('SADD', KEYS[2], KEYS[5 + i])
    redis.call('SADD', KEYS[3], KEYS[5 + i])
    redis.call('DEL', KEYS[4] .. KEYS[5 + i])
end
if added > 0 then
    redis.call('PUBLISH', KEYS[1], ARGV[1])
end
```
```
1665222210.783733 [0 127.0.0.1:57046] "EVALSHA" "c6025eb92295c3a694dcac788ed80401da2cee95" "6" "PSEUDOTABLE_CHANNEL@0" "PSEUDOTABLE_KEY_SET" "PSEUDOTABLE_DEL_SET" "_PSEUDOTABLE:" "ENTRY1" "ENTRY2" "G"
1665222210.783749 [0 lua] "SADD" "PSEUDOTABLE_KEY_SET" "ENTRY1"
1665222210.783753 [0 lua] "SADD" "PSEUDOTABLE_DEL_SET" "ENTRY1"
1665222210.783765 [0 lua] "DEL" "_PSEUDOTABLE:ENTRY1"
1665222210.783775 [0 lua] "SADD" "PSEUDOTABLE_KEY_SET" "ENTRY2"
1665222210.783779 [0 lua] "SADD" "PSEUDOTABLE_DEL_SET" "ENTRY2"
1665222210.783788 [0 lua] "DEL" "_PSEUDOTABLE:ENTRY2"
```
* `m_shaBatchedDel` parameters:
    * `KEYS[1]`: the to-publish event channel name
        * `"PSEUDOTABLE_CHANNEL@0"`
    * `KEYS[2]`: the set name that stores table entry keys
        * `"PSEUDOTABLE_KEY_SET"`
    * `KEYS[3]`: the set name that stores del table entry keys
        * `"PSEUDOTABLE_DEL_SET"`
    * `KEYS[4]`: the temporary table prefix
        * `"_PSEUDOTABLE:ENTRY0"`
    * `KEYS[5]` ~ `KEYS[N + 4]`(`N` is `values.size()`): the temporary table entry name
        * `"ENTRY1" "ENTRY2"`
    * `ARGV[1]`: the message to publish
        * `"G"`

### clear
```lua
redis.call('DEL', KEYS[1])
local keys = redis.call('KEYS', KEYS[2] .. '*')
for i,k in pairs(keys) do
    redis.call('DEL', k)
end
redis.call('DEL', KEYS[3])
```
```
1665223712.734171 [0 127.0.0.1:60272] "EVALSHA" "a7a229c9946f655aebc07e29d5e4d8ff5055ded8" "3" "PSEUDOTABLE_KEY_SET" "_PSEUDOTABLE" "PSEUDOTABLE_DEL_SET"
1665223712.734182 [0 lua] "DEL" "PSEUDOTABLE_KEY_SET"
1665223712.734194 [0 lua] "KEYS" "_PSEUDOTABLE*"
1665223712.734224 [0 lua] "DEL" "_PSEUDOTABLE:ENTRY2"
1665223712.734235 [0 lua] "DEL" "_PSEUDOTABLE:ENTRY1"
1665223712.734244 [0 lua] "DEL" "PSEUDOTABLE_DEL_SET"
```
* `m_shaClear` parameters:
    * `KEYS[1]`: the set name that stores table entry keys
        * `"PSEUDOTABLE_KEY_SET"`
    * `KEYS[2]`: the temporary table prefix
        * `"_PSEUDOTABLE"`
    * `KEYS[3]`: the set name that stores del table entry keys
        * `"PSEUDOTABLE_DEL_SET"`

### temp view
* if temp view is enabled, all key modifications(`set` or `del`) will be applied to a in-memory map(`m_tempViewState`)

#### apply view
```lua
--[[
Sample args format:
KEYS:
   SAMPLE_CHANNEL
   SAMPLE_KEY_SET
   SAMPLE_DEL_KEY_SET
   _SAMPLE:key_0
   _SAMPLE:key_1
 ARGV:
   G   (String to be published to channel)
   2   (Count of objects to set)
   key_0
   key_1
   0   (Count of objects to del)
   2   (Count of A/V pair of object 0)
   attribute_0
   value_0
   attribute_1
   value_1
   1   (Count of A/V pair of object 1)
   attribute_0
   value_0
]]
local arg_start = 2
for i = 1, ARGV[arg_start] do
    redis.call('SADD', KEYS[2], ARGV[arg_start + i])
end
arg_start = arg_start + ARGV[arg_start] + 1
for i = 1, ARGV[arg_start] do
    redis.call('SADD', KEYS[3], ARGV[arg_start + i])
end
arg_start = arg_start + ARGV[arg_start] + 1
for j = 4, #KEYS do
    for i = 1, ARGV[arg_start] do
        redis.call('HSET', KEYS[j], ARGV[arg_start + i * 2 - 1], ARGV[arg_start + i * 2])
    end
    arg_start = arg_start + 2 * ARGV[arg_start] + 1
end
redis.call('PUBLISH', KEYS[1], ARGV[1])
```
#### apply view example
```cpp
    const std::string dbname = "APPL_DB";
    const std::string table_name = "PSEUDOTABLE";
    swss::DBConnector context(dbname, 0, true);

    swss::Table t(&context, table_name);
    swss::ProducerStateTable pst(&context, table_name);

    vkco = {
        {"ENTRY0", "", {{"key0", "value0"}, {"key1", "value1"}, {"key2", "value2"}}},
        {"ENTRY1", "", {{"key0", "value0"}, {"key1", "value1"}}},
        {"ENTRY2", "", {{"key0", "value0"}, {"key1", "value1"}}},
    };
    for (auto &kco : vkco)
    {
        t.set(kfvKey(kco), kfvFieldsValues(kco));
    }

    pst.create_temp_view();
    vkco = {
        {"ENTRY0", "", {{"key0", "value0"}, {"key1", "value11"}, {"key3", "value3"}}},
        {"ENTRY3", "", {{"key0", "value0"}, {"key1", "value1"}}},
    };
    pst.set(vkco);
    pst.apply_temp_view();
```
```
1665230361.912064 [0 127.0.0.1:47872] "EVALSHA" "d2494885834676988130076bc645ab8fdee3a1ec" "5" "PSEUDOTABLE_CHANNEL@0" "PSEUDOTABLE_KEY_SET" "PSEUDOTABLE_DEL_SET" "_PSEUDOTABLE:ENTRY0" "_PSEUDOTABLE:ENTRY3" "G" "4" "ENTRY0" "ENTRY1" "ENTRY2" "ENTRY3" "3" "ENTRY0" "ENTRY1" "ENTRY2" "3" "key0" "value0" "key1" "value11" "key3" "value3" "2" "key0" "value0" "key1" "value1"
1665230361.912096 [0 lua] "SADD" "PSEUDOTABLE_KEY_SET" "ENTRY0"
1665230361.912113 [0 lua] "SADD" "PSEUDOTABLE_KEY_SET" "ENTRY1"
1665230361.912124 [0 lua] "SADD" "PSEUDOTABLE_KEY_SET" "ENTRY2"
1665230361.912136 [0 lua] "SADD" "PSEUDOTABLE_KEY_SET" "ENTRY3"
1665230361.912146 [0 lua] "SADD" "PSEUDOTABLE_DEL_SET" "ENTRY0"
1665230361.912156 [0 lua] "SADD" "PSEUDOTABLE_DEL_SET" "ENTRY1"
1665230361.912165 [0 lua] "SADD" "PSEUDOTABLE_DEL_SET" "ENTRY2"
1665230361.912175 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY0" "key0" "value0"
1665230361.912186 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY0" "key1" "value11"
1665230361.912196 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY0" "key3" "value3"
1665230361.912207 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY3" "key0" "value0"
1665230361.912219 [0 lua] "HSET" "_PSEUDOTABLE:ENTRY3" "key1" "value1"
1665230361.912229 [0 lua] "PUBLISH" "PSEUDOTABLE_CHANNEL@0" "G"
```

## ConsumerStateTable
* `ConsumerStateTable`(`ConsumerTableBase`, `TableName_KeySet`):
    * `m_shaPop`
    * `pops(vkco)`

### constructor
```cpp
    std::string luaScript = loadLuaScript("consumer_state_table_pops.lua");
    m_shaPop = loadRedisScript(db, luaScript);

    for (;;)
    {
        RedisReply watch(m_db, "WATCH " + getKeySetName(), REDIS_REPLY_STATUS);
        watch.checkStatusOK();
        multi();
        enqueue(std::string("SCARD ") + getKeySetName(), REDIS_REPLY_INTEGER);
        subscribe(m_db, getChannelName(m_db->getDbId()));
        bool succ = exec();
        if (succ) break;
    }

    RedisReply r(dequeueReply());
    setQueueLength(r.getReply<long long int>());                                    // [1]
```
* in the constructor of `ConsumerStateTable`, the key part is in the Redis transaction:
    1. get the size of the set that stores the table entry keys
    2. subscribe to the channel owned by the table used by `ProducerStateTable` to notify the key change event
* from `[1]`, it set current queue length as the size of the set that stores the table entry keys
    * because the set stores the table entry keys that already have had key change events

### pops
```lua
redis.replicate_commands()
local ret = {}
local tablename = KEYS[2]
local stateprefix = ARGV[2]
local keys = redis.call('SPOP', KEYS[1], ARGV[1])
local n = table.getn(keys)
for i = 1, n do
   local key = keys[i]
   -- Check if there was request to delete the key, clear it in table first
   local num = redis.call('SREM', KEYS[3], key)
   if num == 1 then
      redis.call('DEL', tablename..key)
   end
   -- Push the new set of field/value for this key in table
   local fieldvalues = redis.call('HGETALL', stateprefix..tablename..key)
   table.insert(ret, {key, fieldvalues})
   for i = 1, #fieldvalues, 2 do
      redis.call('HSET', tablename..key, fieldvalues[i], fieldvalues[i + 1])
   end
   -- Clean up the key in temporary state table
   redis.call('DEL', stateprefix..tablename..key)
end
return ret
```
* parameters:
    * `KEYS[1]`: the set that stores table entry keys
    * `KEYS[2]`: the table prefix(table name + table separator)
    * `KEYS[3]`: the set that stores the del table entry keys
* for any table entry key stored in the set that stores table entry keys:
    * if it is also stored in the set that stores del table entry keys, its formal table entry will be removed first
    * move the content from the temporary table entry to the formal table entry(remove the prefix in this case)
    * return the temporary table entry

## summary
* the set that stores the table entry keys: `tableName + "_KEY_SET"`
    * entry keys being included in this set means that they had key changes
    * the field/value pairs stored in the temporary table key will be moved to the formal table by `ConsumerStateTable::pops`
    * `ConsumerStateTable::pops` only returns the field/value pairs stored in the temporary table key instead all field/value pairs in the table key
* the set that stores the del table entry keys: `tableName + "_DEL_SET"`
    * entry keys being included in this set means that they had key del operations
    * its formal table key will be removed by `ConsumerStateTable::pops`
