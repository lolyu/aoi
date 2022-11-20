#include <iostream>
#include <string>

#include <boost/asio.hpp>

int main()
{
    boost::asio::streambuf b;
    std::ostream os(&b);

    os << "Helloworld!\n";

    std::cout << b.capacity() << std::endl;
    std::cout << b.size() << std::endl;

    b.commit(b.size());

    std::istream is(&b);
    std::string s;
    is >> s;

    std::cout << s << std::endl;

    return 0;
}
