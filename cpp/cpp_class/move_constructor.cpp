#include <iostream>

#include "useless.h"

using namespace std;

int main()
{
    {
        Useless one(10, 'x');
        Useless two = one;
        Useless three(20, 'o');
        // copy elision here so the move constructor will be omitted
        Useless four(one + three);
        one.showData();
        two.showData();
        three.showData();
        four.showData();
    }
    return 0;
}