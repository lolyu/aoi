#include <iostream>

using namespace std;

/*
    /--> B
A -----> C
    \--> D


if there could be multiple instances of a base class, the base class should be a virtual base class,
so all class derived from the virtual base class should have virtual keyword before/after the accessibility mark
*/

class A
{
public:
    int a;
};

class B : virtual public A
{
public:
    int b;
};

class C : virtual public A
{
public:
    int c;
};

class D : virtual public A
{
public:
    int d;
};

class BC : public B, virtual public C
{
public:
    int bc;
};

class CD : virtual public C, public D
{
public:
    int cd;
};

class BCD : public BC, public CD
{
public:
    int bcd;
};

int main()
{
    BCD bcd;
    bcd.a = 10;
    bcd.c = 10;
    return 0;
}