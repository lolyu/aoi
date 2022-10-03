#include <iostream>
#include <swss/dbconnector.h>

using namespace std;

int main()
{
    string dbname = "CONFIG_DB";
    swss::DBConnector context(dbname, 0, true);
    auto ports = context.keys("PORT|*");
    for (auto &port : ports)
    {
        cout << port << endl;
    }

    auto port_config = context.hgetall(ports[0]);
    cout << endl
         << "Port table key value pairs for " << ports[0] << endl;
    for (auto &it : port_config)
    {
        cout << it.first << ": " << it.second << endl;
    }

    cout << endl;
    swss::RedisCommand command;
    command.format("keys %s", "PORT|*");
    cout << command.c_str() << endl;

    swss::RedisReply reply(&context, command, REDIS_REPLY_ARRAY);
    cout << reply.to_string() << endl;
    return 0;
}
