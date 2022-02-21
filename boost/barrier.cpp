#include <iostream>
#include <boost/thread.hpp>
#include <boost/thread/barrier.hpp>
#include <boost/bind.hpp>
#include <boost/atomic.hpp>

boost::mutex io_mutex;

void thread_fun(boost::barrier &cur_barier, boost::atomic<int> &current)
{
    ++current;
    cur_barier.wait();
    boost::lock_guard<boost::mutex> locker(io_mutex);
    std::cout << current << std::endl;
}

int main()
{
    boost::barrier bar(3);
    boost::atomic<int> current(0);
    boost::thread thr1(boost::bind(&thread_fun, boost::ref(bar), boost::ref(current)));
    boost::thread thr2(boost::bind(&thread_fun, boost::ref(bar), boost::ref(current)));
    boost::thread thr3(boost::bind(&thread_fun, boost::ref(bar), boost::ref(current)));
    thr1.join();
    thr2.join();
    thr3.join();
}