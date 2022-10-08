# redis transaction
* redis transaction are a sequence of redis commands wrapped in `MULTI`/`EXEC` block
    * those redis commands are executed by the redis server atomically.

## basic form
```
127.0.0.1:6379> multi
OK
127.0.0.1:6379> set a 100
QUEUED
127.0.0.1:6379> incr a
QUEUED
127.0.0.1:6379> incr a
QUEUED
127.0.0.1:6379> exec
1) OK
2) (integer) 101
3) (integer) 102
```

## exception inside a transaction
* two types of exceptions:
    * command syntax error
    * command execution error

### command syntax error
* redis could detect such error in the accumulation of commands, and it will refuse to execute the transaction
```
127.0.0.1:6379> multi
OK
127.0.0.1:6379> set a 100
QUEUED
127.0.0.1:6379> incr a b
(error) ERR wrong number of arguments for 'incr' command
127.0.0.1:6379> exec
(error) EXECABORT Transaction discarded because of previous errors.
```

### command execution error
* all the commands will be executed even if some of them fails during execution
* redis doen't support rollback in such case
```
127.0.0.1:6379> multi
OK
127.0.0.1:6379> set a 100
QUEUED
127.0.0.1:6379> incr a
QUEUED
127.0.0.1:6379> lrange a 0 -1
QUEUED
127.0.0.1:6379> incr a
QUEUED
127.0.0.1:6379> exec
1) OK
2) (integer) 101
3) (error) WRONGTYPE Operation against a key holding the wrong kind of value
4) (integer) 102
```

## abort a transaction
* use `DISCARD` to abort a transaction before `EXEC` is called


## `WATCH`
* `WATCH` makes a transaction conditional, we are asking Redis to perform the transaction only if none of the `WATCH`ed keys were modified.
    * modifications:
        * modifications from the clients
        * key expiration or eviction
 

* `WATCH` could be called multiple times, all the `WATCH` calls will have the effects to watch for changes starting from the call, up to the moment `EXEC` is called.
* if you `WATCH` a volatile key and Redis expires the key after you `WATCH`ed it, `EXEC` will still work.
* when `EXEC` is called, all keys are `UNWATCH`ed.
* when a client connection is closed, everything gets `UNWATCH`ed

```
WATCH zset
element = ZRANGE zset 0 0
MULTI
ZREM zset element
EXEC
```
