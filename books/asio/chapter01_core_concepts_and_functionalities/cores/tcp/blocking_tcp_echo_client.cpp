#include <iostream>
#include <string>
#include <boost/asio.hpp>

// ncat -l 9000 --keep-open --exec "/bin/cat"

enum
{
    max_length = 1024
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: blocking_tcp_echo_client <host> <port>" << std::endl;
        return 1;
    }

    using boost::asio::ip::tcp;

    try
    {
        boost::asio::io_service ios;

        tcp::socket socket(ios);
        tcp::resolver resolver(ios);
        tcp::resolver::query query(argv[1], argv[2]);
        tcp::resolver::iterator iter = resolver.resolve(query);
        tcp::resolver::iterator end;
        while (iter != end)
        {
            tcp::endpoint endpoint = *iter++;
            std::cout << "Connect to endpoint: " << endpoint << std::endl;
            socket.connect(endpoint);
            break;
        }
        if (!socket.is_open())
        {
            std::cerr << "Fail to connect, exit" << std::endl;
            return 1;
        }

        // boost::asio::connect(socket, resolver.resolve(argv[1], argv[2]));

        std::string input;
        std::cout << "Enter message: ";
        std::cin >> input;

        if (input.length() > max_length)
        {
            input = input.substr(0, max_length);
        }
        std::cout << "Reply is: " << input << std::endl;
        socket.send(boost::asio::buffer(input));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
