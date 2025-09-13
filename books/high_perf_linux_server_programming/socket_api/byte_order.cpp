#include <iostream>

void byteorder()
{
    union {
        uint16_t number;
        uint8_t bytes[2];
    } test;

    test.number = 0x0102;
    if (test.bytes[0] == 0x01 && test.bytes[1] == 0x02)
    {
        std::cout << "Big endian" << std::endl;
    }
    else if (test.bytes[0] == 0x02 && test.bytes[1] == 0x01)
    {
        std::cout << "Little endian" << std::endl;
    }
    else
    {
        std::cout << "Unknown endian" << std::endl;
    }
}

int main()
{
    byteorder();
    return 0;
}
