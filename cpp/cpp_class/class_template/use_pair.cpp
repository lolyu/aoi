#include <iostream>
#include <typeinfo>

#include "pairtp.h"

using namespace std;

int main()
{
    PairTP<const char *, const char *> p("hello", "world");
    cout << p.first() << " " << typeid(p.first()).name() << endl;
    cout << p.second() << " " << typeid(p.second()).name() << endl;
    PairTP<int, int> pi(10, 20);
    cout << pi.first() << endl;
    cout << pi.second() << endl;
    return 0;
}