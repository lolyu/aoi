#include <iostream>
#include <typeinfo>

using namespace std;

// ./a.out | c++filt -t

int main()
{
    auto c0 = 'c';
    auto c1 = u8'A';
    auto c2 = L'c';
    auto c3 = u'c';
    auto c4 = U'c';
    auto c5 = 'abcd'; // multi-character constant

    cout << "int literal c0, type " << typeid(c0).name() << ", size " << sizeof(c0) << "." << endl;
    cout << "int literal c1, type " << typeid(c1).name() << ", size " << sizeof(c1) << "." << endl;
    cout << "int literal c2, type " << typeid(c2).name() << ", size " << sizeof(c2) << "." << endl;
    cout << "int literal c3, type " << typeid(c3).name() << ", size " << sizeof(c3) << "." << endl;
    cout << "int literal c4, type " << typeid(c4).name() << ", size " << sizeof(c4) << "." << endl;
    cout << "int literal c5, type " << typeid(c5).name() << ", size " << sizeof(c5) << "." << endl;

    return 0;
}
