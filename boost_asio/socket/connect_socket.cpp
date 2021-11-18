#include <boost/asio.hpp>
#include <iostream>

using namespace boost;
using namespace std;

int main()
{
    // nc -4 -l -p 9000 -v
    try
    {
        asio::ip::address server = asio::ip::address::from_string("127.0.0.1");
        short port = 9000;
        asio::ip::tcp::endpoint ep(server, port);
        asio::io_service ios;
        asio::ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
    }
    catch (const system::system_error &e)
    {
        cout << "Error: " << e.code() << ", message: " << e.what() << endl;
    }

    return 0;
}