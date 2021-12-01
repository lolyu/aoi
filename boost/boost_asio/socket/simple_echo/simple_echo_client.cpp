#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using namespace boost;
using namespace std;

/*
server side:
# ncat -l 9000 --keep-open --exec "/bin/cat"
*/

// simple echo client/server must agree on the buffer size to read/write

const int BUF_SIZE = 16;

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
    const char send_data[BUF_SIZE] = "HELLOWORLD";
    unique_ptr<char[]> receive_data(new char[sizeof(char) * BUF_SIZE]);

    while (sock.is_open())
    {
        cout << "Send message: " << send_data << endl;
        asio::write(sock, asio::buffer(send_data));
        this_thread::sleep_for(chrono::seconds(1));
        system::error_code ec;
        asio::read(sock, asio::buffer(receive_data.get(), BUF_SIZE), ec);
        cout << "Receive message: " << receive_data.get() << endl;
        if (ec.value())
        {
            if (ec.value() == asio::error::eof)
            {
                cout << "Received EOF, exiting..." << endl;
                break;
            }
            else
            {
                throw system::system_error(ec);
            }
        }
    }
}
