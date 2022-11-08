#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

void handle_signal(std::shared_ptr<boost::asio::signal_set> signals, const boost::system::error_code &error, int signal)
{
    if (!error)
    {
        std::cout << signal << std::endl;
        signals->async_wait(
            boost::bind(
                &handle_signal,
                signals,
                boost::asio::placeholders::error,
                boost::asio::placeholders::signal_number));
    }
}

int main()
{
    boost::asio::io_service ios;
    boost::asio::io_service::work work(ios);
    auto signals = std::make_shared<boost::asio::signal_set>(ios, SIGINT, SIGTERM);
    signals->async_wait(
        boost::bind(
            &handle_signal,
            signals,
            boost::asio::placeholders::error,
            boost::asio::placeholders::signal_number));
    ios.run();
    return 0;
}
