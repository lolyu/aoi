#include <hiredis/hiredis.h>

#include <cstring>
#include <iostream>

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

    reply = static_cast<redisReply *>(redisCommand(context, "DEL %s", "EMPLOYEE|ALICE"));
    if (!reply || context->err) {
        if (reply) {
            std::cout << "Error: " << context->errstr << std::endl;
        } else {
            std::cout << "Command Error!" << std::endl;
        }
    } else {
        std::cout << "Reply type: " << reply->type << std::endl;
        if (reply->str) {
            std::cout << "Reply string: " << reply->str << std::endl;
        }
    }
    freeReplyObject(reply);
    std::cout << std::endl;

    reply = static_cast<redisReply *>(redisCommand(context, "HSET %s %s %s", "EMPLOYEE|ALICE", "NAME", "ALICE"));
    if (!reply || context->err) {
        if (reply) {
            std::cout << "Error: " << context->errstr << std::endl;
        } else {
            std::cout << "Command Error!" << std::endl;
        }
    } else {
        std::cout << "Reply type: " << reply->type << std::endl;
        if (reply->str) {
            std::cout << "Reply string: " << reply->str << std::endl;
        }
    }
    freeReplyObject(reply);
    std::cout << std::endl;

    reply = static_cast<redisReply *>(redisCommand(context, "HSET %s %s %s", "EMPLOYEE|ALICE", "AGE", "188"));
    if (!reply || context->err) {
        if (reply) {
            std::cout << "Error: " << context->errstr << std::endl;
        } else {
            std::cout << "Command Error!" << std::endl;
        }
    } else {
        std::cout << "Reply type: " << reply->type << std::endl;
        if (reply->str) {
            std::cout << "Reply string: " << reply->str << std::endl;
        }
    }
    freeReplyObject(reply);
    std::cout << std::endl;

    reply = static_cast<redisReply *>(redisCommand(context, "HGETALL %s", "EMPLOYEE|ALICE"));
    if (!reply || context->err) {
        if (reply) {
            std::cout << "Error: " << context->errstr << std::endl;
        } else {
            std::cout << "Command Error!" << std::endl;
        }
    } else {
        std::cout << "Reply type: " << reply->type << std::endl;
        if (reply->str) {
            std::cout << "Reply string: " << reply->str << std::endl;
        }
        for (size_t i = 0; i < reply->elements; ++i) {
            redisReply *reply_elem = reply->element[i];
            std::cout << reply_elem->str << std::endl;
        }
    }
    freeReplyObject(reply);
    std::cout << std::endl;

    const char *arguments[] = {"HGETALL", "EMPLOYEE|ALICE"};
    reply = static_cast<redisReply *>(redisCommandArgv(context, 2, arguments, NULL));
    if (!reply || context->err) {
        if (reply) {
            std::cout << "Error: " << context->errstr << std::endl;
        } else {
            std::cout << "Command Error!" << std::endl;
        }
    } else {
        std::cout << "Reply type: " << reply->type << std::endl;
        if (reply->str) {
            std::cout << "Reply string: " << reply->str << std::endl;
        }
        for (size_t i = 0; i < reply->elements; ++i) {
            redisReply *reply_elem = reply->element[i];
            std::cout << reply_elem->str << std::endl;
        }
    }
    freeReplyObject(reply);
    std::cout << std::endl;

    redisFree(context);
    std::cout << "END~" << std::endl;
    return 0;
}