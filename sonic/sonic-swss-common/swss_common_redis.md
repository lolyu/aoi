# redis related classes in swss-common
* `SonicDBConfig`: stores the per network namespace redis socket mapping, db mapping, db separator mapping 
* `RedisCommand`: format redis command.
* `RedisContext`: a wrapper class for hiredis `redisContext` structure.
* `RedisReply`: a wapper class for hiredis `redisReply` structure, used to send command to `RedisContext` and receive && check reply.
* `DBConnector`: child class of `RedosContext`, provide SONiC db connection support and interfaces for common Redis ops.
* `RedisTransactioner`: provide a wrapper over Redis transaction(`multi/exec`)

## a working demo
