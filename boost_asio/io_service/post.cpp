#include <boost/asio.hpp>
#include <iostream>
#include <boost/bind.hpp>

size_t fact(size_t n);

int main()
{
    size_t finished;
    boost::asio::io_service ios;
    // boost::asio::io_service::work work(ios);
    ios.post(boost::bind(fact, 2));
    ios.post(boost::bind(fact, 3));
    ios.post(boost::bind(fact, 4));
    finished = ios.run_one();
    std::cout << finished << std::endl;
    std::cout << (ios.stopped() ? "STOPPED" : "NOT STOPPED") << std::endl;
    finished = ios.run_one();
    std::cout << finished << std::endl;
    std::cout << (ios.stopped() ? "STOPPED" : "NOT STOPPED") << std::endl;
    finished = ios.run_one();
    std::cout << finished << std::endl;
    std::cout << (ios.stopped() ? "STOPPED" : "NOT STOPPED") << std::endl;
    finished = ios.run_one();
    std::cout << finished << std::endl;
    std::cout << (ios.stopped() ? "STOPPED" : "NOT STOPPED") << std::endl;
    return 0;
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