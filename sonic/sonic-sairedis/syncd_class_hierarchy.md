# syncd class notes

* class `NotificationProducer`: send messages through redis channel
    * `m_db`: `DBConnector` obj
    * `m_channel`: channel to publish message to
    * `send()`: send the message to the channel

* class `NotificationConsumer`(`Selectable`): read messages from redis channel
    * m_db: DBConnector object to store the original db connector
	* m_subscribe: DBConnector object  to store the subscription db connector
	* m_channel: the channel subscribed to
	* m_queue: notification message queue to store the messages in the subscribed channel
	* processReply: parse the reply from the subscribed channel, get the message, push to m_queue
	* readData: read messages in the subscribed channel so far
	* peek: poll the fd of m_subscribe to see if there are more data to read
	* pop: pop one message from m_queue
    * pops: pops up to POP_BATCH_SIZE


* class `RedisNotificationProducerBase`: base class to define producer APIs'
    * `send()`
* class `RedisNotificationProducer`(`NotificationProducer`): proxy class to `NotificationProducer`
    * `m_db`
    * `m_notificationProducer`
    * `send()`

* class `RedisClient`:

* class `NotificationQueue`: class to act as notification queue
    * `enqueue()`
    * `tryDequeue()`

* class `NotificationProcessor`:
    * `m_notifications`
    * `m_notificationQueue`
    * `m_ntf_process_thread`
    * `m_synchronizer`
    * `sendNotification`
        * call `m_notifications->send()` to send notification to OA
    * `ntf_process_function`
        * dequeue notifications from the `m_notificationQueue` and call `m_synchronizer` to process
        * `syncProcessNotification` dispatch the notifications to its corresponding handlers
        * the handlers process the notifications and send valid notifications(`sendNotification`)

* class `NotificationHandler`:
    * `m_switchNotifications`
    * `m_notificationQueue`: the notification queue(`m_processor.m_notificationQueue`)
    * `m_processor`: a `NotificationProcessor` object


* class `SlotBase`:
    * `m_handler`: a `SwitchNotifications` object
    * `m_sn`: a `sai_switch_notifications_t` struct
    * `m_slots.at(context)->m_handler->onFdbEvent(count,data)`

* class `Slot`

* class `SwitchNotifications`:
    * `m_slots`
    * `m_slot`
    * `getSwitchNotifications()`

* class `SelectableEvent`(`Selectable`): a selectable event that could be selected, it is implemented based on `eventfd`
    * `m_efd`: the `eventfd` object
    * `getFd()`: return the `eventfd` object
    * `readData()`: read from `m_efd`
    * `notify()`: write to `m_efd` to make it readable

* class `SelectableChannel`(`SelectableChannel`): a selectable based on `ZMQ_REP` `zeroMQ` socket with endpoint: `"ipc:///tmp/saiServer"`, the messages received are jsonized strings
    * `m_endpoint`: `zeroMQ` endpoint string
    * `m_context`: `zeroMQ` context`
    * `m_socket`: `zeroMQ` socket
    * `m_fd`: `zeroMQ` socket fd
    * `m_queue`: queue to store requests received from `zeroMQ` socket
    * `m_buffer`: `zeroMQ` receive buffer
    * `m_allowZmqPoll`: use to notify `zmqPollThread` to do next poll, this is to ensure the `receive/send` pattern is followed, in this case, for the request received from the `zeroMQ` socket, the response is sent back before receiving next request from the `zeroMQ` socket.
    * `m_runThread`
    * `m_zmlPollThread`: thread to run `zmqPollThread()`
    * `m_selectableEvent`
    * `set()`: send replies to the `zeroMQ` socket, finish the receive/send` pattern, and notify `zmqPollThread` to poll next request by setting `m_allowZmqPoll` to true.
    * `getFd()`: return `m_selectableEvent.getFd()`
    * `zmqPollThread()`: keeps polling the `zeroMQ` socket and store the requests into `m_queue`, and call `m_selectableEvent.notify()` to notify the events to the select event loop.
    * `pop`: pop out one request from `m_queue`, json-loads the request string, and returns `KeyOpFieldsValuesTuple`

