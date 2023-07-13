#include <thread>
#include <vector>
#include <atomic>
#include <iostream>

class spinlock;
std::atomic_flag lock = ATOMIC_FLAG_INIT;

void f(int n)
{
    for (int cnt = 0; cnt < 40; ++cnt)
    {
        while (lock.test_and_set(std::memory_order_acquire))
        {
            ;
        }
        static int out{};
        std::cout << n << ((++out % 40) == 0 ? '\n' : ' ');
        lock.clear(std::memory_order_release);
    }
}

class spinlock
{
public:
    spinlock() : _flag(ATOMIC_FLAG_INIT) {}
    spinlock(spinlock &) = delete;
    spinlock &operator=(const spinlock &) = delete;
    ~spinlock() = default;
    void lock()
    {
        while (_flag.test_and_set(std::memory_order_acquire))
            ;
    }
    void unlock()
    {
        _flag.clear(std::memory_order_release);
    }

private:
    std::atomic_flag _flag;
};

spinlock slock;

void f_with_slock(int n)
{
    for (int cnt = 0; cnt < 40; ++cnt)
    {
        slock.lock();
        static int out{};
        std::cout << n << ((++out % 40) == 0 ? '\n' : ' ');
        slock.unlock();
    }
}

int main()
{
    std::vector<std::thread> threads;
    for (int n = 0; n < 10; ++n)
    {
        // threads.emplace_back(f, n);
        threads.emplace_back(f_with_slock, n);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }
    return 0;
}
