# NotificationProducer && NotificationConsumer

## NotificationProducer
* `NotificationProducer`:
    * `m_db`: `DbConnector` object
    * `m_channel`: the to-publish channel
    * `send(op, data, values)`: send the jsonized (op, data, values) through `m_channel`



## NotificationConsumer
* `NotificationConsumer`(`Selectable`):
    * `m_db`: the `DbConnector` passed from constructor
    * `m_subscribe`: the `DbConnector` owned by itself specific for subscription
    * `m_channel`: the subscribed channel
    * `m_queue`: queue to store the jsonized message received from the subscribed channel
    * `readData()`: read from the subscribed channel, parse the reply and push the message to `m_queue`
    * `processReply(reply)`: parse the reply, check reply type and push the message to `m_queue`
    * `pop(op, data, values)`: pop out a message from `m_queue` and parse its content into `(op, data, values)`
    * `pops(vkco)`


### pops
* `pops` will keep peek the subscribed Redis socket to see if there are any more messages to come
```cpp
void swss::NotificationConsumer::pops(std::deque<KeyOpFieldsValuesTuple> &vkco)
{
    SWSS_LOG_ENTER();

    vkco.clear();
    while(!m_queue.empty())
    {
        while(!m_queue.empty())
        {
            std::string op;
            std::string data;
            std::vector<FieldValueTuple> values;

            pop(op, data, values);
            vkco.emplace_back(data, op, values);
        }
        // Too many popped, let's return to prevent DoS attach
        if (vkco.size() >= POP_BATCH_SIZE)
            return;
        // Peek for more data in redis socket
        int rc = swss::peekRedisContext(m_subscribe->getContext());
        if (rc <= 0)
            break;

        readData();
    }
}
```

## summary
* `NotificationProducer` and `NotificationConsumer` communicates through a specific channel, and there is no modification to the db.
