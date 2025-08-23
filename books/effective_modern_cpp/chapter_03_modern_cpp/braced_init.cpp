#include <iostream>
#include <vector>
#include <atomic>
#include <memory>

class Widget
{
public:
    Widget()
    {
        std::cout << "Widget::Widget()" << std::endl;
    }
    Widget(int i, bool b)
    {
        std::cout << "Widget::Widget(int, bool)" << std::endl;
    }
    Widget(int i, double d)
    {
        std::cout << "Widget::Widget(int, double)" << std::endl;
    }

    Widget(std::initializer_list<long double> il)
    {
        std::cout << "Widget::Widget(std::initializer_list<long double>)" << std::endl;
    }
};

template<typename T, typename... Ts>
T doSomeWork0(Ts&&... params)
{
    return T(std::forward<Ts>(params)...);
}

template<typename T, typename... Ts>
T doSomeWork1(Ts&&... params)
{
    return T{std::forward<Ts>(params)...};
}

int main()
{
    double x = 0, y = 9, z = 10;
    int sum0 = x + y + z;
    int sum1{x + y + z}; // compiler warns narrowing conversion

    Widget w0;   // default constructor
    Widget w1(); // declares a function
    Widget w2{}; // default constructor

    Widget w3{10, true};
    Widget w4{10, 20.0};    // braced initialization always choose constructor with initializer_list

    auto vi0 = doSomeWork0<std::vector<int>>(10, 2);
    auto vi1 = doSomeWork1<std::vector<int>>(10, 2);
    auto vip = std::make_shared<std::vector<int>>(10, 2);
    std::cout << vi0.size() << std::endl;
    std::cout << vi1.size() << std::endl;
    std::cout << vip->size() << std::endl;

    return 0;
}
