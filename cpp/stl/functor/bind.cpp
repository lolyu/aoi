#include <functional>
#include <iostream>

/**
 * references:
 * * https://en.cppreference.com/w/cpp/utility/functional/ref
 */

double my_divide(double x, double y)
{
    return x / y;
}

struct MyPair {
    double x;
    double y;
    double multiply() { return x * y; }
};

int main()
{
    auto fn0 = std::bind(my_divide, 10, 2);
    std::cout << fn0() << std::endl;

    // std::bind(class fn, args0, args1, ...)
    // args0, args1, ... must meet the signature of fn
    // args0, args1, ... could be either values or placeholders
    auto fn1 = std::bind(my_divide, std::placeholders::_1, 2);
    std::cout << fn1(10) << std::endl;

    auto fn2 = std::bind(my_divide, std::placeholders::_2, std::placeholders::_1);
    std::cout << fn2(10, 2) << std::endl;

    auto fn3 = std::bind<int>(my_divide, std::placeholders::_1, std::placeholders::_2);
    std::cout << fn3(10, 2) << std::endl;

    MyPair mypair{10, 2};
    auto fn4 = std::bind(&MyPair::multiply, std::placeholders::_1);
    std::cout << fn4(mypair) << std::endl;

    auto fn5 = std::bind(&MyPair::x, std::placeholders::_1);
    std::cout << fn5(mypair) << std::endl;

    return 0;
}