#include <boost/asio.hpp>
#include <memory>
#include <iostream>

int main()
{
    boost::asio::io_service ios;
    std::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(ios));

    work.reset();

    // io_service::run will block forever with the presence of a io_service::work object
    ios.run();

    return 0;
}