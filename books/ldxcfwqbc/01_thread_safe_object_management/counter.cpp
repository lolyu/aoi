#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

class Counter
{
public:
    Counter() : m_value(0) {}
    Counter(Counter &) = delete;
    Counter &operator=(const Counter &) = delete;

    int value();
    int increase();

private:
    int m_value;
    std::mutex m_mutex;
};

int Counter::value()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_value;
}

int Counter::increase()
{
    // std::lock_guard<std::mutex> lock(m_mutex);
    return m_value = m_value + 1;
}

Counter g_counter;

void f()
{
    for (int i = 0; i < 100000; ++i)
    {
        g_counter.increase();
    }
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i)
    {
        threads.push_back(std::thread(f));
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    std::cout << g_counter.value() << std::endl;
    return 0;
}
