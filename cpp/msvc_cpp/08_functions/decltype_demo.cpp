#include <iostream>
#include <typeinfo>

int f() { return 10; }

int main()
{
    int i = 0;
    int j = 1;

    decltype(i) d;
    decltype(f()) e;
    decltype((i)) f = i;
    decltype(i + j) g;

    std::cout << typeid(d).name() << std::endl;
    std::cout << typeid(e).name() << std::endl;
    std::cout << typeid(f).name() << std::endl;
    std::cout << typeid(g).name() << std::endl;

    return 0;
}
