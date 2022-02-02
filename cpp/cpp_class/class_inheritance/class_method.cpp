#include <iostream>

using namespace std;

/*
* method resolution:
* https://stackoverflow.com/questions/3310910/method-resolution-order-in-c
*/

class Base
{
public:
    virtual void function0() { cout << "Base::function0" << endl; }
    void function1() { cout << "Base::function1" << endl; }
    void function2() { cout << "Base::function2" << endl; }
    void function3() { cout << "Base::function3" << endl; }
};

class Derived : public Base
{
public:
    virtual void function0() { cout << "Derived::function0" << endl; }
    void function1(int i) { cout << "Derived::function1: " << i << endl; }
    void function2(int i) { cout << "Derived::function2: " << i << endl; }
};

int main()
{
    Base *pbase = new Derived();
    Derived *pderived = static_cast<Derived *>(pbase);
    pbase->function0();
    pbase->function1();
    pbase->function2();
    pbase->function3();
    // pbase->function1(10);                // base class pointer only points to the base portion
    // pderived->function1();               // function overloading only works in the same scope, derived classes introduce new scopes.
    pderived->function1(10);
    // pderived->function2();               // functions with same name will always hide those in the base class
    pderived->function2(10);
    pderived->function3();
    delete pderived;
    return 0;
}
