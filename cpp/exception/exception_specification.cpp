#include <iostream>

using namespace std;

/*
noexcept = noexept(true) = throw(): means the function doesn't throw an exception
noexcept(false) = throw(...): means the function can throw an exception of any type
throw(type): (C++ 14 and earlier), means the function can throw an exception of specific type

NOTE: if any function called between the one throws an exception and the one that handles the
exception are specified as `noexcept`, `noexcept(false)` or throw(), the program is terminated when noexcept
function propagates the exception.
*/

void func0();
void func1() noexcept;
void func2();
void func3();

int main()
{
    try {
        func0();
    } catch (const exception &ec) {
        cout << ec.what() << endl;
    }
    return 0;
}

void func0()
{
    cout << "func0: START" << endl;
    func1();
    cout << "func0: END" << endl;
}

// the exception propagation will stop here as func1 is noexcept
void func1() noexcept
{
    cout << "func1: START" << endl;
    func2();
    cout << "func1: END" << endl;
}

void func2()
{
    cout << "func2: START" << endl;
    throw runtime_error("in func2");
    cout << "func2: END" << endl;
}
