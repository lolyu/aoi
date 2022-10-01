# hi hiredis
## basic synchronous API
```
redisContext *redisConnect(const char *ip, int port);
void *redisCommand(redisContext *c, const char *format, ...);
void freeReplyObject(void *reply);
```
* `redisContext` stores the connection state to redis server
* `redisReply` stores the reply for a Redis command

## setup connection
```cpp
redisContext *c = redisConnect("127.0.0.1", 6379);
if (c == NULL || c->err) {
    if (c) {
        printf("Error: %s\n", c->errstr);
        // handle error
    } else {
        printf("Can't allocate redis context\n");
    }
}
```
* disconnect and free the context
```cpp
void redisFree(redisContext *c);
```

## sending command
```cpp
void *redisCommand(redisContext *c, const char *format, ...);
void *redisCommandArgv(redisContext *c, int argc, const char **argv, const size_t *argvlen);
```
## reply parsing
```cpp
/* This is the reply object returned by redisCommand() */
typedef struct redisReply {
    int type; /* REDIS_REPLY_* */
    long long integer; /* The integer when type is REDIS_REPLY_INTEGER */
    size_t len; /* Length of string */
    char *str; /* Used for both REDIS_REPLY_STATUS, REDIS_REPLY_ERROR and REDIS_REPLY_STRING */
    size_t elements; /* number of elements, for REDIS_REPLY_ARRAY */
    struct redisReply **element; /* elements vector for REDIS_REPLY_ARRAY */
} redisReply;
```
* reply types:
    * `REDIS_REPLY_STATUS`
    * `REDIS_REPLY_ERROR`
    * `REDIS_REPLY_INTEGER`
    * `REDIS_REPLY_NIL`
    * `REDIS_REPLY_STRING`
    * `REDIS_REPLY_ARRAY`

* **NOTE**: replies should be freed using the `freeReplyObject()` function.
    * `freeReplyObject()` also take care of freeing sub-reply objects contained in arrays, users don't need to free the sub replies.
    * 

## references
* 
