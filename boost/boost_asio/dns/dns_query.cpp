#include <boost/asio.hpp>
#include <iostream>

using namespace boost;
using namespace std;

int main()
{
    string host = "www.google.com";
    string port_num = "3333";

    asio::io_service ios;

    asio::ip::tcp::resolver::query resolver_query(host, port_num, asio::ip::tcp::resolver::query::numeric_service);

    asio::ip::tcp::resolver resolver(ios);

    boost::system::error_code ec;

    asio::ip::tcp::resolver::iterator it = resolver.resolve(resolver_query, ec);
    if (ec.value())
    {
        cout << "Failed to resolve a DNS name, error code = " << ec.value() << ", error message = " << ec.message();
        return ec.value();
    }
    asio::ip::tcp::resolver::iterator it_end;
    for (; it != it_end; ++it)
    {
        asio::ip::tcp::endpoint ep = it->endpoint();
        cout << "Host " << it->host_name() << " ip" << (ep.address().is_v4() ? "v4: " : "v6: ") << ep.address().to_string() << endl;
    }

    return 0;
}