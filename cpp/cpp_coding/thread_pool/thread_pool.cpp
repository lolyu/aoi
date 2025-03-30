#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <functional>
#include <vector>
#include <chrono>

/*
Key takeaways:
1. thread primitives
2. mutex, lock
3. conditional variable
*/

template <typename T>
class MPMCBlockingQUeue
{
public:
    MPMCBlockingQUeue(bool nonblock=false) : _nonblock(nonblock) {}

    template <typename U>
    void push(U &&value)
    {
        std::lock_guard<std::mutex> lock(_producer_mutex);
        _producer_queue.push(std::forward<U>(value));
        _not_empty.notify_one();
    }

    bool pop(T &value)
    {
        std::unique_lock<std::mutex> lock(_consumer_mutex);
        _not_empty.wait(lock, [this]() { return !_consumer_queue.empty() || swap_queue() || _nonblock; });
        if (_consumer_queue.empty())
        {
            return false;
        }
        value = std::move(_consumer_queue.front());
        _consumer_queue.pop();
        return true;
    }

    void cancel()
    {
        std::lock_guard<std::mutex> lock(_producer_mutex);
        _nonblock = true;
        _not_empty.notify_all();
    }

private:
    bool swap_queue()
    {
        std::unique_lock<std::mutex> lock(_producer_mutex);
        std::swap(_producer_queue, _consumer_queue);
        return _consumer_queue.size();
    }

private:
    std::mutex _producer_mutex;
    std::mutex _consumer_mutex;
    std::queue<T> _producer_queue;
    std::queue<T> _consumer_queue;
    bool _nonblock;
    std::condition_variable _not_empty;
};

template <typename T>
class SPMCBlockingQueue
{
public:
    SPMCBlockingQueue(bool nonblock=false) : _nonblock(nonblock) {}

    bool pop(T &value)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _not_empty.wait(lock, [this]() { return !_queue.empty() || _nonblock; });
        if (_queue.empty())
        {
            return false;
        }
        value = std::move(_queue.front());
        _queue.pop();
        return true;
    }

    template <typename U>
    void push(U &&value)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(std::forward<U>(value));
        _not_empty.notify_one();
    }

    void cancel()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _nonblock = true;
        _not_empty.notify_all();
    }

private:
    std::mutex _mutex;
    std::condition_variable _not_empty;
    std::queue<T> _queue;
    bool _nonblock = false;
};

class ThreadPool
{
public:
    explicit ThreadPool(size_t num_threads)
    {
        for (int i = 0; i < num_threads; ++i)
        {
            _workers.emplace_back([this]() { worker(); });
        }
    }

    ~ThreadPool()
    {
        _tasks.cancel();
        join_all();
    }

    void join_all()
    {
        _tasks.cancel();
        for (auto &worker : _workers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

    template <typename F, typename... Args>
    void submit(F &&func, Args &&...args)
    {
        auto task = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
        _tasks.push(task);
    }

private:
    void worker()
    {
        while (true)
        {
            std::function<void ()> task;
            if (!_tasks.pop(task))
            {
                break;
            }
            task();
        }
    }

private:
    MPMCBlockingQUeue<std::function<void ()>> _tasks;
    std::vector<std::thread> _workers;
};

void sleep_for(size_t t)
{
    std::cout << std::this_thread::get_id() << " sleep for " << t << " seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(t));
}

int main()
{
    ThreadPool thread_pool(2);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 1; i < 10; ++i)
    {
        thread_pool.submit(sleep_for, i);
    }
    thread_pool.join_all();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << std::endl;
    return 0;
}
