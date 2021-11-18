#include <boost/asio.hpp>
#include <iostream>

using namespace boost;
using namespace std;

int main()
{
    unsigned short port_num = 3333;

    asio::ip::tcp::endpoint ep_tcp(asio::ip::address_v4::any(), port_num);
    asio::ip::udp::endpoint ep_udp(asio::ip::address_v4::any(), port_num);
    asio::io_service ios;

    asio::ip::tcp::acceptor acceptor(ios, ep_tcp.protocol());
    asio::ip::udp::socket sock(ios, ep_udp.protocol());

    boost::system::error_code ec;

    acceptor.bind(ep_tcp, ec);

    if (ec.value() != 0)
    {
        cout << "Failed to bind the acceptor socket. Error code = " << ec.value() << ". Message: " << ec.message();
        return ec.value();
    }

    sock.bind(ep_udp, ec);
    if (ec.value() != 0)
    {
        cout << "Failed to bind the udp socket. Error code = " << ec.value() << ". Message: " << ec.message();
        return ec.value();
    }

    return 0;
}