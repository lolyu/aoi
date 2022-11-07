#include <iostream>
#include <boost/asio.hpp>

int main()
{
    boost::asio::io_service ios;
    boost::asio::deadline_timer timer(ios);
    timer.expires_from_now(boost::posix_time::seconds(4));
    auto time_to_expire = timer.expires_from_now();
    std::cout << time_to_expire.seconds() << std::endl;

    timer.wait();

    return 0;
}
