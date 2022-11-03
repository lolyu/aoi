#include <memory>
#include <chrono>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

void handler(boost::asio::io_service::strand &strand, int count);

int main()
{
    boost::asio::io_service ios;
    boost::asio::io_service::strand strand(ios);
    auto work = std::make_unique<boost::asio::io_service::work>(ios);

    boost::thread_group threads;
    threads.create_thread([&]()
                          { ios.run(); });
    threads.create_thread([&]()
                          { ios.run(); });
    threads.create_thread([&]()
                          { ios.run(); });
    threads.create_thread([&]()
                          { ios.run(); });
    threads.create_thread([&]()
                          { ios.run(); });

    ios.post(
        [&strand]()
        { handler(strand, 3); });

    work.reset();
    threads.join_all();
    return 0;
}

void handler(boost::asio::io_service::strand &strand, int count)
{
    std::cout << "handler running in thread " << std::this_thread::get_id() << std::endl;
    std::cout << "thread " << std::this_thread::get_id() << " sleep for 500ms" << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(500));

    if (count > 0)
    {
        boost::asio::io_service &ios = strand.context();
        // ios.post(
        //     boost::asio::bind_executor(
        //         strand,
        //         [&strand, count]()
        //         { handler(strand, count - 1); }));
        // ios.post(
        //     boost::asio::bind_executor(
        //         strand,
        //         [&strand, count]()
        //         { handler(strand, count - 1); }));
        // ios.post(
        //     boost::asio::bind_executor(
        //         strand,
        //         [&strand, count]()
        //         { handler(strand, count - 1); }));
        boost::asio::post(strand, [&strand, count]()
                          { handler(strand, count - 1); });
        boost::asio::post(strand, [&strand, count]()
                          { handler(strand, count - 1); });
        boost::asio::post(strand, [&strand, count]()
                          { handler(strand, count - 1); });
    }
}
