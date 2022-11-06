#include <iostream>
#include <array>
#include <memory>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#define CONCURRENCY_LIMIT 5

// client: # nc localhost 9000

enum
{
    max_length = 1024
};

class session : public std::enable_shared_from_this<session>
{
public:
    session(boost::asio::ip::tcp::socket &socket) : socket(std::move(socket))
    {
    }

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        socket.async_read_some(
            boost::asio::buffer(data),
            [this, self](const boost::system::error_code &error, std::size_t bytes_transferred)
            {
                if (!error)
                {
                    std::cout << "Received " << bytes_transferred << " bytes data from " << socket.remote_endpoint() << std::endl;
                    do_write(bytes_transferred);
                }
            });
    }

    void do_write(size_t bytes_transferred)
    {
        auto self(shared_from_this());
        socket.async_write_some(
            boost::asio::buffer(data, bytes_transferred),
            [this, self](const boost::system::error_code &error, std::size_t bytes_transferred)
            {
                if (!error)
                {
                    do_read();
                }
            });
    }

private:
    boost::asio::ip::tcp::socket socket;
    std::array<uint8_t, max_length> data;
};

class server
{
public:
    server(boost::asio::io_service &ios, short port)
        : ios(ios), port(port),
          acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
    }

    void do_accept()
    {
        acceptor.async_accept(
            [this](const boost::system::error_code &ec, boost::asio::ip::tcp::socket socket)
            {
                if (!ec)
                {
                    std::cout << "Accept new connection from " << socket.remote_endpoint() << std::endl;
                    // std::make_shared<session>(std::move(socket))->start();
                    std::make_shared<session>(socket)->start();
                }
                do_accept();
            });
    }

private:
    boost::asio::io_service &ios;
    boost::asio::ip::tcp::acceptor acceptor;
    short port;
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: async_tcp_echo_server <port>" << std::endl;
        return 1;
    }

    try
    {
        boost::asio::io_service ios;
        auto work = std::make_unique<boost::asio::io_service::work>(ios);

        boost::thread_group threads;
        for (int i = 0; i < CONCURRENCY_LIMIT; ++i)
        {
            threads.create_thread([&ios]()
                                  { ios.run(); });
        }

        server s(ios, std::atoi(argv[1]));
        s.do_accept();

        std::cout << "Start the server" << std::endl;

        work.reset();
        threads.join_all();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
