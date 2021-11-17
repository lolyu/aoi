#include <iostream>
#include <boost/asio.hpp>

int main()
{
    // io_service interacts with the OS
    boost::asio::io_service ios;

    // tcp part
    boost::asio::ip::tcp tcp_protocol = boost::asio::ip::tcp::v4();
    // 1
    boost::asio::ip::tcp::socket sock0(ios);
    boost::system::error_code ec;
    // in open, a real OS socket is created
    sock0.open(tcp_protocol, ec);
    if (ec.value())
    {
        std::cout << "Error code " << ec.value() << " with error message " << ec.message() << std::endl;
        return ec.value();
    }

    // 2
    try
    {
        boost::asio::ip::tcp::socket sock1(ios, tcp_protocol);
    }
    catch (const boost::system::system_error &e)
    {
        std::cout << "Error code " << e.code() << " with error message " << e.what() << std::endl;
    }

    // udp part
    boost::asio::ip::udp udp_protocol = boost::asio::ip::udp::v4();
    // 1
    boost::asio::ip::udp::socket sock2(ios);
    sock2.open(udp_protocol, ec);
    if (ec.value())
    {
        std::cout << "Error code " << ec.value() << " with error message " << ec.message() << std::endl;
        return ec.value();
    }

    // 2
    try
    {
        boost::asio::ip::udp::socket sock3(ios, udp_protocol);
    }
    catch (const boost::system::system_error &e)
    {
        std::cout << "Error code " << e.code() << " with error message " << e.what() << std::endl;
    }

    // passive socket
    boost::asio::ip::tcp tcp_protocol_ipv6 = boost::asio::ip::tcp::v6();
    boost::asio::ip::tcp::acceptor sock4(ios);

    sock4.open(tcp_protocol_ipv6, ec);
    if (ec.value())
    {
        std::cout << "Error code " << ec.value() << " with error message " << ec.message() << std::endl;
        return ec.value();
    }

    return 0;
}