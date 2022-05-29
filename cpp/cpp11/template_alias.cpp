#include <array>
#include <iostream>

const int LIMIT = 5;

template <typename ElemType>
using MyArry = std::array<ElemType, LIMIT>;

int main()
{
    MyArry<double> elems{1.1, 1.2, 1.3, 1.4, 1.5};
    return 0;
}