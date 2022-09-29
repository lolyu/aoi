#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    // steady_clock is a stop watch
    auto start0 = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto end0 = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed0 = end0 - start0;
    std::cout << elapsed0.count() << std::endl;

    // system_clock returns the system clock
    auto start1 = std::chrono::system_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto end1 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed1 = end1 - start1;
    std::cout << elapsed1.count() << std::endl;

    // high_resolution_clock is a high-resolution-version stop clock
    auto start2 = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(2));
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed2 = end2 - start2;
    std::cout << elapsed2.count() << std::endl;

    return 0;
}
