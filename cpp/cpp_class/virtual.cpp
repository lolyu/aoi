#include <iostream>
#include <string>

using namespace std;

/*
class method name hiding vs overriding

Name hiding: functions in the derived class will shadow those functions in the base classes with the same name
this will cause an issue if the function is called via a pointer/reference to a derived class

Overriding:
if a member function is a virtual in the base class, the function in the derived class with the same function name and signature
is also virtual and it overrides the one in the base.
*/

class Parent
{
public:
    void doA() { cout << "doA in Parent" << endl; }
    virtual void doB() { cout << "doB in Parent" << endl; }
};

class Child : public Parent
{
public:
    void doA() { cout << "doA in Child" << endl; }
    void doB() { cout << "doB in Child" << endl; }
};

Parent *p1 = new Parent();
Parent *p2 = new Child();
Child *cp = new Child();

void testStuff()
{
    p1->doA();
    p2->doA();
    cp->doA();

    p1->doB();
    p2->doB();
    cp->doB();
}

int main()
{
    testStuff();
    return 0;
}