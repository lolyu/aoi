#include <iostream>
#include <stdlib.h>
#include <arpa/inet.h>

int main()
{
    unsigned short a = 0x1234;
    auto b = ntohs(a);
    auto c = htons(a);
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    return 0;
}