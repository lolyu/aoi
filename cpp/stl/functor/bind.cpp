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

void myfunc(int &n0, int &n1, const int &n2)
{
    std::cout << n0 << " " << n1 << " " << n2 << std::endl;
    ++n0;
    ++n1;
}

struct Elem {
    Elem(int i = 10)
        : i(i){};
    Elem(const struct Elem &elem)
    {
        std::cout << "Elem copy constructor." << std::endl;
        this->i = elem.i;
    }

    int i;
};

void func_elem_ref(struct Elem &e)
{}

void func_elem_value(struct Elem e)
{}

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

    int n0 = 1, n1 = 2, n2 = 3;
    auto bound_f = std::bind(myfunc, n0, std::ref(n1), std::cref(n2));
    n0 = 10, n1 = 20, n2 = 30;
    bound_f();
    std::cout << n0 << " " << n1 << " " << n2 << std::endl;

    struct Elem elem(100);

    auto bound_func_elem0 = std::bind(func_elem_ref, elem);
    bound_func_elem0();

    auto bound_func_elem1 = std::bind(func_elem_value, std::ref(elem));
    bound_func_elem1();

    auto bound_func_elem2 = std::bind(func_elem_ref, std::ref(elem));
    bound_func_elem2();

    return 0;
}