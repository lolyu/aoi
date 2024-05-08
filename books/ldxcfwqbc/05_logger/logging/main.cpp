#include "AsyncLogging.h"
#include "LogFile.h"
#include "LogStream.h"

#include <iostream>
#include <sstream>
#include <thread>

const int rotate_size = 500*1000*1000;

template <typename ASYNC>
void bench(ASYNC *log)
{
    log->start();

    int cnt = 0;
    const int batch = 1000;
    const bool long_log = true;
    std::string emptystr = " ";
    std::string longstr(3000, 'X');
    longstr += " ";

    for (int t = 0; t < 30; ++t)
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < batch; ++i)
        {
            std::stringstream ss;
            ss << "Hello 0123456789"
               << " abcdefghijklmnopqrstuvwxyz "
               << (long_log ? longstr : emptystr)
               << cnt++
               << std::endl;
            log->append(ss.str().c_str(), ss.str().length());
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> fsec = end - start;
        std::cout << fsec.count() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main()
{
    AsyncLogging log("async_logging", rotate_size);
    bench(&log);
    return 0;
}