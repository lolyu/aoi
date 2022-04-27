#include <hiredis/hiredis.h>

#include <iostream>

#include "swss/dbconnector.h"
#include "swss/rediscommand.h"
#include "swss/redisreply.h"

using namespace std;

class RedisCntxt : public swss::RedisContext
{
public:
    RedisCntxt()
        : RedisContext() {}

    void initContext(const char *host, int port, const timeval *tv = nullptr) { swss::RedisContext::initContext(host, port, tv); }
};

int main()
{
    swss::RedisCommand redisCommand;
    redisCommand.format("HGETALL %s", "EMPLOYEE|ALICE");
    cout << redisCommand.c_str() << endl;

    RedisCntxt redisContext;
    redisContext.initContext("172.17.0.1", 6379);

    swss::RedisReply redisReply(&redisContext, redisCommand, REDIS_REPLY_ARRAY);
    for (int i = 0; i < redisReply.getChildCount(); ++i) {
        cout << redisReply.getChild(i)->str << '\t';
    }
    cout << endl;
    return 0;
}
