#include <hiredis/hiredis.h>

#include <cstring>
#include <iostream>

#include "stdarg.h"

using namespace std;

void formatCommand(char **command, const char *format, ...)
{
    va_list va;
    va_start(va, format);
    int len = redisvFormatCommand(command, format, va);
    va_end(va);
    if (len == -1) {
        throw bad_alloc();
    } else if (len == -2) {
        throw invalid_argument("formatCommand");
    }
}

int main()
{
    redisContext *context = NULL;
    redisReply *reply = NULL;
    char *command = nullptr;
    formatCommand(&command, "HGETALL %s", "EMPLOYEE|ALICE");
    cout << strlen(command) << endl;
    cout << command << endl;

    context = redisConnect("127.0.0.1", 6379);
    if (!context || context->err) {
        if (context) {
            std::cout << "Error: " << context->errstr << std::endl;
        } else {
            std::cout << "Can't allocate Redis context!" << std::endl;
        }
    }

    int rc = redisAppendFormattedCommand(context, command, strlen(command));
    if (rc != REDIS_OK) {
        throw bad_alloc();
    }

    rc = redisGetReply(context, reinterpret_cast<void **>(&reply));
    if (rc != REDIS_OK) {
        throw runtime_error("Failed to get reply from Redis");
    }

    std::cout << "Reply type: " << reply->type << std::endl;
    for (int i = 0; i < reply->elements; ++i) {
        redisReply *reply_elem = reply->element[i];
        std::cout << reply_elem->str << std::endl;
    }
    std::cout << std::endl;

    freeReplyObject(reply);

    redisFreeCommand(command);

    return 0;
}