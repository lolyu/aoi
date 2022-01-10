#include "demo0.h"
#include <iostream>

// const int A::STATIC_CONST_INT = 10;
// https://en.cppreference.com/w/cpp/language/static
// if a const non-inline static data member or a constexpr static data member is odr-used,
// a definition at namespace scope is still required, but it cannot have an initializer.
const int A::STATIC_CONST_INT;

void getStaticConstIntAddr0()
{
    std::cout << &(A::STATIC_CONST_INT) << std::endl;
}