#include <iostream>
#include <type_traits>
#include <typeinfo>

template <class T>
std::string
type_name()
{
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

template <typename T, std::size_t N>
std::size_t array_size(const T (&)[N])
{
    return N;
}

template <typename T>
void f0(const T &t)
{
    std::cout << "Template function pass by reference, T is " << type_name<T>() << std::endl;
}

template <typename T>
void f1(T &&t)
{
    std::cout << "Template function pass by universal reference, T is " << type_name<T>() << std::endl;
}

template <typename T>
void f2(T t)
{
    std::cout << "Template function pass by value, T is " << type_name<T>() << std::endl;
}

int main()
{
    int i = 42;
    const int &ri = i;
    const char s[] = "Hello, World!";

    std::cout << "Array size of s is " << array_size(s) << std::endl;

    f0(i);          // T is int
    f0(ri);         // T is int
    f0(s);          // T is const char[14]

    f1(i);          // T is int&
    f1(ri);         // T is const int &
    f1(s);          // T is const char (&)[14]
    f1(100);        // T is int

    f2(i);          // T is int
    f2(ri);         // T is int
    f2(s);          // T is const char *, this is array decay into pointer
    f2(100);        // T is int

    return 0;
}
