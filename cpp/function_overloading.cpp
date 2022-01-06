#include <iostream>

using namespace std;

/*
function signature
    * function name
    * function parameter number
    * function parameter type
    * function parameter ordering
*/

int sumup(int, int);
// double sumup(int, int);              Note return type is not included in function signature

// the ANSI C++ standard says that const and & should not be used to determine a function's signature.
int sumup(int &, int &);
int sumup(const int, const int);
int sumup(const int &, const int &);

void func0(double &d);       // matches modifiable lvalue
void func0(const double &d); // matches const lvable and rvalue

void func1(double &d);       // matches modifiable lvalue
void func1(const double &d); // matches const lvable
void func1(double &&d);      // matches rvable

int main()
{
    double d0 = 20.0f;
    const double d1 = 30.0f;
    func0(d0);
    func0(d1);
    func0(40.0f);
    func1(d0);
    func1(d1);
    func1(40.0f);
    return 0;
}

void func0(double &d)
{
    cout << "func0(double &d)" << endl;
}

void func0(const double &d)
{
    cout << "func0(const double &d)" << endl;
}

void func1(double &d)
{
    cout << "func1(double &d)" << endl;
}

void func1(const double &d)
{
    cout << "func1(const double &d)" << endl;
}

void func1(double &&d)
{
    cout << "func1(double &&d)" << endl;
}
