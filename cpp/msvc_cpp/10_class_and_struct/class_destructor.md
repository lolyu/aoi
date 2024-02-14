# class destructor
* class order of destruction:
    * the body of the destructor function is executed
    * destructors for non-static member objects are called in the reverse order of the declaration.
    * destructors for non-virtual base classes are called in the reverse order of declaration.
    * destructors for virtual base classes are called in the reverse order of declaration.

## destructor order
* constructors are called as a depth-first, left-to-right, and post-order traversal of the inheritance directed asyclic graph
* destructors are called in the reverse order of construction.

```cpp
class Base
{
public:
    Base() { cout << typeid(*this).name() << endl; }
    ~Base() { cout << typeid(*this).name() << endl; }
};

class A : public Base
{
public:
    A() { cout << typeid(*this).name() << endl; }
    ~A() { cout << typeid(*this).name() << endl; }
};

class B : public Base
{
public:
    B() { cout << typeid(*this).name() << endl; }
    ~B() { cout << typeid(*this).name() << endl; }
};

class C : virtual public A, virtual public B
{
public:
    C() { cout << typeid(*this).name() << endl; }
    ~C() { cout << typeid(*this).name() << endl; }
};

class D : virtual public A, virtual public B
{
public:
    D() { cout << typeid(*this).name() << endl; }
    ~D() { cout << typeid(*this).name() << endl; }
};

class E : public C, public D, virtual public B
{
public:
    E() { cout << typeid(*this).name() << endl; }
    ~E() { cout << typeid(*this).name() << endl; }
};
```
* for the above example:
    * construction order:
        * `Base` -> `A` -> `Base` -> `B` -> `C` -> `D` -> `E`
    * destruction order
        * `E` -> `D` -> `C` -> `B` -> `Base` -> `A` -> `Base`
* **NOTE**: the virtual base class is initialized only once, so once a virtual base class is visited and constructed in the traversal, ignore it.
## reference
