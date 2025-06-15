#include <iostream>
#include <type_traits>
#include <typeinfo>

template <class T>
std::string
type_name()
{
    std::string r = typeid(T).name();
    if (std::is_const<T>::value)
        r += " const";
    if (std::is_volatile<T>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

int main()
{
    auto x = 27;
    const auto cx = x;
    const auto &rcx = x;

    auto &&uref1 = x;
    auto &&uref2 = cx;
    auto &&uref3 = 100;

    std::cout << "x is " << type_name<decltype(x)>() << std::endl;          // x is int
    std::cout << "cx is " << type_name<decltype(cx)>() << std::endl;        // cx is const int
    std::cout << "rcx is " << type_name<decltype(rcx)>() << std::endl;      // rcx is const int&
    std::cout << "uref1 is " << type_name<decltype(uref1)>() << std::endl;  // uref1 is int &
    std::cout << "uref2 is " << type_name<decltype(uref2)>() << std::endl;  // uref2 is const int &
    std::cout << "uref3 is " << type_name<decltype(uref3)>() << std::endl;  // uref3 is int &&

    return 0;
}
