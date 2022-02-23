#include <iostream>
#include <string>

using namespace std;

/*
the rationale behind `public overloaded non-virtuals call protected non-overloaded virtuals`:

If a public interface has serveral virtual overloads and the derived class writer only override one, the other overloads
that are not overrided will be hiding if users of the derived class try to access those overloads via the derived class.
So the derived class writer should always take care to expose those non-overrided overloads to the derived class via using syntax.

Protected and private virtuals define the class’ customizable behavior.
Public functions defined the class's interfaces, which should be consistent across class inheritance
*/

class Base
{
public:
    virtual void f(int x) { cout << "Base::f(int x)" << endl; }
    virtual void f(string x) { cout << "Base::f(string x)" << endl; }
};

class Derived : public Base
{
public:
    void f(int x) { cout << "Derived::f(int x)" << endl; }
    using Base::f;
};

int main()
{
    Base *bp = new Derived();
    bp->f(100);
    delete bp;

    // the hiding rule applies when access member function via the derived class or derived class pointer
    // the functions defined in the derived class will shadow those defined in the base class.
    Derived *dp = new Derived();
    dp->f("helloworld");
    dp->f(100);
    delete dp;
    return 0;
}