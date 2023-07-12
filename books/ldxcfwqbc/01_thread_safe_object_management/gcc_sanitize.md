# gcc sanitize options

## thread sanitizer
* thread sanitizer is used to detect race condition
```cpp
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
```
* compile it with `-fsanitize=thread` and run, will have the following output:
```
WARNING: ThreadSanitizer: data race (pid=3359678)
  Read of size 4 at 0x558f3a8ac160 by thread T2:
    #0 Counter::increase() <null> (a.out+0x23de)
    #1 f() <null> (a.out+0x244c)
    #2 void std::__invoke_impl<void, void (*)()>(std::__invoke_other, void (*&&)()) <null> (a.out+0x5208)
    #3 std::__invoke_result<void (*)()>::type std::__invoke<void (*)()>(void (*&&)()) <null> (a.out+0x514b)
    #4 void std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) <null> (a.out+0x50a0)
    #5 std::thread::_Invoker<std::tuple<void (*)()> >::operator()() <null> (a.out+0x5042)
    #6 std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run() <null> (a.out+0x4ff4)
    #7 <null> <null> (libstdc++.so.6+0xd6de3)

  Previous write of size 4 at 0x558f3a8ac160 by thread T1:
    #0 Counter::increase() <null> (a.out+0x23f3)
    #1 f() <null> (a.out+0x244c)
    #2 void std::__invoke_impl<void, void (*)()>(std::__invoke_other, void (*&&)()) <null> (a.out+0x5208)
    #3 std::__invoke_result<void (*)()>::type std::__invoke<void (*)()>(void (*&&)()) <null> (a.out+0x514b)
    #4 void std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) <null> (a.out+0x50a0)
    #5 std::thread::_Invoker<std::tuple<void (*)()> >::operator()() <null> (a.out+0x5042)
    #6 std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run() <null> (a.out+0x4ff4)
    #7 <null> <null> (libstdc++.so.6+0xd6de3)

  Location is global 'g_counter' of size 48 at 0x558f3a8ac160 (a.out+0x000000009160)

  Thread T2 (tid=3359681, running) created by main thread at:
    #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:962 (libtsan.so.0+0x5ea79)
    #1 std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) <null> (libstdc++.so.6+0xd70a8)
    #2 main <null> (a.out+0x24ae)

  Thread T1 (tid=3359680, running) created by main thread at:
    #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:962 (libtsan.so.0+0x5ea79)
    #1 std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) <null> (libstdc++.so.6+0xd70a8)
    #2 main <null> (a.out+0x24ae)

SUMMARY: ThreadSanitizer: data race (/home/lolv/workspace/repo/playground/mts/a.out+0x23de) in Counter::increase()
==================
344947
ThreadSanitizer: reported 1 warnings
```

## references
* https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html
