#include <iostream>

using namespace std;

/*
* Every class that uses virtual functions (or is derived from a class that uses virtual functions) is given its own virtual table.
* This table is simply a static array that the compiler sets up at compile time.
* A virtual table contains one entry for each virtual function that can be called by objects of the class.
* Each entry in this table is simply a function pointer that points to the most-derived function accessible by that class.
* The virtual table is stored in the portion of the most derived class with virtual functions
* when using a base class pointer to point to a derived class object, 
* https://www.learncpp.com/cpp-tutorial/the-virtual-table/
*/

class BaseWithoutVirtual
{
public:
private:
    int mVariable;
};

class BaseWithVirtual
{
public:
    virtual void function0(){};
    virtual void function1(){};
    int mVariable;
};

class FirstDerived : public BaseWithVirtual
{
public:
    virtual void function0(){};
};

class SecondDerived : public BaseWithVirtual
{
public:
    virtual void function1(){};
};

int main()
{
    cout << sizeof(BaseWithoutVirtual) << endl;
    cout << sizeof(BaseWithVirtual) << endl;
    BaseWithVirtual *bptr = new BaseWithVirtual();
    BaseWithVirtual *dptr0 = new FirstDerived();
    BaseWithVirtual *dptr1 = new SecondDerived();

    // virtual function table is stored in the portion of the most derived class
    // in this case, the virtual function table is stored in class BaseWithVirtual
    // though dptr0 and dptr1 are BaseWithVirtual pointer, it only points to the base portion(BaseWithVirtual), could not access either FirstDerived or SecondDerived portion
    // but note that the virtual function table is stored in BaseWithVirtual, so both dptr0 and dptr1 could access the virtual function table
    dptr0->function0();
    dptr1->function1();

    delete bptr;
    delete dptr0;
    delete dptr1;
    return 0;
}
