#include <iostream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <vector>
#include <boost/type_index.hpp>

template <typename T>
std::string type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::string tn = typeid(TR).name();
    if (std::is_const<TR>::value)
        tn += " const";
    if (std::is_volatile<TR>::value)
        tn += " volatile";
    if (std::is_lvalue_reference<T>::value)
        tn += "&";
    else if (std::is_rvalue_reference<T>::value)
        tn += "&&";
    return tn;
}

template <typename T>
void f0(const T &param)
{
    std::cout << "T = " << type_name<T>() << std::endl;
    std::cout << "param = " << type_name<decltype(param)>() << std::endl;
}

template <typename T>
void f1(const T &param)
{
    std::cout << "T = " << boost::typeindex::type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << "param = " << boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name() << std::endl;}

class Widget {};

std::vector<Widget> create_widgets(size_t n)
{
    return std::vector<Widget>(n);
}

int main()
{
    const auto vw = create_widgets(2);

    f0(&vw[0]);
    f1(&vw[0]);

    return 0;
}
