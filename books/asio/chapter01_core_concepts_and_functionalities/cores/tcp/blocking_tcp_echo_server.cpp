#include <array>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum
{
    max_length = 1024
};

void handle_conn(tcp::socket socket)
{
    std::array<uint8_t, max_length> buffer;
    boost::system::error_code error;

    try
    {
        if (socket.is_open())
        {
            std::cout << "Accept connection from " << socket.remote_endpoint() << std::endl;
        }

        while (socket.is_open())
        {
            size_t bytes_received = socket.read_some(boost::asio::buffer(buffer), error);
            if (error == boost::asio::error::eof)
                break;
            else if (error)
                throw boost::system::system_error(error);

            socket.write_some(boost::asio::buffer(buffer, bytes_received));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in thread " << std::this_thread::get_id() << ": " << e.what() << std::endl;
    }
}

void serve(boost::asio::io_service &ios, unsigned short port)
{
    tcp::acceptor acceptor(ios, tcp::endpoint(tcp::v4(), port));
    acceptor.set_option(tcp::acceptor::reuse_address(true));
    while (true)
    {
        // tcp::acceptor::accept is blocking
        auto thread = std::thread(handle_conn, acceptor.accept());
        std::cout << "Create a new thread " << thread.get_id() << std::endl;
        thread.detach();
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: blocking_tcp_echo_server <port>" << std::endl;
        return 1;
    }

    try
    {
        boost::asio::io_service ios;
        serve(ios, ::atoi(argv[1]));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
