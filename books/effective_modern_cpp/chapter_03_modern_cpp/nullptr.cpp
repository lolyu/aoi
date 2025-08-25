#include <iostream>
#include <mutex>
#include <memory>

#define NULL 0

class Widget {};

int f1(std::shared_ptr<Widget> spw)
{
    return 10;
}

double f2(std::unique_ptr<Widget> upw)
{
    return 3.14;
}

bool f3(Widget* pw)
{
    return true;
}

template <typename F, typename M, typename P>
auto lockAndCall(F func, M &m, P p) -> decltype(auto)
{
    std::lock_guard<M> lock(m);
    return func(p);
}

int main()
{
    std::mutex m;
    std::cout << f1(0) << std::endl;                            // 0 is implicitly converted to nullptr
    // std::cout << lockAndCall(f1, m, 0) << std::endl;         // implicit conversion won't be considered inside template function

    std::cout << f2(NULL) << std::endl;                         // NULL is implicitly converted to nullptr
    // std::cout << lockAndCall(f2, m, NULL) << std::endl;      // same as above

    std::cout << f3(nullptr) << std::endl;
    std::cout << lockAndCall(f3, m, nullptr) << std::endl;

    return 0;
}
