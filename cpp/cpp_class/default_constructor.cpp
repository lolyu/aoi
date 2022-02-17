#include <iostream>
// https://stackoverflow.com/questions/13576055/how-is-default-different-from-for-default-constructor-and-destructor
class A
{
public:
    A() {}
    int i;
    int j;
};

class B
{
public:
    B() = default;
    int i;
    int j;
};

int main()
{
    for (int i = 0; i < 10; ++i)
    {
        A *pa = new A();
        B *pb = new B();
        std::cout << pa->i << "," << pa->j << std::endl;
        std::cout << pb->i << "," << pb->j << std::endl;
        delete pa;
        delete pb;
    }
    return 0;
}