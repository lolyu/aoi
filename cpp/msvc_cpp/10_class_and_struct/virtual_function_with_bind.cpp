#include <iostream>
#include <typeinfo>
#include <functional>

using namespace std;

class Base
{
public:
    virtual void foo() { cout << typeid(this).name() << endl; };
    void bar() { cout << typeid(this).name() << endl; };
};

class Derived : public Base
{
public:
    void foo() { cout << typeid(this).name() << endl; }
    void bar() { cout << typeid(this).name() << endl; };
};

int main()
{
    Base b0;
    Derived d0;

    Base *bptr = &d0;

    // virtual function could be dispatched with std::bind, and
    // this pointer is always of the class that defines the
    // dispatched member function
    std::bind(&Base::foo, &d0)();           // Derived*
    std::bind(&Base::foo, bptr)();          // Derived*

    std::bind(&Base::bar, &d0)();           // Base*
    std::bind(&Base::bar, bptr)();          // Base*

    return 0;
}
