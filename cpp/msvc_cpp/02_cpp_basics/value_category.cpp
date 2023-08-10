#include <iostream>

namespace detail
{
    template <class T>
    struct value_category
    {
        static constexpr char const *value = "prvalue";
    };
    template <class T>
    struct value_category<T &>
    {
        static constexpr char const *value = "lvalue";
    };
    template <class T>
    struct value_category<T &&>
    {
        static constexpr char const *value = "xvalue";
    };
}

#define PRINT_VALUE_CAT(expr) std::cout << #expr << " is a " << ::detail::value_category<decltype((expr))>::value << '\n'

struct S
{
    int i;
};

int main()
{
    int &&r = 42;
    PRINT_VALUE_CAT(4);            // prvalue
    PRINT_VALUE_CAT(r);            // lvalue
    PRINT_VALUE_CAT(std::move(r)); // xvalue

    PRINT_VALUE_CAT(S{0});   // prvalue
    PRINT_VALUE_CAT(S{0}.i); // xvalue
}
