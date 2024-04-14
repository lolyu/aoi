#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdio>
#include <atomic>
#include <chrono>

using namespace std;

mutex m;
condition_variable cv;
bool a_finish = false;
bool b_finish = false;

void a() {
    unique_lock<mutex> ul(m);
    this_thread::sleep_for(chrono::milliseconds(300));
    printf("1\n");

    a_finish = true;
    ul.unlock();
    cv.notify_all();
}

void b() {
    unique_lock<mutex> ul(m);
    this_thread::sleep_for(chrono::milliseconds(200));
    cv.wait(ul, [](){ return a_finish; });
    printf("2\n");
    
    b_finish = true;
    ul.unlock();
    cv.notify_all();
}

void c() {
    unique_lock<mutex> ul(m);
    this_thread::sleep_for(chrono::milliseconds(100));
    cv.wait(ul, [](){ return b_finish; });
    printf("3\n");

    b_finish = true;
    ul.unlock();
    cv.notify_all();
}

int main() {
    std::thread at(a);
    std::thread bt(b);
    std::thread ct(c);

    ct.join();
    bt.join();
    at.join();
}
