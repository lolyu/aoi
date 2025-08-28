#include <iostream>
#include <type_traits>
#include <string>
#include <map>

// template alias
template <typename T>
using StrMap0 = std::map<std::string, T>;

// typedef
template <typename T>
struct StrMap1
{
    typedef std::map<std::string, T> type;
};

template <typename T>
void printStrMap0(void *ptr)
{
    auto mp = static_cast<StrMap0<T> *>(ptr);
    if (!ptr) return;
    for (auto it = mp->cbegin(); it != mp->cend(); ++it)
    {
        std::cout << "<" << it->first << ", " << it->second << ">" << std::endl;
    }
}

template <typename T>
void printStrMap1(void *ptr)
{
    auto mp = static_cast<typename StrMap1<T>::type *>(ptr);
    if (!ptr) return;
    for (auto it = mp->cbegin(); it != mp->cend(); ++it)
    {
        std::cout << "<" << it->first << ", " << it->second << ">" << std::endl;
    }
}

int main()
{
    StrMap0<int> m0{{"Alice", 10}, {"Bob", 20}};
    StrMap1<int>::type m1{{"Cat", 10}, {"David", 20}};

    printStrMap0<int>(&m0);
    printStrMap1<int>(&m1);

    // std::remove_reference<T>::type is defined with typedef
    // std::remove_reference_t<T> is defined with template alias

    return 0;
}
