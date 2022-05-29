#include <iostream>

using namespace std;

/*
https://en.cppreference.com/w/cpp/language/override
https://en.cppreference.com/w/cpp/language/final
*/

/*
`override` ensures the function is overriding a virtual function in the base class
`final`:
if used for a member function, it ensures the function is overriding a virtual function in the base class, and the function itself could not be override in derived classes
if used for a class, itself could not be used as a base class
*/

struct A {
    virtual void foo() {}
    void bar() {}
    virtual ~A() = default;
};

struct B final : public A {
    virtual void foo() override {}
    // void bar() override {}                   // cannot use `override` with a non-virtual function
    virtual ~B() override = default;
};

struct C : public A {
    void foo() final {}
    // void bar() final {}                      // cannot use `final` with a non-virtual function
};

struct D : public C {
    // void foo() {}                            // cannot override a final member function
};

// struct E : public B                          // cannot derive from a final class
// {
// };

int main()
{
    return 0;
}