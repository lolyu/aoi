#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 120;

    boost::system::error_code ec;

    asio::ip::address ip_address_0 = asio::ip::address::from_string(raw_ip_address, ec);
    if (ec.value())
    {
        std::cout << ec.value() << std::endl
                  << ec.message() << std::endl;
        return ec.value();
    }

    asio::ip::address ip_address_1 = asio::ip::address_v4::any();

    // 127.0.0.1
    std::cout << ip_address_0.to_string() << std::endl;
    // 0.0.0.0
    std::cout << ip_address_1.to_string() << std::endl;

    asio::ip::tcp::endpoint ep_0(ip_address_0, port_num);
    asio::ip::tcp::endpoint ep_1(ip_address_1, port_num);
    return 0;
}