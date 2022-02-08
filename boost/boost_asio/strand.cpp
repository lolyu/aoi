#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <boost/bind/bind.hpp>
#include <iostream>

std::mutex global_stream_lock;

/*
io_service::strand::wrap

if the wrapped function is posted by io_service, and when the wrapped function is executed by one of the work thread, it has the
same effect as called by the strand's dispatch
*/

void workFunc(std::shared_ptr<boost::asio::io_service> ios, int counter)
{
    std::unique_lock<std::mutex> lock(global_stream_lock);
    std::cout << "Start thread " << counter << std::endl;
    lock.unlock();

    ios->run();

    lock.lock();
    std::cout << "Stop thread " << counter << std::endl;
    lock.unlock();
}

void print(int number)
{
    std::cout << number;
}

int main()
{
    std::shared_ptr<boost::asio::io_service> ios(new boost::asio::io_service);
    std::unique_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*ios));
    std::unique_ptr<boost::asio::io_service::strand> strand(new boost::asio::io_service::strand(*ios));

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i)
    {
        threads.push_back(std::move(std::thread(boost::bind(workFunc, ios, i))));
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    ios->post(strand->wrap(boost::bind(print, 1)));
    ios->post(strand->wrap(boost::bind(print, 2)));
    ios->post(boost::bind(print, 3));
    ios->post(boost::bind(print, 4));

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    ios->post(boost::bind(print, 5));
    ios->post(boost::bind(print, 6));
    ios->post(strand->wrap(boost::bind(print, 7)));
    ios->post(strand->wrap(boost::bind(print, 8)));

    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    ios->post(strand->wrap(boost::bind(print, 9)));
    ios->post(strand->wrap(boost::bind(print, 10)));

    work.reset();

    for (auto &th : threads)
    {
        th.join();
    }

    return 0;
}