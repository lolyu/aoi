#include <iostream>
#include <string>

using namespace std;

class Base {
public:
    template <typename T>
    void foo(T t)
    {
        cout << t << endl;
    }
};

template void Base::foo<double>(double t);

class Derived : public Base {
public:
    using Base::foo;
    void foo(int &i) { i = 100; }
};

template void Derived::foo<string>(string s);

int main()
{
    Derived d;
    d.foo(100);
    d.foo(string("helloworld"));
    return 0;
}