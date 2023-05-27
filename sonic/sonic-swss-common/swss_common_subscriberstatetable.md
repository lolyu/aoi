# subscriberstatetable

* `SubscriberStateTable`(`ConsumerTableBase`): subscribes to key changes from a specified table
    * `m_buffer`: inherited from `ConsumerTableBase`, stores related table entries that has key updates
    * `m_keyspace`: the keyspace channel to subscribe, of format `"__keyspace@" + dbId + "__:" + tableName + tableSeparator + "*"`
    * `m_keyspace_event_buffer`: stores events from the subscribed keyspace channel(`m_keyspace`)
    * `m_table`: `Table` object
    * `readData()`
        * reads from redis, and stores the events into `m_keyspace_event_buffer`
    * `hasData()`
        * returns `true` if `m_buffer` or `m_keyspace_event_buffer` is not empty
    * `hasCachedData()`
        * returns `bool(m_buffer.size() + m_keyspace_event_buffer.size() > 1)`
    * `initializedWithData()`
        * returns `!m_buffer.empty()`
    * `popEventBuffer()`
        * pops a key event from `m_keyspace_event_buffer`
    * `pops(vkco, prefix)`
        * if `m_buffer` is not empty, pops out all items from `m_buffer` and return them.
        * if `m_buffer` is empty, try to parse key events from `m_keyspace_event_buffer` and parse the key events and read related keys' entries from the table and return them.


## summary
* `SubscriberStateTable` will pops out table entries that has key changes/updates
    * table entry add/del
        * set table entry `N`(`N` > 1) times will trigger `N` keyspace events, so `SubscriberStateTable` could pops `N` table entries
        * del table entry `N`(`N` > 1) times will only trigger one keyspace event, so `SubscriberStateTable` could pops only one table entry 
    * table entry fields modification
* if register a `SubscriberStateTable` obj `sel` with the select architecture to poll table key changes, and there is one key change, the first `select` could return the `sel`, if the user dones't call `sel->pops()` to consume the key changes, the second `select` will not return `sel` even there is cached events in `sel`
    * this is because `hasCachedData()` returns `true` only if `bool(m_buffer.size() + m_keyspace_event_buffer.size() > 1)`
    * so the first `select` will not put `sel` back into the I/O ready queue
    * please refer to the `SubscriberStateTable` [code demo](https://github.com/lolyu/aoi/tree/master/sonic/sonic-swss-common/codes) for more details about this
* **NOTE**: redis command other than `del` is regarded as `SET` command, only `del` is regarded as `DEL` command
    * for example, `set`, `hset`, `hdel` are all regarded as `SET` command
* for key changes over a table, they could be consumed by multiple `SubscriberStateTable` as the updated table entry keys are included in the keyspace event.
