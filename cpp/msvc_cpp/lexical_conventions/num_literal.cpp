#include <iostream>
#include <typeinfo>

using namespace std;

// ./a.out | c++filt -t

int main()
{
    auto i0 = 100;
    auto i1 = 100000111110000;
    auto i2 = 24'847'458'121;
    auto i3 = 23ul;
    auto i4 = 0b000101;

    cout << "int literal i0, type " << typeid(i0).name() << ", size " << sizeof(i0) << "." << endl;
    cout << "int literal i1, type " << typeid(i1).name() << ", size " << sizeof(i1) << "." << endl;
    cout << "int literal i2, type " << typeid(i2).name() << ", size " << sizeof(i2) << "." << endl;
    cout << "int literal i3, type " << typeid(i3).name() << ", size " << sizeof(i3) << "." << endl;
    cout << "int literal i4, type " << typeid(i4).name() << ", size " << sizeof(i4) << "." << endl;

    auto f0 = 0.001;
    auto f1 = 0.00001f;
    auto f2 = 0.0000l;
    cout << "int literal f0, type " << typeid(f0).name() << ", size " << sizeof(f0) << "." << endl;
    cout << "int literal f1, type " << typeid(f1).name() << ", size " << sizeof(f1) << "." << endl;
    cout << "int literal f2, type " << typeid(f2).name() << ", size " << sizeof(f2) << "." << endl;

    return 0;
}
