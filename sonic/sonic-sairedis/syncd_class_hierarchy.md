# syncd class notes

* class `NotificationProducer`: send message through redis channel
    * `m_db`: `DBConnector` obj
    * `m_channel`: channel to publish message to
    * `send()`: send the message to the channel
* class `RedisNotificationProducerBase`: base class to define producer APIs'
    * `send()`
* class `RedisNotificationProducer`(`NotificationProducer`): proxy class to `NotificationProducer`
    * `m_db`
    * `m_notificationProducer`
    * `send()`

* `RedisCLient`:

* `NotificationQueue`: class to act as notification queue
    * `enqueue()`
    * `tryDequeue()`

* `NotificationProcessor`:
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

* `NotificationHandler`:
    * `m_switchNotifications`
    * `m_notificationQueue`: the notification queue(`m_processor.m_notificationQueue`)
    * `m_processor`: a `NotificationProcessor` object


* `SlotBase`:
    * `m_handler`: a `SwitchNotifications` object
    * `m_sn`: a `sai_switch_notifications_t` struct
    * `m_slots.at(context)->m_handler->onFdbEvent(count,data)`

* `Slot`

* `SwitchNotifications`:
    * `m_slots`
    * `m_slot`
    * `getSwitchNotifications()`

* `SelectableEvent`(`Selectable`): a selectable event that could be selected, it is implemented based on `eventfd`
    * `m_efd`: the `eventfd` object
    * `getFd()`: return the `eventfd` object
    * `readData()`: read from `m_efd`
    * `notify()`: write to `m_efd` to make it readable

* `SelectableChannel`(`SelectableChannel`): a selectable based on `ZMQ_REP` `zeroMQ` socket with endpoint: `"ipc:///tmp/saiServer"`, the messages received are jsonized strings
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
 

## SAI interface
* `SaiInterface`: class to define the `SAI` APIs to do `CRUD` operations
    * `initialize()`
    * `uninitialize()`
    * `create()`
    * `remove()`
    * `set()`
    * `get()`
 
* `ClientServerSai`(`SaiInterface`): proxy class to delegate `SAI` API calls to the `SaiInterface` object(stored in `m_sai`)
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

* `Sai`(`SaiInterface`): the actual `SAI` object to interact with `ASIC_DB` to talk to `syncd`
    * `m_apiInitialized`
    * `m_apimutex`
    * `m_contextMap`
    * `m_service_method_table`
    * `m_recorder`

* `ServerSai`(`SaiInterface`):
    * `m_apiInitialized`: bool if the `SAI` API is initialized
    * `m_runServerThread`: bool if `m_serverThread` is started
    * `m_apimutex`
    * `m_service_method_table`
    * `m_sai`: `SAI` object, the actual `SAI` obj to talk to syncd
    * `m_serverThread`: thread running `serverThreadFunction`
    * `m_selectableChannel`: a `ZeroMQSelectableChannel` object
    * `m_serverThreadThreadShouldEndEvent`: event to notify `serverThreadFunction` to exit
    * `serverThreadFunction`: keeps polling the `m_selectableChannel` to read any request from the `ZeroMQ` client, and call `processEvent` to process
    * `processEvent()`: pop the event and call `processSingleEvent()`
    * `processSingleEvent()`:
        * dispatch the events to the corresponding handlers based on the redis asic state command type(`REDIS_ASIC_STATE_COMMAND_*`)
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

* `ClientSai`(`SaiInterface`):

## references
* https://man7.org/linux/man-pages/man2/eventfd.2.html
