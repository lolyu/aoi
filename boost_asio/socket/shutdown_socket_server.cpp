#include <boost/asio.hpp>
#include <iostream>

using namespace boost;
using namespace std;

void processRequest(asio::ip::tcp::socket &sock);

int main()
{
    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(), 9000);
        asio::io_service ios;
        asio::ip::tcp::acceptor acceptor(ios, ep.protocol());
        acceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        asio::ip::tcp::socket sock(ios);
        acceptor.bind(ep);
        acceptor.listen();
        acceptor.accept(sock);
        processRequest(sock);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}

void processRequest(asio::ip::tcp::socket &sock)
{

    system::error_code ec;
    while (sock.is_open())
    {
        asio::streambuf request_buf;
        asio::read(sock, request_buf, ec);
        auto data = request_buf.data();
        std::string msg(asio::buffers_begin(data), asio::buffers_begin(data) + data.size());
        cout << "Received message: " << msg << endl;
        asio::write(sock, asio::buffer(request_buf.data()));
        if (ec.value() != asio::error::eof)
        {
            throw system::system_error(ec);
        }
        else
        {
            break;
        }
    }
}