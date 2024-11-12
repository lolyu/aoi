#include <iostream>

template <size_t n>
struct factorial
{
    enum { value = factorial<n - 1>::value * n };
};

template <>
struct factorial<0>
{
    enum { value = 1 };
};

int main()
{
    std::cout << factorial<1>::value << std::endl;
    std::cout << factorial<2>::value << std::endl;
    std::cout << factorial<3>::value << std::endl;
    std::cout << factorial<4>::value << std::endl;
    std::cout << factorial<5>::value << std::endl;
    std::cout << factorial<6>::value << std::endl;
    std::cout << factorial<7>::value << std::endl;
    std::cout << factorial<8>::value << std::endl;
}
