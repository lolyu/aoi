#include <iostream>

#include "stdarg.h"

void print(int n, ...)
{
    va_list va;
    va_start(va, n);
    for (int i = 0; i < n; ++i) {
        std::cout << va_arg(va, int) << ' ';
    }
    std::cout << std::endl;
}

int main()
{
    print(5, 1, 2, 3, 4, 5);
    return 0;
}