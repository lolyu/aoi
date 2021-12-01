#include <boost/asio.hpp>
#include <iostream>

using namespace boost;
using namespace std;

void communicate(asio::ip::tcp::socket &sock);

int main()
{
    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address_v4::from_string("127.0.0.1"), 9000);
        asio::io_service ios;
        asio::ip::tcp::socket sock(ios, ep.protocol());
        sock.connect(ep);
        communicate(sock);
    }
    catch (const system::system_error &e)
    {
        cout << "Error code = " << e.code() << ", error message = " << e.what() << endl;
        return e.code().value();
    }
    return 0;
}

void communicate(asio::ip::tcp::socket &sock)
{
    const char message[] = "HELLOWORLD";
    asio::write(sock, asio::buffer(message));
    // three shutdown types:
    // asio::socket_base::shutdown_receive;
    // asio::socket_base::shutdown_send;
    // asio::socket_base::shutdown_both;
    // sock.shutdown(asio::socket_base::shutdown_send);
    asio::streambuf response_buf;
    system::error_code ec;
    asio::read(sock, response_buf, ec);
    auto data = response_buf.data();
    std::string msg(asio::buffers_begin(data), asio::buffers_begin(data) + data.size());
    cout << "Received message: " << msg << endl;
    if (ec.value() == asio::error::eof)
    {
        cout << "Received EOF, exiting..." << endl;
    }
    else
    {
        throw system::system_error(ec);
    }
}

/*
shutdown_send
0000 Ether / IP / TCP 127.0.0.1:47448 > 127.0.0.1:9000 S
0001 Ether / IP / TCP 127.0.0.1:9000 > 127.0.0.1:47448 SA
0002 Ether / IP / TCP 127.0.0.1:47448 > 127.0.0.1:9000 A
0003 Ether / IP / TCP 127.0.0.1:47448 > 127.0.0.1:9000 PA / Raw
0004 Ether / IP / TCP 127.0.0.1:9000 > 127.0.0.1:47448 A
0005 Ether / IP / TCP 127.0.0.1:47448 > 127.0.0.1:9000 FA
0006 Ether / IP / TCP 127.0.0.1:9000 > 127.0.0.1:47448 FA
0007 Ether / IP / TCP 127.0.0.1:47448 > 127.0.0.1:9000 A

shutdown_receive
0000 Ether / IP / TCP 127.0.0.1:47528 > 127.0.0.1:9000 S
0001 Ether / IP / TCP 127.0.0.1:9000 > 127.0.0.1:47528 SA
0002 Ether / IP / TCP 127.0.0.1:47528 > 127.0.0.1:9000 A
0003 Ether / IP / TCP 127.0.0.1:47528 > 127.0.0.1:9000 PA / Raw
0004 Ether / IP / TCP 127.0.0.1:9000 > 127.0.0.1:47528 A
0005 Ether / IP / TCP 127.0.0.1:47528 > 127.0.0.1:9000 FA
0006 Ether / IP / TCP 127.0.0.1:9000 > 127.0.0.1:47528 FA
0007 Ether / IP / TCP 127.0.0.1:47528 > 127.0.0.1:9000 A

shutdown_both
*/