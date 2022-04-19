#include <iostream>
#include <type_traits>

using namespace std;

/*
https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170

Special member functions:
    * default constructor
    * Destructor
    * Copy constructor
    * Copy assignment
    * Move constructor
    * Move assignment

Trivial class: when a class has compiler-provided or explicitly default special member functions, it is a trivial type
Trivial types have:
    * trivial default constructor
    * trivial copy constructor
    * trivial copy assignment
    * trivial destructor
    * no virtual functions or virtual base classes
    * no base class that is non-trivial
    * no data member that is non-trivial

Trivial types:
    * can do memcopy
    * cannot be consumed by C program

Standard layout types: doesn't contain C++ language features like virtual functions which are not found in the C language, and all members have the same access control


POD: plain old types, both trivial and standard layout types

*/

class Trivial0
{
private:
public:
    int i;
};

class Trivial1
{
private:
public:
    int i;
    Trivial1() = default;
    Trivial1(int i)
        : i(i) {}
};

class NonTrivial0
{
private:
public:
    int i;
    NonTrivial0() = default;
    NonTrivial0(int i)
        : i(i) {}
    ~NonTrivial0() {}
};

class NonTrivial1
{
private:
public:
    int i;
    NonTrivial1(int i = 10)
        : i(i) {}
    ~NonTrivial1() = default;
};

struct B {
protected:
    virtual void Foo() {}
};

struct A : B {
    int a;
    int b;
    void Foo() override {}
};

struct C {
    int a;

private:
    int b;
};

struct D {
    int a;
    int b;
    D() {}
};

struct POD {
    int a;
    int b;
};

int main()
{
    cout << boolalpha;
    cout << is_trivial<Trivial0>::value << endl;
    cout << is_trivial<Trivial1>::value << endl;
    cout << is_trivial<NonTrivial0>::value << endl;
    cout << is_trivial<NonTrivial1>::value << endl;

    cout << endl;
    cout << is_trivial<B>() << endl;           // false
    cout << is_standard_layout<B>() << endl;   // false
    cout << is_trivial<A>() << endl;           // false
    cout << is_standard_layout<A>() << endl;   // false
    cout << is_trivial<C>() << endl;           // true
    cout << is_standard_layout<C>() << endl;   // false
    cout << is_trivial<D>() << endl;           // false
    cout << is_standard_layout<D>() << endl;   // true
    cout << is_trivial<POD>() << endl;         // true
    cout << is_standard_layout<POD>() << endl; // true
    return 0;
}