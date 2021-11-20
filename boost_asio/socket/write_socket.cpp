#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

void writeToSocket(asio::ip::tcp::socket &, const std::string &);
void writeToSocketAsioWrite(asio::ip::tcp::socket &, const std::string &);

int main()
{
    asio::ip::address local_any = asio::ip::address_v4::any();
    unsigned short port_num = 9000;
    asio::io_service ios;
    asio::ip::tcp::endpoint ep(local_any, port_num);
    asio::ip::tcp::socket sock(ios, ep.protocol());
    std::string message = "helloworld\n";
    try
    {
        sock.connect(ep);
        writeToSocket(sock, message);
        writeToSocketAsioWrite(sock, message);
        writeToSocket(sock, message);
        writeToSocketAsioWrite(sock, message);
    }
    catch (const system::system_error &e)
    {
        std::cout << "Error code: " << e.code() << ", error message: " << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

void writeToSocket(asio::ip::tcp::socket &sock, const std::string &msg)
{
    size_t total_written = 0;
    while (total_written < msg.length())
    {
        total_written += sock.write_some(asio::buffer(msg.c_str() + total_written, msg.length() - total_written));
    }
}

void writeToSocketAsioWrite(asio::ip::tcp::socket &sock, const std::string &msg)
{
    // https://www.boost.org/doc/libs/1_58_0/doc/html/boost_asio/reference/SyncWriteStream.html
    asio::write(sock, asio::buffer(msg));
}