* class `SwitchConfig`
	* `m_switchIndex`
		* default `0`
	* `m_hardwareInfo`
		* default `""`

```
Jun 17 15:45:32.611800 lab-dev-acs-01 NOTICE swss#orchagent: :- insert: added switch: idx 0, hwinfo ''
``` 


* class `SwitchConfigContainer`:
	* `m_indexToConfig`: mapping from switch index to `SwitchConfig` object
	* `m_hwinfoToConfig`: mapping from hardware info to `SwitchConfig` object

* class `ContextConfig`: **Question?**: what is `ContextConfig`? Is it per virtual switch?
	* `m_guid`: the switch id
    * `m_name`
    * `m_dbAsic`
    * `m_dbCounters`
    * `m_dbFlex`
    * `m_dbState`
    * `m_zmqEnable`
        * default `false` 
    * `m_zmqEndpoint`
        * default `"ipc:///tmp/zmq_ep"`
    * `m_zmqNtfEndpoint`
        * default `"ipc:///tmp/zmq_ntf_ep"`
    * `m_scc`: a `SwitchConfigContainer`

* class `ContextConfigContainer`: stores the mapping from context to `ContextConfig`
	* `m_map`
	* `insert()`
 	* `get()`
	* `getAllContextConfigs()`
	* `loadFromFile()`
	* `getDefault()`
		* return one `ContextConfigContainer` obj with one `ContextConfig` that contains a default `SwitchConfig`

* class `Context`:
    * `m_meta`: a `Meta` object
    * `m_redisSai`: a `RedisRemoteSaiInterface` object
    * `m_notificationCallback`: notification callback

* class `Channel`: virtual class to represent a channel to `syncd`
    * `m_callback`
    * `m_responseTimeoutMs`
    * `m_runNotificationThread`
    * `m_notificationThreadShouldEndEvent`
    * `m_notificationThread`
    * `notificationThreadFunction()`
    * `setbuffered()`
    * `flush()`
    * `set()`
    * `del()`
    * `wait()`

* class `RedisChannel`(`Channel`): the class to interact syncd via `ASIC_DB`, it could send requests/receive responses from `syncd`, and it starts a thread listening for any notifications from `syncd`.
    * `m_dbAsic`: `"ASIC_DB"`
    * `m_asicState`: `ProducerTable` used to send commands(create/remove/set/get) to syncd
    * `m_getConsumer`: `ConsumerTable` used to receive responses from `syncd`
    * `m_db`: db connector
    * `m_redisPipeline`: redis pipeline
    * `m_dbNtf`: db connector used for notifications
    * `m_notificationConsumer`: `NotificationConsumer` to read notifications from `syncd`
    * `notificationThreadFunction()`: the function that `Channel::m_notificationThread` runs, keeps receiving notifications from `syncd`, and calls the `m_callback` to process it
    * `set()`
    * `del()`
    * `wait()`: wait for the specified response from `syncd`
		* for `SAI` operations, first use `m_asicState` to write to `ASIC_DB` to notify the `syncd` to process the `SAI` operations, then use `m_getConsumer` to wait for responses.

