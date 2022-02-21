#include <iostream>

using namespace std;

class Base
{
public:
    virtual const Base *getThis() const
    {
        cout << "Base::getThis" << endl;
        return this;
    }
};

class Derived : public Base
{
public:
    const Derived *getThis() const
    {
        cout << "Derived::getThis" << endl;
        return this;
    }
};

int main()
{
    Base *bp = new Derived();
    bp->getThis();
    return 0;
}