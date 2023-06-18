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

* class `RedisCLient`:

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
	* `m_guid`
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
    * `m_meta`
    * `m_redisSai`
    * `m_notificationCallback`

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

* class `RedisChannel`(`Channel`):
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

## SAI interface
* class `SaiInterface`: class to define the `SAI` APIs to do `CRUD` operations
    * `initialize()`
    * `uninitialize()`
    * `create()`
    * `remove()`
    * `set()`
    * `get()`

* class `RemoteSaiInterface`(`SaiInterface`):

* Gaps:
    * what is SAI meta?
    * what is vid?

* class `RedisRemoteSaiInterface`(`RemoteSaiInterface`):
    * `m_contextConfig`
    * `m_redisCommunicationMode`
    * `m_recorder`
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
    * `m_contextMap`
		* **NOTE**: the `SAI` API calls are dispatched to the corresponding context by macro `REDIS_CHECK_CONTEXT`
    * `m_service_method_table`
    * `m_recorder`
    * `initialize()`: setup the `Context` based on the context config
        * **QUESTION?**: what is the context?
	* `create()`: `context->m_meta->create()`
	* `remove()`: `context->m_meta->remove()`
 	* `set()`: `context->m_meta->set()`
  	* `get()`: `context->m_meta->get()`

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


* class `ServerSai`(`SaiInterface`):
    * `m_apiInitialized`: bool if the `SAI` API is initialized
    * `m_runServerThread`: bool if `m_serverThread` is started
    * `m_apimutex`
    * `m_service_method_table`
    * `m_sai`: `SAI` object, the actual `SAI` obj to talk to `syncd`
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
