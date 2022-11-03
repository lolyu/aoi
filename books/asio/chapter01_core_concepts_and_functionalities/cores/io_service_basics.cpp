#include <memory>
#include <chrono>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

void worker_func(boost::asio::io_service &ios)
{
    for (;;)
    {
        try
        {
            ios.run();
            break;
        }
        catch (const std::exception &ex)
        {
            std::cout << "Got exception " << ex.what() << " in thread " << std::this_thread::get_id() << std::endl;
            continue;
        }
    }
}

void dispatch_handler(boost::asio::io_service &ios, int count)
{
    std::cout << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(100));

    if (count > 0)
    {
        ios.dispatch([&ios, count]()
                     { dispatch_handler(ios, count - 1); });
        ios.dispatch([&ios, count]()
                     { dispatch_handler(ios, count - 1); });
    }
}

void post_handler(boost::asio::io_service &ios, int count)
{
    std::cout << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::microseconds(100));

    if (count > 0)
    {
        ios.post([&ios, count]()
                 { post_handler(ios, count - 1); });
        ios.post([&ios, count]()
                 { post_handler(ios, count - 1); });
    }
}

int main()
{
    boost::asio::io_service ios;
    auto work = std::make_unique<boost::asio::io_service::work>(ios);

    boost::thread_group threads;
    for (int i = 0; i < 5; ++i)
    {
        threads.create_thread([&]()
                              { worker_func(ios); });
    }
    std::cout << "The main thread " << std::this_thread::get_id() << std::endl;

    // both io_service::dispatch and io_service::post will guarantee the handler will
    // only be executed in a thread in which `run()`, `run_one()`, `poll()` or `poll_one()`
    // is currently being invoked.

    // but io_service::dispatch might execute the handler right away, if the current thread
    // invokes `run()`, `run_one()`, `poll()`, or `poll_one()`.
    auto start = std::chrono::steady_clock::now();
    ios.dispatch([&ios]()
                 { dispatch_handler(ios, 3); });
    auto end = std::chrono::steady_clock::now();
    std::cout << "Time spent with dispatch: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(4));

    // ios_service::post always queue the handler, no allowing execute the handler right
    // away
    start = std::chrono::steady_clock::now();
    ios.post([&ios]()
             { post_handler(ios, 3); });
    end = std::chrono::steady_clock::now();
    std::cout << "Time spent with post: ";
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << std::endl;

    // io_service::stop() will stop the service immediately, any queued handlers will be dropped
    // ios.stop();
    work.reset();
    // call the destructor of io_service::work could stop the service gracefully, allowing queued
    // handlers to be finished
    threads.join_all();
    return 0;
}
