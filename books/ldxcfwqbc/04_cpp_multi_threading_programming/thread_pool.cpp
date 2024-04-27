#include <iostream>
#include <utility>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <atomic>
#include <ctime>

class ThreadPool
{
public:
    ThreadPool(ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &) = delete;
public:
    using Task = std::function<void ()>;
public:
    explicit ThreadPool(const std::string name = std::string("ThreadPool")) : _name(name)
    {
        set_max_queue_size(4);
    }

    ~ThreadPool()
    {
        if (_running)
        {
            stop();
        }
    }

    void set_max_queue_size(size_t max_size)
    {
        _max_size = max_size;
    }
    void set_thread_init_callback(const Task &t) { _init_callback = t; }

    void start(int num_threads)
    {
        assert(_threads.empty());
        _running = true;
        _threads.resize(num_threads);
        for (int i = 0; i < num_threads; ++i)
        {
            _threads[i].reset(new std::thread(std::bind(&ThreadPool::run_in_thread, this)));
        }
        if (num_threads == 0 && _init_callback)
        {
            _init_callback();
        }
    }

    void stop()
    {
        _running = false;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _not_empty.notify_all();
            _not_full.notify_all();
        }

        for (auto &thread : _threads)
        {
            thread->join();
        }
    }

    const std::string &name() const { return _name; }
    size_t max_size() const { return _max_size; }
    size_t size() const
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _tasks.size();
    }

    void run(Task task)
    {
        if (_threads.empty())
        {
            task();
        }
        else
        {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                while (is_full() && _running)
                {
                    _not_full.wait(lock);
                }
                // the wakeup could be due to either task pop event
                // or stop event
                if (!_running)
                {
                    return;
                }
                _tasks.push_back(std::move(task));
            }
            _not_empty.notify_one();
        }
    }

private:
    bool is_full() const
    {
        return _tasks.size() == _max_size;
    }

    bool is_empty() const
    {
        return _tasks.size() == 0;
    }

    Task get_task()
    {
        Task t;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while (is_empty() && _running)
            {
                _not_empty.wait(lock);
            }
            // the wakeup could be due to either new task event or
            // stop event
            if (!is_empty())
            {
                t = _tasks.front();
                _tasks.pop_front();
                _not_full.notify_one();
            }
        }
        return t;
    }

    void run_in_thread()
    {
        try
        {
            if (_init_callback)
            {
                _init_callback();
            }
            while (_running)
            {
                Task t = get_task();
                if (!_running)
                {
                    break;
                }
                if (t)
                {
                    std::cout << time(0) << std::endl;
                    t();
                    std::cout << time(0) << std::endl;
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "Exception caught in ThreadPool " << _name << std::endl;
            std::cerr << "Reason: " << e.what() << std::endl;
            abort();
        }
    }

private:
    std::string _name;
    mutable std::mutex _mutex;
    std::condition_variable _not_empty;
    std::condition_variable _not_full;
    std::deque<Task> _tasks;
    std::vector<std::unique_ptr<std::thread>> _threads;
    size_t _max_size = 0;
    std::atomic<bool> _running{false};
    Task _init_callback;
};

void f(int i)
{
    std::this_thread::sleep_for(std::chrono::seconds(i));
    std::cout << std::this_thread::get_id() << ": " << i << std::endl;
}

int main()
{
    ThreadPool tp("helloworld");
    tp.start(2);
    tp.run(std::bind(f, 1));
    tp.run(std::bind(f, 2));
    tp.run(std::bind(f, 3));
    tp.run(std::bind(f, 4));
    std::this_thread::sleep_for(std::chrono::seconds(10));
    tp.stop();
    return 0;
}
