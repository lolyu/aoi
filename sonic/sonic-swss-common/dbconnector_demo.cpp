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
    return 0;
}
