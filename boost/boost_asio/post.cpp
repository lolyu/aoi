#include <boost/asio.hpp>
#include <memory>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <mutex>
#include <vector>

std::mutex global_stream_lock;

void workFunc(boost::asio::io_service &ios, int counter)
{
    std::unique_lock<std::mutex> lock(global_stream_lock);
    std::cout << counter << std::endl;
    lock.unlock();

    ios.run();

    lock.lock();
    std::cout << counter << std::endl;
    lock.unlock();
}

size_t fact(size_t n)
{
    if (n <= 1)
    {
        return n;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return n * fact(n - 1);
}

void getFactorial(size_t n)
{
    std::unique_lock<std::mutex> lock(global_stream_lock);
    std::cout << "Calculating " << n << "! factorial" << std::endl;
    lock.unlock();

    size_t f = fact(n);

    lock.lock();
    std::cout << n << "! = " << f << std::endl;
    lock.unlock();
}

int main()
{
    boost::asio::io_service ios;
    std::unique_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(ios));
    std::unique_lock<std::mutex> lock(global_stream_lock);
    std::cout << "Start!" << std::endl;
    lock.unlock();

    std::vector<std::thread> threads;
    for (int i = 0; i < 5; ++i)
    {
        threads.push_back(std::move(std::thread(boost::bind(workFunc, boost::ref(ios), i))));
    }
    ios.post(boost::bind(getFactorial, 5));
    ios.post(boost::bind(getFactorial, 6));
    ios.post(boost::bind(getFactorial, 7));

    std::this_thread::sleep_for(std::chrono::seconds(10));

    work.reset();

    for (auto &th : threads)
    {
        th.join();
    }

    return 0;
}