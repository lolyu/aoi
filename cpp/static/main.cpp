#include <iostream>
// #include "demo0.h"
#include "demo0.h"

// extern const int A::STATIC_CONST_INT;

int main()
{
    getStaticConstIntAddr0();
    getStaticConstIntAddr1();
    std::cout << A::STATIC_CONST_INT << std::endl;
    A a;
    return 0;
}