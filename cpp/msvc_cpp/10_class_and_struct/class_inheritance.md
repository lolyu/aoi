# class inheritance

```
class Derived : [virtual] [access-specifier] Base1, [virtual] [access-specifier] Base2, ...
{
    // class declaration body
};
```

## virtual functions
* a function in derived classes override virtual functions in base classes only if their type is the same.
* **NOTE**: the override of virtual functions in derived class are always virtual, there is no need to be defined with the `virtual` keyword again.
* for the details about how virtual functions are implemented, refer to this doc: https://github.com/lolyu/aoi/blob/master/cpp/msvc_cpp/10_class_and_struct/virtual_function.md

### suppress virtual function call
* virtual function call could be suppressed with the scope-resolution operator
```cpp
#include <iostream>

using namespace std;

class Base
{
    public:
    virtual void f() { cout << "Base::f" << endl; };
};

class Derived : public Base
{
    public:
    void f() override { cout << "Derived::f" << endl; }
};

class DerivedDerived : public Derived
{
    public:
    void f() override { cout << "DerivedDerived::f" << endl; }
};

int main()
{
    Derived d;
    DerivedDerived dd;

    Base &b0 = d;
    Base &b1 = dd;
    b0.f();
    b1.f();

    b0.Base::f();
    b1.Base::f();

    return 0;
}
```

## single inheritance
## multiple inheritance
* for multiple inheritance, the base class list order matters:
    * the class appeared ealier is initialized earlier
    * the class appeared ealier is destroyed later

* multiple inheritance might introduce name ambiguity, which could be resolved with qualified resolution.
* multiple inheritance also could introduce conversion ambiguity when convert pointer/reference to derived class to the parent class subject as there might be multiple parent class subobjects inside the derived class object.
```cpp
#include <iostream>

using namespace std;

struct Queue
{
    int i;
};

struct CashierQueue : public Queue
{
    int j;
};

struct LunchQueue : public Queue
{
    int k;
};

struct TakeoutQueue : public Queue
{
    int l;
};

struct LunchCashierQueue : public CashierQueue, public LunchQueue
{
    int m;
};

struct LunchTakeoutCashierQueue : public LunchCashierQueue, public TakeoutQueue
{
    int n;
};

int main()
{
    LunchTakeoutCashierQueue ltcq;
    ltcq.CashierQueue::i = 1;
    ltcq.LunchQueue::i = 2;
    ltcq.TakeoutQueue::i = 3;
    // name ambiguity, compiler doesn't know which Queue subobject to use
    // there are three Queue subobjects in the LunchTakeoutCashierQueue object
    ltcq.Queue::i = 2;

    LunchTakeoutCashierQueue *pltcq = &ltcq;
    Queue *q;
    q = (Queue *)(LunchQueue *)pltcq;
    q = (Queue *)(LunchQueue *)pltcq;
    q = (Queue *)(TakeoutQueue *)pltcq;
    // conversion ambiguity as there are three Queue subobjects in the LunchTakeoutCashierQueue object
    q = (Queue *)pltcq;
    return 0;
}
```

### virtual base
* a class can be an indirect base class to a derived class more than once, so the derived class object will have multiple copies of the class members.
* with virtual base class, the derived class only have one copy of the base class
![image](https://github.com/lolyu/aoi/assets/35479537/771e441a-0a32-4b21-8aef-29464c0ed61c)


## reference
