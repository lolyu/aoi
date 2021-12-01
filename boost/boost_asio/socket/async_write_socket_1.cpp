#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

void callback(const boost::system::error_code &ec, std::size_t bytes_transferred);

int main()
{
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 9000;

    try
    {
        std::string message = "HELLOWORLD";
        asio::ip::tcp::endpoint ep(asio::ip::address_v4::from_string(raw_ip_address), port_num);
        asio::io_service ios;
        asio::ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
        asio::async_write(sock, asio::buffer(message), callback);

        // asio::os_service::run returns after the last asynchronous callback is finished
        ios.run();
    }
    catch (const system::system_error &e)
    {
        std::cout << "Error code: " << e.code() << ", error message: " << e.what() << std::endl;
    }

    return 0;
}

void callback(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
    if (ec.value() != 0)
    {
        std::cout << "Error occured! Error code = " << ec.value() << ". Message: " << ec.message();
        return;
    }
    return;
}
