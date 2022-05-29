#include <iostream>

using namespace std;

/**
 * Six special member functions
 * * default contructor
 * * destructor
 * * copy constructor
 * * copy assignment
 * * move constructor
 * * move assignment
 */

/*
 * default constructor will be implicitly defined if no other constructors provided
 * destructor will be implicitly defined if no destructor provided
 * copy constructor will be implicitly defined if no move constructor and move assignment provided
 * copy assignment will be implicitly defined if no move constructor and move assignment provided
 * move constructor will be implicitly defined if no destructor, copy constructor, copy assignment, and move assignment provided
 * move assignment will be implicitly defined if no destructor, copy constructor, copy assignment, and move assignment provided
 */

struct A {
    int i;
};

struct B {
    int i;
    B(int i)
        : i(i) {}
};

struct C {
    int i;
    C(int i)
        : i(i) {}
    C() = default;
    ~C() = default;
    C(C &&c)
        : i(c.i){};
};

int main()
{
    B b0(10);
    // B b1;
    C c0(100);
    // c0 = []() { return C(200); }();
    return 0;
}