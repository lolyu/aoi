#include "demo0.h"
#include <iostream>

void getStaticConstIntAddr1()
{
    std::cout << &(A::STATIC_CONST_INT) << std::endl;
}