* class `Recorder`: log `Sai` API calls to `sairedis.rec`
    * `Recorder use letters to represent API call operations, lower letters for API calls, upper letters for API calls' responses:
        *  flush fdb: `f`, `F`
        *  query attribute/attribute enum capability or get/clear stats: `q`, `Q`
        *  notify syncd: `a`, `A`
        *  create: `c`, `C`
        *  remove: `r`, `R`
        *  set: `s`, `S`
        *  get: `g`, `G`
        *  notification: `n`
        *  generic response: `E`

* class `VirtualObjectIdManager`: construct or parse a `SAI` object OID(`sai_object_id_t`)
	* **NOTE**: a `sai_object_id_t`(64bits) is consisted of four parts: `<switch index(8bits)><context index(8bits)><object type(8bits)><object index(40bits)>`

* class `Notification`: base class to represent a notification

* class `NotificationFdbEvent`(`Notification`): class to represent a `FDB` event notification

* class `Switch`: class to represent a virtual switch
	* `m_switchId`: `SAI` switch object `oid`
 	* `m_switchNotifications`: struct `sai_switch_notifications_t` to contains all notification handler pointers
	* `m_hardwareInfo`: `SAI` switch hardware info
 	* `updateNotifications(attrCount, *attrList)`: update `m_switchNotifications` struct
  		* when the switch is created, the notification handler pointers are passed as the object attributes

## SAI interface
* class `SaiInterface`: class to define the `SAI` APIs to do `CRUD` operations
    * `initialize()`
    * `uninitialize()`
    * `create()`
    * `remove()`
    * `set()`
    * `get()`

* class `RemoteSaiInterface`(`SaiInterface`):

* class `Meta`(`SaiInterface`): provide API calls parameter validation
	* `m_implementation`: a `RedisRemoteSaiInterface`
	* `m_portRelatedSet`: mapping from port `oid` to its related objects' `oid`s.
	* `m_oids`: mapping from object `oid` to its reference counter
	* `m_saiObjectCollection`:
	* `m_attrKeys`

* Gaps:
    * what is SAI meta?
    * what is vid?

* class `RedisRemoteSaiInterface`(`RemoteSaiInterface`):
    * `m_contextConfig`
    * `m_redisCommunicationMode`
    * `m_recorder`
    * `m_switchContainer`: contains all the virtual switch created via this `SaiInterface`
    * `m_notificationCallback`
    * `initialize()`
        * initialize the communication channel
            * `ZeroMQChannel`
            * `RedisChannel`(default)
    * `create()`
    * `remove()`
    * `set()`
    * `get()`

* class `ClientServerSai`(`SaiInterface`): proxy class to delegate `SAI` API calls to the `SaiInterface` object(stored in `m_sai`)
    * `m_apiInitialized`
    * `m_apimutex`
    * `m_service_method_table`: the config retrievers
    * `m_sai`: a `SaiInterface` object, either a `ServerSai` or `ClientSai`
    * `initialize()`
    * `uninitialize()`
    * `create()`: `m_sai->create()`
    * `remove()`: `m_sai->remove()`
    * `set()`: `m_sai->set()`
    * `get()`: `m_sai->get()`


* class `Sai`(`SaiInterface`): the actual `SAI` object to interact with `ASIC_DB` to talk to `syncd`
    * `m_apiInitialized`
    * `m_apimutex`
    * `m_contextMap`: mapping from switch oid to its `Context` object.
		* **NOTE**: the `SAI` API calls are dispatched to the corresponding context by macro `REDIS_CHECK_CONTEXT`
    * `m_service_method_table`
    * `m_recorder`
    * `initialize()`: setup the `Context` based on the context config
        * **QUESTION?**: what is the context?
	* `create()`: `context->m_meta->create()`
	* `remove()`: `context->m_meta->remove()`
 	* `set()`: `context->m_meta->set()`
  	* `get()`: `context->m_meta->get()`

* the context check macro
```cpp
#define REDIS_CHECK_CONTEXT(oid)                                            \
    auto _globalContext = VirtualObjectIdManager::getGlobalContext(oid);    \
    auto context = getContext(_globalContext);                              \
    if (context == nullptr) {                                               \
        SWSS_LOG_ERROR("no context at index %u for oid %s",                 \
                _globalContext,                                             \
                sai_serialize_object_id(oid).c_str());                      \
        return SAI_STATUS_FAILURE; }
