# sai and syncd

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
    * `m_handler`
    * `m_sn`

* `Slot`

* `SwitchNotifications`:
    * `m_slot`
