#include <array>
#include <iostream>
#include <memory>
#include <string>

#include <netpacket/packet.h>
#include <net/ethernet.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

class Sniffer
{
public:
    Sniffer(boost::asio::io_service &ios, const std::string &port_name)
        : port_name(port_name), ios(ios), raw_socket(ios, boost::asio::generic::raw_protocol(PF_PACKET, SOCK_RAW))
    {
        sockaddr_ll sockaddr{0};
        sockaddr.sll_family = PF_PACKET;
        sockaddr.sll_protocol = htons(ETH_P_ALL);
        sockaddr.sll_ifindex = if_nametoindex(port_name.c_str());
        sockaddr.sll_hatype = 1;
        auto end_point = boost::asio::generic::basic_endpoint<boost::asio::generic::raw_protocol>(&sockaddr, sizeof(sockaddr));
        raw_socket.bind(end_point);
        raw_socket.non_blocking();
    }

    void start_sniff()
    {
        raw_socket.async_receive(
            boost::asio::null_buffers(),
            boost::bind(
                &Sniffer::handle_recv,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_recv(const boost::system::error_code &ec, size_t bytes_transferred)
    {
        if (ec)
        {
            std::cout << "Error: " << ec.message() << std::endl;
        }
        else
        {
            std::cout << bytes_transferred << " bytes read" << std::endl;
            bytes_transferred = raw_socket.receive(boost::asio::buffer(buffer, buffer.size()));
            std::cout << bytes_transferred << " bytes read" << std::endl;

            std::cout << "Source MAC address: ";
            std::cout << boost::format("%02X-%02X-%02X-%02X-%02X-%02X") %
                             buffer[0] % buffer[1] % buffer[2] %
                             buffer[3] % buffer[4] % buffer[5];
            std::cout << std::endl;
            std::cout << "Dest MAC address: ";
            std::cout << boost::format("%02X-%02X-%02X-%02X-%02X-%02X") %
                             buffer[6] % buffer[7] % buffer[8] %
                             buffer[9] % buffer[10] % buffer[11];
            std::cout << std::endl;
        }
        start_sniff();
    }

    boost::asio::io_service &get_io_service() { return ios; }

private:
    const std::string &port_name;
    std::array<uint8_t, 128> buffer;
    boost::asio::io_service &ios;
    boost::asio::generic::raw_protocol::socket raw_socket;
};

int main()
{
    boost::asio::io_service ios;
    auto work = std::make_unique<boost::asio::io_service::work>(ios);
    Sniffer sniffer(ios, "lo");

    boost::thread_group threads;
    threads.create_thread([&]()
                          { ios.run(); });
    sniffer.start_sniff();
    work.reset();
    threads.join_all();
    return 0;
}
