#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

int main()
{
    boost::asio::io_service ios;
    auto work = std::make_unique<boost::asio::io_service::work>(ios);
    boost::asio::deadline_timer timer(ios);
    boost::thread thread([&ios]()
                         { ios.run(); });

    timer.expires_from_now(boost::posix_time::seconds(4));
    auto time_to_expire = timer.expires_from_now();
    std::cout << time_to_expire.seconds() << std::endl;

    timer.async_wait([](const boost::system::error_code &rc)
                     { std::cout << "helloworld from deadline timer handler" << std::endl; });

    work.reset();
    thread.join();

    return 0;
}
