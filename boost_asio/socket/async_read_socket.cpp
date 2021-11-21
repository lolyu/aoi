#include <iostream>
#include <boost/asio.hpp>
#include <memory>

struct Session
{
    std::shared_ptr<boost::asio::ip::tcp::socket> sock;
    std::unique_ptr<char[]> buf;
    std::size_t total_bytes_read;
    unsigned int buf_size;
};

void callback(const boost::system::error_code &ec, std::size_t bytes_transferred, std::shared_ptr<struct Session> s);
void readFromSocket(std::shared_ptr<struct Session> &s);

const size_t BUF_SIZE = 1024;

int main()
{
    boost::asio::io_service ios;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), 9000);
    boost::asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
    acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

    try
    {
        acceptor.bind(ep);
        acceptor.listen();
        struct Session *sp = new Session();
        sp->sock.reset(new boost::asio::ip::tcp::socket(ios));
        sp->buf.reset(new char[BUF_SIZE]);
        sp->total_bytes_read = 0;
        sp->buf_size = BUF_SIZE;
        acceptor.accept(*(sp->sock.get()));
        std::shared_ptr<struct Session> ssp(sp);
        // readFromSocket(ssp);
        boost::asio::async_read(*(sp->sock.get()), boost::asio::buffer(sp->buf.get(), sp->buf_size), std::bind(callback, std::placeholders::_1, std::placeholders::_2, ssp));
        ios.run();

        std::cout << std::string(ssp->buf.get(), ssp->total_bytes_read) << std::endl;
    }
    catch (const boost::system::system_error &e)
    {
        std::cout << "Error code: " << e.code() << ", error message: " << e.what() << std::endl;
    }
    return 0;
}

void callback(const boost::system::error_code &ec, std::size_t bytes_transferred, std::shared_ptr<struct Session> s)
{
    // increase the bytes counter even EOF is reached
    s->total_bytes_read += bytes_transferred;
    if (ec.value() != 0)
    {
        std::cout << "Error code: " << ec.value() << ", error message: " << ec.message() << std::endl;
        return;
    }

    if (s->sock.get()->is_open() && s->total_bytes_read < s->buf_size)
    {
        s->sock->async_read_some(
            boost::asio::buffer(s->buf.get() + s->total_bytes_read, s->buf_size - s->total_bytes_read),
            std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
    }
    return;
}

void readFromSocket(std::shared_ptr<struct Session> &s)
{
    s->sock->async_read_some(boost::asio::buffer(s->buf.get(), s->buf_size), std::bind(callback, std::placeholders::_1, std::placeholders::_2, s));
}