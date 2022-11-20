#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <chrono>

#include "icmp_header.h"
#include "ipv4_header.h"

class pinger
{
public:
    pinger(boost::asio::io_service &ios, const char *destination_address)
        : resolver(ios), socket(ios, boost::asio::ip::icmp::v4()), timer(ios)
    {
        destination = *resolver.resolve(boost::asio::ip::icmp::v4(), destination_address, "").begin();

        start_send();
        start_receive();
    }

private:
    void start_send()
    {
        std::string body("\"Hello!\" from Asio ping.");

        icmp_header echo_request;
        echo_request.type(icmp_header::echo_request);
        echo_request.code(0);
        echo_request.identifier(get_identifier());
        echo_request.sequence_number(++sequence_number);
        compute_checksum(echo_request, body.begin(), body.end());

        boost::asio::streambuf request_buffer;
        std::ostream os(&request_buffer);
        os << echo_request << body;

        time_sent = std::chrono::steady_clock::now();
        socket.send_to(request_buffer.data(), destination);

        num_replies = 0;
        timer.expires_at(time_sent + std::chrono::seconds(5));
        timer.async_wait(boost::bind(&pinger::handle_timeout, this));
    }

    void handle_timeout()
    {
        if (num_replies == 0)
        {
            std::cout << "Request timed out" << std::endl;
        }

        start_send();
    }

    void start_receive()
    {
        reply_buffer.consume(reply_buffer.size());

        socket.async_receive(
            reply_buffer.prepare(65536),
            boost::bind(&pinger::handle_receive, this, boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(std::size_t bytes_transferred)
    {
        reply_buffer.commit(bytes_transferred);

        std::istream is(&reply_buffer);
        ipv4_header ipv4_hdr;
        icmp_header icmp_hdr;
        is >> ipv4_hdr >> icmp_hdr;

        if (is &&
            icmp_hdr.type() == icmp_header::echo_reply &&
            icmp_hdr.identifier() == get_identifier() &&
            icmp_hdr.sequence_number() == sequence_number)
        {
            if (num_replies++ == 0)
            {
                timer.cancel();
            }

            auto now = std::chrono::steady_clock::now();
            auto elapsed = now - time_sent;
            std::cout
                << bytes_transferred - ipv4_hdr.header_length()
                << " bytes from " << ipv4_hdr.source_address()
                << ": icmp_seq=" << icmp_hdr.sequence_number()
                << ", ttl=" << ipv4_hdr.time_to_live()
                << ", time=" << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count()
                << std::endl;
        }

        start_receive();
    }

    static unsigned short get_identifier()
    {
        return static_cast<unsigned short>(::getpid());
    }

private:
    boost::asio::ip::icmp::resolver resolver;
    boost::asio::ip::icmp::endpoint destination;
    boost::asio::ip::icmp::socket socket;
    boost::asio::steady_timer timer;
    unsigned short sequence_number = 0;
    std::chrono::steady_clock::time_point time_sent;
    boost::asio::streambuf reply_buffer;
    std::size_t num_replies = 0;
};

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: ping <host>" << std::endl;
            std::cerr << "(You may need to run this program as root.)" << std::endl;
            return 1;
        }

        boost::asio::io_service ios;
        pinger p(ios, argv[1]);
        ios.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }
}
