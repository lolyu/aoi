# C++ class member access control
* access control:
    * `public`
    * `protected`
    * `private`
* the default acess level for `class` is `private, and `public` for `struct` and `union`
* member access specifiers may affect class layout:
    * the addresses of non-static data members are only guaranteed to increase in order of declaration ofr the members not separated by an access specifier with the same access.

* **NOTE**: the access control is applied in compilation, the C++ executable has no information about the access control.

## public member access
* public members are accessible anywhere

## protected member access
* a protected class member is only accessible:
    * to other class members and class friends.
    * to the members of any derived class of that class(regardless of whether on the same instance or different instance), **but only the access is through the derived class or a derived class of that derived class**

```cpp
class Base;
class Derived;
class DerivedDerived;

class Base
{
protected:
    int i;

private:
    void bf(Base &b, Derived &d, DerivedDerived &dd);
};

class Derived : public Base
{
public:
    void df(Base &b, Derived &d, DerivedDerived &dd);
};

class DerivedDerived : public Derived
{
public:
    void ddf(Base &b, Derived &d, DerivedDerived &dd);
};

void Base::bf(Base &b, Derived &d, DerivedDerived &dd)
{
    ++i;            // ok, access i in the same object
    ++b.i;          // ok, access other Base object's i
    ++d.i;          // ok, access other Derived object's i
    ++dd.i;         // ok, access other DerivedDerived object's i
}

void Derived::df(Base &b, Derived &d, DerivedDerived &dd)
{
    ++i;            // ok, access the protected i in the same object
    ++(this->i);    // ok, access the protected i in the same object
    ++b.i;          // not ok, access other Base object's i
    ++d.i;          // ok, access other Derived object's i
    ++dd.i;         // ok, access other DerivedDerived object's i
}

void DerivedDerived::ddf(Base &b, Derived &d, DerivedDerived &dd)
{
    ++i;            // ok, access the protected i in the same object
    ++(this->i);    // ok, access the protected i in the same object
    ++b.i;          // not ok, access other Base object's i
    ++d.i;          // not ok, access other Derived object's i
    ++dd.i;         // ok, access other DerivedDerived object's i
}
```

## private member access
* the default access level
* only accessible to class members(regardless of whether on the same instance or different instance) or class friends

## inheritance access control
|base class member access level/inheritance access level|public|protected|private|
|-|-|-|-|
|public|public|protected|private|
|protected|protected|protected|private|
|private|not-accessible|not-accessible|not-accessible|
* **NOTE**: the default inheritance access control level for `class` is `private`, and `public` for `struct`.

```cpp
class Derived : Base
class Derived : private Base

struct Derived : Base
struct Derived : public Base
```

### public inheritance
### protected inheritance
### private inheritance

## references
* https://en.cppreference.com/w/cpp/language/access
* https://www.vishalchovatiya.com/memory-layout-of-cpp-object/
* https://en.cppreference.com/w/cpp/language/lookup
