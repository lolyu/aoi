#include <iostream>

// lvalue reference: appear on the left side of assignment operator, could apply & operator to take its address
// rvalue reference: appear on the right side of assignment operator, could not apply & operator to take its address

inline double f(double tf)
{
    return tf * 11.1;
}

int main()
{
    using namespace std;
    double tc = 25;
    double &&rv0 = 11.1;
    double &&rv1 = f(rv0);
    cout << "Value: " << tc << ", Address: " << &tc << endl;
    cout << "Value: " << rv0 << ", Address: " << &rv0 << endl;
    cout << "Value: " << rv1 << ", Address: " << &rv1 << endl;
    return 0;
}