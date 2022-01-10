#ifndef _DEMO0_H_
#define _DEMO0_H_
#include <iostream>

class A
{
public:
    A() = default;
    ~A() = default;

    static const int STATIC_CONST_INT = 0;
    static const double STATIC_CONST_DOUBLE;
};

void getStaticConstIntAddr0();
void getStaticConstIntAddr1();

#endif