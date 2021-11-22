#include <boost/asio.hpp>
#include <iostream>

using namespace boost;
using namespace std;

const int BUF_SIZE = 16;

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
    unique_ptr<char[]> received_data(new char[BUF_SIZE]);
    while (sock.is_open())
    {
        asio::read(sock, asio::buffer(received_data.get(), BUF_SIZE), ec);
        if (ec.value())
        {
            if (ec.value() != asio::error::eof)
            {
                throw system::system_error(ec);
            }
            else
            {
                break;
            }
        }
        cout << "Echo received message: " << received_data.get() << endl;
        asio::write(sock, asio::buffer(received_data.get(), BUF_SIZE));
    }
}