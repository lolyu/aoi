#include <iostream>
#include <type_traits>
#include <typeinfo>

template <class T>
std::string
type_name()
{
    // NOTE: why we need remove the reference here?
    // if T const int &, is_const<T>::value is false, as T itself is not const-qualified.
    // but TR is const int, which is const-qualified
    typedef typename std::remove_reference<T>::type TR;
    std::string r = typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
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
