#include "demo.h"

// redefinition here
// int i = 100;
int b = 10;
// int A::static_member = 10;

void getStaticMemberAddr(const int *p)
{
    p = &(A::const_static_member);
}