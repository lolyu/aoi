#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

std::string readFromSocket(asio::ip::tcp::socket &sock, size_t read_size = 8);
std::string readFromSocketAsioRead(asio::ip::tcp::socket &sock, size_t read_size = 8);
std::string readFromSocketDelim(asio::ip::tcp::socket &sock, char delim = '\n');

int main()
{
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 9000;

    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);
        asio::io_service ios;
        asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
        asio::ip::tcp::socket sock(ios);
        acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        acceptor.bind(ep);
        acceptor.listen();
        acceptor.accept(sock);
        // std::cout << readFromSocket(sock) << ":";
        // std::cout << readFromSocketAsioRead(sock) << ":";
        // std::cout << readFromSocketAsioRead(sock) << ":";
        while (true)
        {
            std::cout << readFromSocketDelim(sock) << ":";
        }
    }
    catch (const system::system_error &e)
    {
        std::cout << "Error code: " << e.code() << ", error message: " << e.what() << std::endl;
    }

    return 0;
}

std::string readFromSocket(asio::ip::tcp::socket &sock, size_t read_size)
{
    char *buffer = new char[read_size];
    read_size = sock.read_some(asio::buffer(buffer, read_size));
    std::string input(buffer, read_size);
    delete[] buffer;
    return input;
}

std::string readFromSocketAsioRead(asio::ip::tcp::socket &sock, size_t read_size)
{
    char *buffer = new char[read_size];
    asio::read(sock, asio::buffer(asio::buffer(buffer, read_size)));
    std::string input(buffer, read_size);
    delete[] buffer;
    return input;
}

std::string readFromSocketDelim(asio::ip::tcp::socket &sock, char delim)
{
    // after read_until, the buffer might contains multiple delimitor symbols
    static asio::streambuf buffer;
    std::cout << std::endl
              << "BEFORE READ: " << buffer.data().size() << std::endl;
    asio::read_until(sock, buffer, delim);
    std::cout << std::endl
              << "AFTER READ: " << buffer.data().size() << std::endl;
    std::istream input_s(&buffer);
    std::string line;
    std::getline(input_s, line);
    return line;
}
