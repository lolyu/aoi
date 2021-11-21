#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

struct Session;
void writeToSockAsync(std::shared_ptr<asio::ip::tcp::socket> sock, std::string &message);
void callback(const boost::system::error_code &ec, std::size_t bytes_transferred, std::shared_ptr<Session> s);

int main()
{
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 9000;

    try
    {
        std::string message = "HELLOWORLD";
        asio::ip::tcp::endpoint ep(asio::ip::address_v4::from_string(raw_ip_address), port_num);
        asio::io_service ios;

        std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));
        sock->connect(ep);
        writeToSockAsync(sock, message);

        // asio::os_service::run returns after the last asynchronous callback is finished
        ios.run();
    }
    catch (const system::system_error &e)
    {
        std::cout << "Error code: " << e.code() << ", error message: " << e.what() << std::endl;
    }

    return 0;
}

struct Session
{
    std::shared_ptr<asio::ip::tcp::socket> sock;
    std::string buf;
    std::size_t total_bytes_written;
};

void callback(const boost::system::error_code &ec, std::size_t bytes_transferred, std::shared_ptr<Session> s)
{
    if (ec.value() != 0)
    {
        std::cout << "Error occured! Error code = " << ec.value() << ". Message: " << ec.message();
        return;
    }
    s->total_bytes_written += bytes_transferred;
    if (s->total_bytes_written == s->buf.length())
    {
        return;
    }
    s->sock->async_write_some(
        asio::buffer(s->buf.c_str() + s->total_bytes_written, s->buf.length() - s->total_bytes_written),
        std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}

void writeToSockAsync(std::shared_ptr<asio::ip::tcp::socket> sock, std::string &message)
{
    std::shared_ptr<struct Session> s(new struct Session);

    s->buf = message;
    s->total_bytes_written = 0;
    s->sock = sock;
    s->sock->async_write_some(asio::buffer(s->buf), std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}
