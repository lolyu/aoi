#include <iostream>

class A
{
public:
    void func() {}
};

class B : public A
{
public:
    void func() {}
};

int main()
{
    B b;
    b.func();
    return 0;
}
