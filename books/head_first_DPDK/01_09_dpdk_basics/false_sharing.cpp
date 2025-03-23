#include <iostream>
#include <thread>
#include <new>
#include <atomic>
#include <chrono>
// #include <latch>
#include <vector>

#define CL_SIZE         64

// 1: 2
// 2: 3
// 3: 6
// 4: 7
// 5: 8
// 6: 10
// 7: 12
// 8: 13
// 9: 14
// 10: 16
// 11: 17
// 12: 20
// 13: 23
// 14: 27
// 15: 28
// 16: 29

int main()
{
    std::vector<std::thread> threads;
    int hc = std::thread::hardware_concurrency();
    hc = hc <= CL_SIZE ? hc : CL_SIZE;
    for (int thread_count = 1; thread_count <= hc; ++thread_count)
    {
        // std::latch lsync(thread_count);
        std::atomic<uint> fsync(thread_count);
        struct alignas(CL_SIZE) { char shared[CL_SIZE]; } cacheline;
        std::atomic<int64_t> sum(0);

        for (int t = 0; t != thread_count; ++t)
        {
            threads.emplace_back(
                [&](char volatile &c)
                {
                    // lsync.arrive_and_wait();
                    if (fsync.fetch_sub(1, std::memory_order::memory_order_relaxed) != 1)
                    {
                        while (fsync.load(std::memory_order::memory_order_relaxed));
                    }

                    auto start = std::chrono::high_resolution_clock::now();
                    for(size_t r = 10000000; r--;)
                    {
                        c = c + 1;
                    }
                    sum += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count();
                },
                std::ref(cacheline.shared[t])
            );
        }
        for (auto &thrd : threads)
        {
            thrd.join();
        }
        threads.resize(0);
        std::cout << thread_count << ": " << (int)(sum / (1.0e7 * thread_count) + 0.5) << std::endl;
    }

    return 0;
}
