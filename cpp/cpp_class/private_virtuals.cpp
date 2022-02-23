#include <iostream>

using namespace std;

class Base
{
private:
    virtual void opStep0() { cout << "Base::opStep0" << endl; }
    virtual void opStep1() { cout << "Base::opStep1" << endl; }

public:
    void doOperation()
    {
        opStep0();
        opStep1();
    }
};

class Derived : public Base
{
public:
    void opStep0() { cout << "Derived::onStep0" << endl; }
    void opStep1() { cout << "Derived::onStep1" << endl; }
};

int main()
{
    Base *bp = new Derived();
    bp->doOperation();
    bp->opStep0();
    delete bp;
    return 0;
}