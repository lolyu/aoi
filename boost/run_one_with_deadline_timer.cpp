#include <iostream>
#include <boost/asio.hpp>
#include <memory>

int main()
{
    int run_count;
    boost::asio::io_service ios;
    boost::asio::deadline_timer timer(ios);
    timer.expires_from_now(boost::posix_time::seconds(2));
    timer.async_wait([](const boost::system::error_code &ec)
                     {
                         std::cout << "Timer expires, ";
                         if (!ec)
                         {
                             std::cout << "Timer expired at " << std::time(0) << std::endl;
                         }
                     });
    // run_one will only block till it finishes one handler
    // if there is no handlers left, run_one will return immediately
    run_count = ios.run_one();
    std::cout << run_count << std::endl;
    run_count = ios.run_one();
    std::cout << run_count << std::endl;
    return 0;
}