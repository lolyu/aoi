#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <string>
#include <functional>
#include <chrono>

using namespace std;

class Latch
{
public:
    Latch(size_t count) : _count(count) {}
    Latch(const Latch &) = delete;
    Latch &operator=(const Latch &) = delete;

    void count_down()
    {
        unique_lock<mutex> lock(_mutex);
        --_count;
        if (_count == 0)
        {
            _cv.notify_all();
        }
    }

    void wait()
    {
        unique_lock<mutex> lock(_mutex);
        while (_count > 0)
        {
            _cv.wait(lock);
        }
    }

private:
    size_t _count;
    condition_variable _cv;
    mutex _mutex;
};

struct Job
{
    const string name;
    string product{"not worked"};
    thread action{};
};

int main()
{
    Job jobs[]{{"Annika"}, {"Buru"}, {"Chuck"}};
    Latch work_done{3};
    Latch start_clean_up{1};

    auto work = [&](Job& my_job)
    {
        my_job.product = my_job.name + " worked";
        work_done.count_down();
        start_clean_up.wait();
        my_job.product = my_job.name + " cleaned";
    };

    std::cout << "Work is starting... ";
    for (auto &job : jobs)
    {
        job.action = thread(work, ref(job));
    }

    work_done.wait();
    std::cout << "done:\n";
    for (auto const& job : jobs)
        std::cout << "  " << job.product << '\n';

    std::cout << "Workers are cleaning up... ";
    start_clean_up.count_down();
    for (auto &job : jobs)
    {
        job.action.join();
    }

    std::cout << "done:\n";
    for (auto const& job : jobs)
    {
        std::cout << "  " << job.product << '\n';
    }

    return 0;
}
