#include <iostream>

class A
{
public:
    void foo(){};
};

class B
{
public:
    void foo(){};
};

class C : private A
{
public:
    using A::foo;
};

class D : private B, public C
{
};

int main()
{
    D d;
    // this will complain the lookup for `foo` is ambiguous
    // https://stackoverflow.com/questions/26022439/unexpected-behaviour-of-using-declaration-with-private-inheritance-in-c
    // name lookup happens before the accessibility check
    d.foo();
    return 0;
}