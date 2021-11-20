#include <iostream>
#include <boost/asio.hpp>

using namespace boost;

int main()
{
    asio::streambuf buf;
    std::ostream output(&buf);
    output << "Message1\nMessage2\n";

    std::istream input(&buf);
    std::string message1;
    std::getline(input, message1);
    std::cout << message1 << std::endl;
    std::getline(input, message1);
    std::cout << message1 << std::endl;
    return 0;
}