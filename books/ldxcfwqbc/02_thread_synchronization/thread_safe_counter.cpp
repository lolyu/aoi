#include <thread>
#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <vector>
#include <utility>

std::mutex _io_lock;

class Counter
{
public:
    Counter() = default;

    unsigned int get() const
    {
        std::shared_lock<std::shared_mutex> lock(_rw_lock);
        return _value;
    }

    void increment()
    {
        std::unique_lock<std::shared_mutex> lock(_rw_lock);
        ++_value;
    }

    void reset()
    {
        std::unique_lock<std::shared_mutex> lock(_rw_lock);
        _value = 0;
    }

private:
    mutable std::shared_mutex _rw_lock;
    unsigned int _value = 0;
};

int main()
{
    Counter counter;
 
    std::vector<std::pair<std::thread::id, unsigned int>> vs;
    auto increment_and_get = [&counter, &vs]()
    {
        for (int i{}; i != 3; ++i)
        {
            counter.increment();
            vs.emplace_back(std::this_thread::get_id(), counter.get());
        }
    };
 
    std::thread thread1(increment_and_get);
    std::thread thread2(increment_and_get);
 
    thread1.join();
    thread2.join();

    for (auto &p : vs)
    {
        std::cout << p.first << ": " << p.second << std::endl;
    }

    return 0;
}
