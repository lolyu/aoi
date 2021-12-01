#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include <boost/thread.hpp>
#include <thread>

void workerFunc(boost::asio::io_service &ios)
{
    ios.run();
}

void workerFuncErrored(boost::asio::io_service &ios)
{
    throw boost::system::system_error(boost::system::error_code(), "HELLOWORLD");
}

int main()
{
    boost::asio::io_service ios;
    std::shared_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(ios));

    // boost::thread_group threads0;
    std::vector<std::thread> threads1;
    threads1.push_back(std::thread([&]()
                                   { workerFunc(ios); }));
    threads1.push_back(std::thread([&]()
                                   { workerFunc(ios); }));
    threads1.push_back(std::thread([&]()
                                   { workerFunc(ios); }));
    threads1.push_back(std::thread([&]()
                                   { workerFunc(ios); }));
    threads1.push_back(std::thread([&]()
                                   { workerFuncErrored(ios); }));

    std::this_thread::sleep_for(std::chrono::seconds(2));
    ios.stop();
    for (auto &th : threads1)
    {
        th.join();
    }

    // workerFuncErrored(ios);

    return 0;
}