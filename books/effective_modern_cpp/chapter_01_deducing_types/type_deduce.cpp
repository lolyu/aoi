#include <iostream>
#include <string>
#include <typeinfo>
#include <type_traits>
#include <vector>
#include <boost/type_index.hpp>

template <typename T>
void type_name_helper(std::vector<std::string> &results)
{
    if (std::is_reference<T>::value)
    {
        typedef typename std::remove_reference<T>::type TR;

        if (std::is_const<T>::value)
            results.push_back("const");

        if (std::is_lvalue_reference<T>::value)
            results.push_back("&");
        else if (std::is_rvalue_reference<T>::value)
            results.push_back("&&");

        type_name_helper<TR>(results);
    }
    else if (std::is_pointer<T>::value)
    {
        typedef typename std::remove_pointer<T>::type TP;

        if (std::is_const<T>::value)
            results.push_back("const");
        results.push_back("*");

        type_name_helper<TP>(results);
    }
    else
    {
        if (std::is_volatile<T>::value)
            results.push_back("volatile");
        if (std::is_const<T>::value)
            results.push_back("const");
        results.push_back(typeid(T).name());
        return;
    }
}

template <typename T>
std::string type_name()
{
    std::vector<std::string> results;
    type_name_helper<T>(results);
    std::string result;

    for (auto it = results.crbegin(); it != results.crend(); ++it)
    {
        result += (" " + *it);
    }

    return result;
}

template <typename T>
void f(const T &param)
{
    std::cout << "T = " << type_name<T>() << std::endl;
    std::cout << "param = " << type_name<decltype(param)>() << std::endl;
    std::cout << "T = " << boost::typeindex::type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << "param = " << boost::typeindex::type_id_with_cvr<decltype(param)>().pretty_name() << std::endl;
}

class Widget {};

std::vector<Widget> create_widgets(size_t n)
{
    return std::vector<Widget>(n);
}

int main()
{
    const auto vw = create_widgets(2);

    f(vw[0]);

    std::cout << std::endl;

    f(&vw[0]);

    return 0;
}
