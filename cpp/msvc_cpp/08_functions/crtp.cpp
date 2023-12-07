#include <iostream>

template <typename T>
class Base
{
public:
    void name()
    {
        static_cast<T *>(this)->implement_name();
    }
};

class Derived0 : public Base<Derived0>
{
public:
    void implement_name()
    {
        std::cout << "Derived0" << std::endl;
    }
};

class Derived1 : public Base<Derived1>
{
public:
    void implement_name()
    {
        std::cout << "Derived1" << std::endl;
    }
};

int main()
{
    Derived0 d0;
    Derived1 d1;
    d0.name();
    d1.name();
    return 0;
}
