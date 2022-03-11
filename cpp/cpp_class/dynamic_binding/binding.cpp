#include <iostream>
#include <typeinfo>

using namespace std;

/*
g++ -fdump-class-hierarchy -c

Vtable for Base
Base::_ZTV4Base: 3 entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI4Base)
16    (int (*)(...))Base::foo

Class Base
   size=8 align=8
   base size=8 base align=8
Base (0x0x7febf52c98a0) 0 nearly-empty
    vptr=((& Base::_ZTV4Base) + 16)

Vtable for Derived
Derived::_ZTV7Derived: 3 entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI7Derived)
16    (int (*)(...))Derived::foo

Class Derived
   size=8 align=8
   base size=8 base align=8
Derived (0x0x7febf52fca90) 0 nearly-empty
    vptr=((& Derived::_ZTV7Derived) + 16)
  Base (0x0x7febf5302660) 0 nearly-empty
      primary-for Derived (0x0x7febf52fca90)
*/

class Base
{
public:
    virtual void foo()
    {
        cout << "Base::foo " << typeid(this).name() << endl;
    }
    void bar() { foo(); }
};

class Derived : public Base
{
public:
    void foo() override
    {
        cout << "Derived::foo " << typeid(this).name() << endl;
        cell();
    }

    void cell()
    {
        cout << "Derived::cell " << typeid(this).name() << endl;
    }
};

int main()
{
    Base *bp = new Derived();
    bp->foo();
    bp->bar();
    return 0;
}