# ProducerStateTable and ConsumerStateTable

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

### set operation
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
    1. add the table entry key to the set
    2. store its field/value pairs into a temporary table
    3. publish this key change to the channel if the table entry key is new to the set in step#1
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

## ConsumerStateTable
* `ConsumerStateTable`():
