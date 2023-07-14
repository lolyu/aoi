#include <iostream>
#include <atomic>
#include <vector>
#include <thread>

class Singleton
{
private:
    static std::atomic<Singleton *> cached_object;

public:
    Singleton() : data(++count) {}

    ~Singleton() = default;

    int get_data() { return data; }

    static Singleton *get_object()
    {
        Singleton *obj = cached_object.load();
        if (!obj)
        {
            obj = new Singleton();
            if (obj)
            {
                Singleton *cached = nullptr;
                // if compare_exchange_weak returns true
                // => the cached_object is replaced with the new object created by this thread
                // if compare_exchange_weak returns false
                // => 1. spurious failure, the cached_object is still a nullptr, should retry
                // => 2. not a spurious failure, the cached_object is not a nullptr(created by another thread), should exit
                while (!cached_object.compare_exchange_weak(cached, obj) && !cached)
                {
                    ;
                }
                if (cached)
                {
                    delete obj;
                    obj = cached;
                }
            }
        }
        return obj;
    }

    static int count;
    int data;
};

int Singleton::count = 0;
std::atomic<Singleton *> Singleton::cached_object;

void f()
{
    auto obj = Singleton::get_object();
    std::cout << obj->get_data() << std::endl;
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 20; ++i)
    {
        threads.emplace_back(f);
    }

    for (auto &th : threads)
    {
        th.join();
    }
    return 0;
}
