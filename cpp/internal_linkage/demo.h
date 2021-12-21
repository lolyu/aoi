#ifndef _DEMO_H_
#define _DEMO_H_
#include <iostream>
#include <string>

static int counter = 0;
static int i;
const int t = 100;

/*
A::A and A::~A violates the one definition rule(ODR) because counter is a static variable with internal linkage, each translation unit including
this header will have its unique copy, so A::A and A::~A are not the same methods across different translation unit.
*/

class A
{
public:
    A()
    {
        ++counter;
        std::cout << "A constructor: " << counter << std::endl;
    }

    ~A()
    {
        --counter;
        std::cout << "A destructor: " << counter << std::endl;
    }
    const static int const_static_member = 20;
    const static std::string static_str_member;
    static int static_member;
    // could initialize static class member inside the header file
    // static int static_member = 10;
};

// int A::static_member = 10;
// const std::string A::static_str_member = "helloworld";

static A a;

#endif