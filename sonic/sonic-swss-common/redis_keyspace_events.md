# redis keyspace events
* Keyspace events allow clients to subcribe to pub/sub channels to receive events affecting Redis data set:
    * all the commands affecting a given key
    * all the keys receiving an LPUSH operation
    * all the keys expiring in the database 0

## event types
* type of events:
    * keyspace notification: targeting a key
    * keyevent notification: targeting an operation

* `set` and `del` a key named `mykey`:
```
lolv@195ce68912ea:~$ redis-cli -n 0
127.0.0.1:6379> set mykey myvablue
OK
127.0.0.1:6379> set mykey myvalue
OK
127.0.0.1:6379> del mykey
(integer) 1
127.0.0.1:6379> set mykey myvalue
OK
127.0.0.1:6379> del mykey
(integer) 1
```
* the keyspace notification and keyevent notification:
```
lolv@195ce68912ea:~$ redis-cli
127.0.0.1:6379> SUBSCRIBE __keyspace@0__:mykey
Reading messages... (press Ctrl-C to quit)
1) "subscribe"
2) "__keyspace@0__:mykey"
3) (integer) 1
1) "message"
2) "__keyspace@0__:mykey"
3) "set"
1) "message"
2) "__keyspace@0__:mykey"
3) "del"
^C
lolv@195ce68912ea:~$ redis-cli
127.0.0.1:6379> SUBSCRIBE __keyevent@0__:del
Reading messages... (press Ctrl-C to quit)
1) "subscribe"
2) "__keyevent@0__:del"
3) (integer) 1
1) "message"
2) "__keyevent@0__:del"
3) "mykey"
```

## config
```
127.0.0.1:6379> config get notify-keyspace-events
1) "notify-keyspace-events"
2) "AKE"
```
* `AKE` means enable all events except for key miss and new key events

## references
* https://redis.io/docs/manual/keyspace-notifications/
