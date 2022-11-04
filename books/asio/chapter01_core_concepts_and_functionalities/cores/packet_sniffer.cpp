#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <malloc.h>

#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>

#define BUFFER_SIZE 30
std::array<uint8_t, BUFFER_SIZE> buffer;
std::shared_ptr<boost::asio::io_service> ios;
std::shared_ptr<boost::asio::io_context::strand> strand;
std::shared_ptr<boost::asio::posix::stream_descriptor> stream;

void startRecv();
void handleRecv(const boost::system::error_code &ec, size_t bytes_transferred);

int main()
{
    ios = std::make_shared<boost::asio::io_service>();
    strand = std::make_shared<boost::asio::io_service::strand>(*ios);
    stream = std::make_shared<boost::asio::posix::stream_descriptor>(*ios);
    int sock = socket(AF_PACKET, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_ICMP);

    sockaddr_ll addr = {0};
    addr.sll_ifindex = if_nametoindex("lo");
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    stream->assign(sock);

    startRecv();
    ios->run();
}

void handleRecv(const boost::system::error_code &ec, size_t bytes_transferred)
{
    if (ec)
    {
        std::cout << "Error: " << ec.message() << std::endl;
    }
    else
    {
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
    startRecv();
}

void startRecv()
{
    stream->async_read_some(
        boost::asio::buffer(buffer, BUFFER_SIZE),
        strand->wrap(
            boost::bind(
                &handleRecv,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred)));
}
