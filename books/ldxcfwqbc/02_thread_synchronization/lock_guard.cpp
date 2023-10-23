#include <functional>
#include <iostream>
#include <mutex>
#include <string_view>
#include <syncstream>
#include <thread>

volatile int g_i = 0;
std::mutex g_i_mutex;

void safe_increment(int iterations)
{
    const std::lock_guard<std::mutex> lock(g_i_mutex);
    for (int i = 0; i < iterations; ++i, ++g_i);
    std::cout << "thread #" << std::this_thread::get_id() << ", g_i" << g_i << std::endl;
}

void unsafe_increment(int iterations)
{
    for (int i = 0; i < iterations; ++i, ++g_i);
    std::osyncstream(std::cout) << "thread #" << std::this_thread::get_id() << ", g_i" << g_i << std::endl;
}


int main()
{
    auto test = [](std::string_view func_name, std::function<void (int)> func)
    {
        g_i = 0;
        std::cout << func_name << ":" << std::endl << "before, g_i: " << g_i << std::endl;
        {
            std::thread t0(func, 1000000);
            std::thread t1(func, 1000000);
        }
    };

    test("safe_increment", safe_increment);
    test("unsafe_increment", unsafe_increment);

    return 0;
}
