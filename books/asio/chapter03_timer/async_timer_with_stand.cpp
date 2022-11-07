#include <iostream>
#include <memory>
#include <vector>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#define CONCURRENCY 5

class Printer
{
public:
    Printer(boost::asio::io_service &ios, uint timer_count = 2)
        : ios(ios), strand(ios)
    {
        for (size_t index = 0; index < timer_count; ++index)
        {
            timers.push_back(boost::asio::steady_timer(ios, boost::asio::chrono::seconds(1)));
        }
        for (size_t index = 0; index < timer_count; ++index)
        {
            timers[index].async_wait(
                boost::asio::bind_executor(
                    strand,
                    [this, index](const boost::system::error_code &error)
                    { print(index); }));
            // timers[index].async_wait(
            //     [this, index](const boost::system::error_code &error)
            //     { print(index); });
        }
    }

    void print(size_t index)
    {
        if (count < 10)
        {
            std::cout << "Timer " << index << ": " << count++ << std::endl;
            timers[index].expires_after(boost::asio::chrono::seconds(1));
            timers[index].async_wait(
                boost::asio::bind_executor(
                    strand,
                    [this, index](const boost::system::error_code &error)
                    { print(index); }));
            // timers[index].async_wait(
            //     [this, index](const boost::system::error_code &error)
            //     { print(index); });
        }
    }

private:
    boost::asio::io_service &ios;
    boost::asio::io_service::strand strand;
    std::vector<boost::asio::steady_timer> timers;
    int count = 0;
};

int main()
{
    boost::asio::io_service ios;

    auto work = std::make_unique<boost::asio::io_service::work>(ios);
    boost::thread_group threads;
    for (int i = 0; i < CONCURRENCY; ++i)
    {
        threads.create_thread([&ios]()
                              { ios.run(); });
    }

    Printer printer(ios);

    work.reset();
    threads.join_all();

    return 0;
}