```

* notifications captured in the `ASIC_DB`
```
127.0.0.1:6379> subscribe NOTIFICATIONS
Reading messages... (press Ctrl-C to quit)
1) "subscribe"
2) "NOTIFICATIONS"
3) (integer) 1
1) "message"
2) "NOTIFICATIONS"
3) "[\"fdb_event\",\"[{\\\"fdb_entry\\\":\\\"{\\\\\\\"bvid\\\\\\\":\\\\\\\"oid:0x2600000000063d\\\\\\\",\\\\\\\"mac\\\\\\\":\\\\\\\"00:06:07:08:09:0A\\\\\\\",\\\\\\\"switch_id\\\\\\\":\\\\\\\"oid:0x21000000000000\\\\\\\"}\\\",\\\"fdb_event\\\":\\\"SAI_FDB_EVENT_LEARNED\\\",\\\"list\\\":[{\\\"id\\\":\\\"SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID\\\",\\\"value\\\":\\\"oid:0x3a000000000652\\\"}]}]\"]"
```
* How does the notification get processed?
	* thread `RedisChannel::notificationThreadFunction()` ->
	* `RedisRemoteSaiInterface::handleNotification()` ->
		1. convert the serialized notification to its corresponding `Notification` derivative classes.
    	2. get the registered notification handler pointers
			* `Context::handle_notification()` ->
			* `Sai::handle_notification()` ->
			* `RedisRemoteSaiInterface::syncProcessNotification()`
		3. `notification->executeCallback(sn)`

* How does OA register the notification handlers?


* class `ServerSai`(`SaiInterface`):
    * `m_apiInitialized`: bool if the `SAI` API is initialized
    * `m_runServerThread`: bool if `m_serverThread` is started
    * `m_apimutex`
    * `m_service_method_table`
    * `m_sai`: `Sai` object, the actual `SAI` interface obj to talk to `syncd`
    * `m_serverThread`: thread running `serverThreadFunction`
    * `m_selectableChannel`: a `ZeroMQSelectableChannel` object
    * `m_serverThreadThreadShouldEndEvent`: event to notify `serverThreadFunction` to exit
    * `serverThreadFunction`: keeps polling the `m_selectableChannel` to read any request from the `ZeroMQ` client, and call `processEvent` to process
    * `initialize()`
  		* `m_sai->initialize()`
  		* start `m_serverThread` to keep handling requests from  the `ZeroMQ` channel
    * `processEvent()`: pop the event and call `processSingleEvent()`
    * `processSingleEvent()`:
        * dispatch the events to the corresponding handlers based on the redis `ASIC` state command type(`REDIS_ASIC_STATE_COMMAND_*`)
        * create(`REDIS_ASIC_STATE_COMMAND_CREATE`) -> `processQuadEvent(SAI_COMMON_API_CREATE, kco)` -> `processEntry`
        * remove(`REDIS_ASIC_STATE_COMMAND_REMOVE`) -> `processQuadEvent(SAI_COMMON_API_REMOVE, kco)` -> `processEntry`
        * set(`REDIS_ASIC_STATE_COMMAND_SET`) -> `processQuadEvent(SAI_COMMON_API_SET, kco)` -> `processEntry`
        * get(`REDIS_ASIC_STATE_COMMAND_GET`) -> `processQuadEvent(SAI_COMMON_API_GET, kco)` -> `processEntry`
    * `processEntry()`
        * `SAI_COMMON_API_CREATE` -> `m_sai->create()`
        * `SAI_COMMON_API_REMOVE` -> `m_sai->remove()`
        * `SAI_COMMON_API_SET` -> `m_sai->set()`
        * `SAI_COMMON_API_GET` -> `m_sai->get()`
    * `create()`: `m_sai->create()`
    * `remove()`: `m_sai->remove()`
    * `set()`: `m_sai->set()`
    * `get()`: `m_sai->get()`

* class `ClientSai`(`SaiInterface`):

## references
* https://man7.org/linux/man-pages/man2/eventfd.2.html
