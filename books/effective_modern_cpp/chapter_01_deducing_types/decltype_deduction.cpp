#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <string>
#include <vector>

template <typename T>
std::string type_name()
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

template<typename Container, typename Index>
auto access0(Container &c, Index i) -> decltype(c[i])
{
    return c[i];
}

template<typename Container, typename Index>
auto access1(Container &c, Index i)
{
    return c[i];
}

template<typename Container, typename Index>
decltype(auto) access2(Container &c, Index i)
{
    return c[i];
}

template<typename Container, typename Index>
decltype(auto) access3(Container &&c, Index i)
{
    return c[i];
}

template<typename Container, typename Index>
decltype(auto) access4(Container &&c, Index i)
{
    return std::forward<Container>(c)[i];
}

int main()
{
    std::vector<int> vi{0, 1, 2, 3, 4, 5};
    std::cout << type_name<decltype(access0(vi, 3))>() << std::endl;                                // int&, decltype deduction
    std::cout << type_name<decltype(access1(vi, 3))>() << std::endl;                                // int, auto deduction
    std::cout << type_name<decltype(access2(vi, 3))>() << std::endl;                                // int&, decltype deduction
    std::cout << type_name<decltype(access3(vi, 3))>() << std::endl;                                // int&
    std::cout << type_name<decltype(access3(std::vector<int>({0, 1, 2, 3}), 3))>() << std::endl;
    std::cout << type_name<decltype(access4(std::vector<int>({0, 1, 2, 3}), 3))>() << std::endl;
    return 0;
}
