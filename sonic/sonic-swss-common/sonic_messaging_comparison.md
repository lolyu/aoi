# sonic messaging comparison

|method|summary|
|-|-|
|`SubscriberStateTable`|<br>1. use Redis keyspace event<br>2. only Redis `DEL` operation is regarded as `del` op, others are regarded as `set` op|
|`ProducerStateTable` && `ConsumerStateTable`|<br>1. use a specific `Redis` pub/sub channel to notify key changes<br>2. use two Redis `SET`s to store `set`/`del` keys<br>3. the key updates are stored in a temporary table waiting to be consumed<br>4. if temporary table key is not existed, consumer will regard this as `del` op|
|`ProducerTable` && `ConsumerTable`||
|`NotitifcationProducer` && `NotificationConsumer`||
