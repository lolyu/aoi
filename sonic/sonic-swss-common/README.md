# swss-common notes

* the select architecture:
    * notes:
        * [swss select architecture](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/swss_common_select.md)
    * codes:
        * [an simple echo tcp server demo with `Select`/`Selectable`](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/codes/tcp_echo_server_with_selectable_demo.cpp)

* Redis related:
    * notes:
        * [hiredis basics](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/hi_hiredis.md)
        * [hiredis pipelining](https://programmer.group/hiredis-realizes-redis-pipeline.html)
        * [more about hiredis pipelining](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/hiredis_pipelining.md)
        * [lua scripting in redis](https://github.com/lolyu/aoi/tree/master/sonic/sonic-swss-common)
        * [redis keyspace event](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/redis_keyspace_events.md)
        * [redis transaction(MULTI/EXEC/WATCH)](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/redis_transaction.md)
    * codes:
        * [hiredis code demo](https://github.com/lolyu/aoi/blob/master/redis/hiredis/redis_basics.cpp)
        * [`RedisCommand`/`RedisReply` demo](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/codes/format_redis_command.cpp)
        * [`DbConnector` demo](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/codes/dbconnector_demo.cpp)

* db table:
    * notes:
        * [swss redis](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/swss_common_redis.md)
        * [swss `Table`](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/swss_common_table.md)
        * [`SubscriberStateTable`](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/swss_common_subscriberstatetable.md)
        * [`ProducerStateTable` && `ConsumerStateTable`](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/producerstatetable_and_consumerstatetable.md)
        * [`NotificationProducer` && `NotificationConsumer`](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/swss_common_notificationproducer_and_notificationconsumer.md)
    * codes:
        * [`Table` demo](https://github.com/lolyu/aoi/edit/master/sonic/sonic-swss-common/table_demo.cpp)
        * [`SubscriberStateTable` demo](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/codes/subscriberstatetable_demo.cpp)
        * [`ProducerStateTable` demo](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/codes/producerstatetable_demo.cpp)
        * [`ConsumerStateTable` demo](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/codes/consumerstatetable_demo.cpp)
        * [`ProducerTable` demo](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/codes/producertable_demo.cpp)
        * [`NotificationProducer` demo](https://github.com/lolyu/aoi/blob/master/sonic/sonic-swss-common/codes/notificationproducer_demo.cpp)
         
