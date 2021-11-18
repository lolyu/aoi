#include <boost/asio.hpp>
#include <iostream>

using namespace boost;
using namespace std;

int main()
{
    const int BACKLOG_SIZE = 30;
    unsigned short port_num = 3333;

    asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), port_num);
    asio::io_service ios;

    try
    {
        asio::ip::tcp::acceptor acceptor(ios);
        acceptor.open(ep.protocol());
        acceptor.bind(ep);
        acceptor.listen(BACKLOG_SIZE);
        asio::ip::tcp::socket client(ios);
        acceptor.accept(client);
    }
    catch (const system::system_error &e)
    {
        cout << "Error code = " << e.code() << ", error message = " << e.what() << endl;
    }

    return 0;
}