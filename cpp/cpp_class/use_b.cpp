#include <iostream>
#include "b.h"

int main()
{
    using namespace std;
    demo::B b;
    demo::B::DoubleArray da{10};
    cout << demo::A::i << endl;
    cout << demo::B::i << endl;
    return 0;
}