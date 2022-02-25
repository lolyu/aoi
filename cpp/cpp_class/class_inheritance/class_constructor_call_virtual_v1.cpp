#include <iostream>
#include <string>

using namespace std;

class Helper {
public:
    virtual void foo() const { cout << "Helper::foo" << endl; }
    virtual void bar() const { cout << "Helper::bar" << endl; }
};

class Helper1 : public Helper {
public:
    void foo() const { cout << "Helper1::foo" << endl; }
    void bar() const { cout << "Helper1::bar" << endl; }
};

class Base {
public:
    Base(const Helper &h)
    {
        h.foo();
        h.bar();
    }
};

class Derived : public Base {
public:
    Derived()
        : Base(Helper1()) {}
};

class Base1 {
public:
    using CustomFn = void (*)();
    Base1(CustomFn foo = &foo, CustomFn bar = &bar)
    {
        foo();
        bar();
    }

private:
    static void foo() { cout << "Base1::foo" << endl; };
    static void bar() { cout << "Base1::bar" << endl; };
};

class Derived1 : public Base1 {
public:
    Derived1()
        : Base1(foo, bar) {}

private:
    static void foo() { cout << "Derived1::foo" << endl; };
    static void bar() { cout << "Derived1::bar" << endl; };
};

int main()
{
    Derived d;
    Derived1 d1;
    return 0;
}