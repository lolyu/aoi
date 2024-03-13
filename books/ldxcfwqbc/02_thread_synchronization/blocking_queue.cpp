#include <iostream>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <cassert>
#include <functional>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

template <typename T>
class BlockingQueue
{
public:
    BlockingQueue() = default;
    BlockingQueue(const BlockingQueue &) = delete;
    BlockingQueue &operator=(const BlockingQueue &) = delete;

    void put(const T &item)
    {
        {
            unique_lock<mutex> lock(_mutex);
            _items.push_back(item);
        }
        _cond.notify_one();
    }

    T pop()
    {
        unique_lock<mutex> lock(_mutex);
        _cond.wait(lock, [this] () { return !_items.empty(); });
        assert(!_items.empty());
        T t = _items.front();
        _items.pop_front();
        return t;
    }

    size_t size() const
    {
        unique_lock<mutex> lock(_mutex);
        return _items.size();
    }

private:
    mutable mutex _mutex;
    condition_variable _cond;
    deque<T> _items;
};

template <typename T>
class BoundedBlockingQueue
{
public:
    BoundedBlockingQueue(size_t max_size = 10) :  _max_size(max_size) {};
    BoundedBlockingQueue(const BoundedBlockingQueue &) = delete;
    BoundedBlockingQueue &operator=(const BoundedBlockingQueue &) = delete;

    void put(const T &item)
    {
        {
            unique_lock<mutex> lock(_mutex);
            _not_full.wait(lock, [this] { return _items.size() < _max_size; });
            _items.push_back(item);
        }
        _not_empty.notify_one();
    }

    T pop()
    {
        T t;
        {
            unique_lock<mutex> lock(_mutex);
            _not_empty.wait(lock, [this] { return _items.size() > 0; });
            t = _items.pop_front();
        }
        _not_full.notify_one();
        return t;
    }

    size_t size() const
    {
        unique_lock<mutex> lock(_mutex);
        return _items.size();
    }

    bool empty() const
    {
        unique_lock<mutex> lock(_mutex);
        return _items.empty();
    }

    bool full() const
    {
        unique_lock<mutex> lock(_mutex);
        return _items.full();
    }

    size_t capacity() const
    {
        unique_lock<mutex> lock(_mutex);
        return _items.capacity();
    }
private:
    mutex _mutex;
    condition_variable _not_empty;
    condition_variable _not_full;
    deque<T> _items;
    size_t _max_size;
};

typedef function<void()> Functor;
BlockingQueue<Functor> task_queue;
atomic_bool running;

void worker_thread()
{
    while (running.load(memory_order_relaxed))
    {
        Functor task = task_queue.pop();
        task();
    }
    cout << "Thread " << this_thread::get_id() << " exits." << endl;
}

void print_function(int i)
{
    cout << i << endl;
}

int main()
{
    running.store(true, memory_order_relaxed);

    vector<thread> threads;

    for (int i = 0; i < 5; ++i)
    {
        threads.push_back(thread(worker_thread));
    }

    for (int i = 0; i < 10; ++i)
    {
        task_queue.put(bind(print_function, i));
    }

    while (task_queue.size() > 0)
    {
        this_thread::sleep_for(chrono::seconds(1));
    }

    running.store(false, memory_order_relaxed);

    for (int i = 0; i < 5; ++i)
    {
        task_queue.put(bind(print_function, i + 100));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    return 0;
}
