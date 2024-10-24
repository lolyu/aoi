#include <iostream>
#include <typeinfo>

class Base
{
public:
    virtual void foo(int n = 100)
    {
        std::cout << typeid(*this).name() << ": " << n << std::endl;
    }
};

class Derived : public Base
{
public:
    void foo(int n = 200) override
    {
        std::cout << typeid(*this).name() << ": " << n << std::endl;
    }
};

int main()
{
    Base *bptr0 = new Base();
    Derived *dptr0 = new Derived();
    Base *bptr1 = dptr0;

    // virtual function called via base class pointer always uses default parameter from
    // the base class declaration.
    bptr0->foo();
    bptr1->foo();
    dptr0->foo();

    return 0;
}

// 4Base: 100
// 7Derived: 100
// 7Derived: 200
