#include <iostream>
#include <typeinfo>

class Base
{
public:
    virtual void foo() const
    {
        std::cout << typeid(*this).name() << "->foo()" << std::endl;
    }
};

class Derived : private Base
{
public:
    virtual void foo() const
    {
        std::cout << typeid(*this).name() << "->foo()" << std::endl;
    }
};

class DerivedDerived : private Derived {};

void func(const Base &b) {}

class EmptyBase {};

class HoldsAnInt
{
private:
    int x;
    EmptyBase e;
};

class HoldsAnIntEBO : private EmptyBase
{
private:
    int x;
};

int main()
{
    Base *bptr = new Base();
    Derived *dptr = new Derived();
    DerivedDerived *ddptr = new DerivedDerived();

    func(*bptr);
    // func(*dptr);

    bptr->foo();
    dptr->foo();

    std::cout << sizeof(HoldsAnInt) << std::endl;       // 8
    std::cout << sizeof(HoldsAnIntEBO) << std::endl;    // 4

    delete bptr, dptr, ddptr;

    return 0;
}
