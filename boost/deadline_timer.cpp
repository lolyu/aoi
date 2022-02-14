#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <thread>

/*
deadline_timer::wait will run the io_service
deadline_timer::async_wait won't, the io_service should be running
*/

void blocking_wait(std::shared_ptr<boost::asio::deadline_timer> timer)
{
    timer->expires_from_now(boost::posix_time::seconds(2));
    timer->wait();
}

void nonblocking_wait(std::shared_ptr<boost::asio::deadline_timer> timer)
{
    timer->expires_from_now(boost::posix_time::seconds(2));
    timer->async_wait([](const boost::system::error_code &ec)
                      {
                          std::cout << "Timer expires, ";
                          if (!ec)
                          {
                              std::cout << "Timer expired at " << std::time(0) << std::endl;
                          } });
}

int main()
{
    std::shared_ptr<boost::asio::io_service> ios(new boost::asio::io_service);
    std::thread th([ios]()
                   { ios->run(); });

    std::shared_ptr<boost::asio::deadline_timer> timer(new boost::asio::deadline_timer(*ios));
    std::unique_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*ios));

    std::cout << std::time(0) << std::endl;
    blocking_wait(timer);
    std::cout << std::time(0) << std::endl;
    nonblocking_wait(timer);
    std::cout << std::time(0) << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int i = timer->cancel();
    std::cout << "Cancel " << i << " wait handlers" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    work.reset();
    th.join();

    return 0;
}