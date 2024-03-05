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

void worker_thread(Latch &latch)
{
    cout << "worker thread: " << this_thread::get_id() << " starts" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    latch.count_down();
    cout << "worker thread: " << this_thread::get_id() << " ends" << endl;
}

int main()
{
    Latch lt(3);
    thread(worker_thread, ref(lt)).detach();
    thread(worker_thread, ref(lt)).detach();
    thread(worker_thread, ref(lt)).detach();
    lt.wait();
    return 0;
}
