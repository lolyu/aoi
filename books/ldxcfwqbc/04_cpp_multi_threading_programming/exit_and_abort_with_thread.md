# exit and abort with thread
* never call `exit` and `abort` in a mult-threaded process, because those two calls will leave the threads in a random state.
    * always join the threads before main thread returns/exits

## exit
* `std::exit` terminates a process but with some cleanups performed:
    1. non-local static objects are destroyed
    2. `std::atexit` handlers are called
    3. current thread thread-local variables are destroyed
* **NOTE**: all the cleanup procedures are called inside the thread calling `exit`

```cppp
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

std::mutex g_mutex;

class Job
{
public:
    Job() : pi(new int)
    {
    }
    ~Job()
    {
        // std::lock_guard<std::mutex> lock(g_mutex);
        std::cout << "Job destructor called inside thread " << std::this_thread::get_id() << std::endl;
        delete pi;
    }
    Job(const Job &) = delete;
    Job &operator=(const Job &) = delete;
    void do_task()
    {
        // std::lock_guard<std::mutex> lock(g_mutex);
        exit(100);
    }
private:
    int *pi = nullptr;
};

Job g_job;

void do_task()
{
    std::cout << "Task thread : " << std::this_thread::get_id() << std::endl;
    g_job.do_task();
}

int main()
{
    std::cout << "Exit a mutl-threaded program" << std::endl;
    std::cout << "main thread: " << std::this_thread::get_id() << std::endl;
    std::thread thread1(do_task);
    thread1.join();
    return 0;
}
```
* output for the above code:
```
Exit a mutl-threaded program
main thread: 140522879121216
Task thread : 140522879117056
Job destructor called inside thread 140522879117056
```



## abort
* terminates the process with a `SIGABRT` signal.

## references
* https://en.cppreference.com/w/cpp/utility/program/exit
* https://en.cppreference.com/w/cpp/utility/program/abort
