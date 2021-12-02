#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <vector>

std::mutex stdout_lock;
/*
post vs dispatch
post:
This function is used to ask the io_context to execute the given handler, but without allowing the io_context to call the handler from inside this function.

dispatch:
This function is used to ask the io_context to execute the given handler,
The handler may be executed inside this function if the guarantee can be met.
the guarantee is that the caller of dispatch is running by one of the worker thread(call io_service::run)

that is to say, if we have a few threads call io_service::run, assign jobs to the worker threads with either post() or dispatch() from the main thread
have the same behavior(the jobs are not guarantee to run immediately because the main thread is not the io_service worker thread)
*/

void workFunc(std::shared_ptr<boost::asio::io_service> ios)
{
    std::unique_lock<std::mutex> lock(stdout_lock);
    std::cout << "Start thread" << std::endl;
    lock.unlock();

    ios->run();

    lock.lock();
    std::cout << "Stop thread" << std::endl;
    lock.unlock();
}

void dispatch(int i)
{
    std::unique_lock<std::mutex> lock(stdout_lock);
    std::cout << "dispatch " << i << std::endl;
    lock.unlock();
}

void post(int i)
{
    std::unique_lock<std::mutex> lock(stdout_lock);
    std::cout << "post " << i << std::endl;
    lock.unlock();
}

void run(std::shared_ptr<boost::asio::io_service> ios)
{
    for (int i = 0; i < 5; ++i)
    {
        ios->dispatch(boost::bind(dispatch, i));
        ios->post(boost::bind(post, i));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    std::shared_ptr<boost::asio::io_service> ios(new boost::asio::io_service);
    std::unique_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(*ios));
    std::thread th(boost::bind(workFunc, ios));
    ios->post(boost::bind(run, ios));

    work.reset();

    th.join();
    return 0;
}