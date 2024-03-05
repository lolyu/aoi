#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <string>

using namespace std;

mutex m;
condition_variable cv;
string data;
bool ready = false;
bool processed = false;

void worker_thread()
{
    unique_lock<mutex> ul(m);
    while (!ready)
    {
        cv.wait(ul);
    }
    // cv.wait(ul, []()
    //         { return ready; });

    cout << "thread processes data" << endl;

    data = "processed data";

    processed = true;
    ul.unlock();
    cv.notify_all();
}

int main()
{
    thread worker(worker_thread);

    data = "raw data";
    {
        unique_lock<mutex> ul(m);
        ready = true;
    }
    cv.notify_all();

    {
        unique_lock<mutex> ul(m);
        while (!processed)
        {
            cv.wait(ul);
        }
    }

    worker.join();

    return 0;
}
