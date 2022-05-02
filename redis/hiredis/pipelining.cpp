#include <hiredis/hiredis.h>

#include <iostream>

/**
when `redisCommand` is called, hiredis firs formats the command base on the Redis protocol, the formatted command is put in the output buffer.
when `redisGetReply` is called:
    1. if the input buffer is non-empty
        * try parse a signle reply from the input buffer and return it
        * if no reply could be parsed, continue at 2
    2. the input buffer is empty
        * write the entire output buffer to the socket
        * read from the socket until a single reply could be parsed

How to enable pipelining?
* Use those two functions to fill up the output buffer
```cpp
void redisAppendCommand(redisContext *c, const char *format, ...);
void redisAppendCommandArgv(redisContext *c, int argc, const char **argv, const size_t *argvlen);
```
* Call `redisGetReply` to retrieve the reply
 */

int main()
{
    redisContext *context = NULL;
    redisReply *reply = NULL;

    context = redisConnect("127.0.0.1", 6379);
    if (!context || context->err) {
        if (context) {
            std::cout << "Error: " << context->errstr << std::endl;
        } else {
            std::cout << "Can't allocate Redis context!" << std::endl;
        }
    }

    redisAppendCommand(context, "DEL %s", "EMPLOYEE|ALICE");
    redisAppendCommand(context, "HSET %s %s %s", "EMPLOYEE|ALICE", "NAME", "ALICE");
    redisAppendCommand(context, "HSET %s %s %s", "EMPLOYEE|ALICE", "AGE", "188");

    do {
        redisGetReply(context, reinterpret_cast<void **>(&reply));
        std::cout << reply->type << std::endl;
        std::cout << reply->integer << std::endl;
    } while (reply && !context->err);
}