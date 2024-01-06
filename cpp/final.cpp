#include <iostream>

class Base
{
public:
    Base() = default;
    virtual void foo() = 0;
    virtual void bar() = 0;
    virtual ~Base() = default;
};

class Derived : public Base
{
public:
    Derived() = default;
    void foo() override final {}
    void bar() override {}
    ~Derived() = default;
};

class DerivedDerived final : public Derived
{
public:
    DerivedDerived() = default;
    // void foo() override {}   // error: overriding final function
    void bar() override {}
    ~DerivedDerived() = default;
};

// we cannot derive from final class
// class DerivedDerivedDerived : public DerivedDerived
// {
// public:
//     DerivedDerivedDerived() = default;
//     void foo() override {}   // error: overriding final function
//     void bar() override {}   // error: overriding final function
//     ~DerivedDerivedDerived() = default;
// };

DerivedDerived d;

int main()
{
    Derived d;
    return 0;
}
