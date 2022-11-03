#include <array>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <string.h>

int main()
{
    const char *words = "helloworld";

    // an individual buffer may be created from:
    // * builtin array
    // * std::vector
    // * std::array
    // * boost::array
    // of POD elements

    char d1[128];
    std::vector<char> d2(128);
    std::array<char, 128> d3;
    const char d4[] = "helloworld";

    boost::asio::mutable_buffer buffer1 = boost::asio::buffer(d1);
    boost::asio::mutable_buffer buffer2 = boost::asio::buffer(d2);
    boost::asio::mutable_buffer buffer3 = boost::asio::buffer(d3);
    boost::asio::mutable_buffer buffer4 = boost::asio::buffer(d2);
    boost::asio::const_buffer buffer5 = boost::asio::buffer(d4);

    d2[0] = 'c';
    d2[1] = 'b';
    d2[2] = 'a';
    d2[3] = '\0';
    std::cout << d2.data() << std::endl;
    std::cout << static_cast<char *>(buffer2.data()) << std::endl;
    std::cout << static_cast<char *>(buffer4.data()) << std::endl;

    return 0;
}
