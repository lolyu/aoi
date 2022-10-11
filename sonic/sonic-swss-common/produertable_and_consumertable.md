# ProducerTable and ConsumerTable

## ProducerTable
* `ProducerTable`(`TableBase`, `TableName_KeyValueOpQueues`)
    * `m_dumpFile`
    * `m_firstItem`
    * `m_buffered`
    * `m_pipeowned`
    * `m_pipe`
    * `m_shaEnqueue`: `Lua` script to push key changes to a Redis list
    * `enqueueDbChange(key, value, op, prefix)`
    * `set(key, values, op, prefix)`
    * `del(key, op, prefix)`

### enqueueDbChange
```lua
redis.call('LPUSH', KEYS[1], ARGV[1], ARGV[2], ARGV[3])
redis.call('PUBLISH', KEYS[2], ARGV[4])
```
* parameters:
    * `KEYS[1]` : `tableName` + `"_KEY_VALUE_OP_QUEUE`
    * `ARGV[1]` : key
    * `ARGV[2]` : value
    * `ARGV[3]` : op
    * `KEYS[2]` : `tableName` + `"_CHANNEL"`
    * `ARGV[4]` : `"G"`

### set
```cpp
enqueueDbChange(key, JSon::buildJson(values), "S" + op, prefix);
```
* the enqueue sequence is `key`, jsonized `values`, `op`
### del
```cpp
enqueueDbChange(key, "{}", "D" + op, prefix);
```

## ConsumerTable

* `ConsumerTable`(`ConsumerTableBase`, `TableName_KeyValueOpQueues`):
    * `m_modifyRedis`
    * `m_shaPop`
    * `pops(vkco, prefix)`
    * `setModifyRedis(modify)`

### pops
```lua
local rets = {}
-- pop Key, Value and OP together.
local popsize = ARGV[1] * 3
local keys   = redis.call('LRANGE', KEYS[1], -popsize, -1)

redis.call('LTRIM', KEYS[1], 0, -popsize-1)

local n = table.getn(keys)
for i = n, 1, -3 do
   local op = keys[i-2]
   local value = keys[i-1]
   local key = keys[i]
   local dbop = op:sub(1,1)
   op = op:sub(2)
   local ret = {key, op}

   local jj = cjson.decode(value)
   local size = #jj

   for idx=1,size,2 do
       table.insert(ret, jj[idx])
       table.insert(ret, jj[idx+1])
   end
   table.insert(rets, ret)

   if ARGV[2] == "0" then
       -- do nothing, we don't want to modify redis during pop
   elseif op == 'bulkset' or op == 'bulkcreate' or op == 'bulkremove' then
       -- omitted
   elseif
       op == 'set' or
       op == 'SET' or
       op == 'create' or
       op == 'remove' or
       op == 'DEL' then
   -- put entries into REDIS hash only when operations are this types
   -- in case of delete command, remove entries
       local keyname = KEYS[2] .. ':' .. key
       if key == '' then
           keyname = KEYS[2]
       end

       if dbop == 'D' then
           redis.call('DEL', keyname)
       else
           local st = 3
           local len = #ret
           while st <= len do
               redis.call('HSET', keyname, ret[st], ret[st+1])
               st = st + 2
           end
       end
   else
    -- notify redis that this command is not supported and require handling
       error("unsupported operation command: " .. op .. ", FIXME")
   end
end

return rets
```

* parameters:
    * `KEYS[1]` : `tableName` + `"_KEY_VALUE_OP_QUEUE`
    * `KEYS[2]` : `prefix` + `tableName`
    * `ARGV[1]` : pop batch size
    * `ARGV[2]` : `m_modifyRedis ? 1 : 0`


## summary
* `ProducerTable` uses a queue(Redis list) to store the operation key, op and value pairs, and `ConsumerTable` pops those changes from the queue and modify the table accordingly.
* `ConsumerTable::pops` could restore the operation as the same sequence as what `ProducerTable` does.



