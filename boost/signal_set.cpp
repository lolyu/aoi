#include <boost/asio.hpp>
#include <thread>
#include <memory>
#include <iostream>
#include <chrono>

int main()
{
    std::shared_ptr<boost::asio::io_service> ios(new boost::asio::io_service);
    std::unique_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*ios));
    boost::asio::signal_set signals(*ios, SIGINT, SIGTERM);
    signals.async_wait([](const boost::system::error_code &ec, int signal)
                       {
                           if (!ec.value())
                           {
                               std::cout << "Got signal " << signal << std::endl;
                               // if don't exit here and press Ctrl + C many times, the first Ctrl + C will trigger this handler
                               // signal_set will enqueue those registered signals with no handlers, the next async_wait will dequeue the signals(in ascending signal number order)
                               // exit(EXIT_SUCCESS);
                           }
                       });

    std::thread th([ios]()
                   { ios->run(); });

    std::this_thread::sleep_for(std::chrono::seconds(10));

    std::cout << "Finish" << std::endl;

    ios->stop();

    th.join();

    return 0;
}