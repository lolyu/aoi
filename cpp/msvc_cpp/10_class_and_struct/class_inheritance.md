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
## 


## reference
