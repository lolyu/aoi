#include <iostream>
#include <memory>

using namespace std;

/*
https://stackoverflow.com/questions/8140801/how-do-i-hand-out-weak-ptrs-to-this-in-my-constructor
https://stackoverflow.com/questions/27085782/how-to-break-shared-ptr-cyclic-reference-using-weak-ptr
*/

struct A0;

struct B0
{
    A0 &a;
    B0(A0 &a) : a(a) {}
    ~B0() { cout << "Destroy B0" << endl; }
};

struct A0
{
    B0 b;
    A0() : b(*this) {}
    ~A0() { cout << "Destroy A0" << endl; }
};

struct A1;

struct B1
{
    A1 *a;
    B1(A1 *a) : a(a) {}
    ~B1() { cout << "Destroy B1" << endl; }
};

struct A1
{
    B1 *b;
    A1() : b(new B1(this)) {}
    ~A1()
    {
        delete b;
        cout << "Destroy A1" << endl;
    }
};

struct A2;

struct B2
{
    shared_ptr<A2> a;
    B2(A2 *a) : a(a) {}
    ~B2() { cout << "Destroy B2" << endl; }
};

struct A2
{
    shared_ptr<B2> b;
    A2() : b(make_shared<B2>(this)) {}
    ~A2() { cout << "Destroy A2" << endl; }
};

struct A3;

struct B3
{
    weak_ptr<A3> wa;
    B3(shared_ptr<A3> a) { wa = a; }
    ~B3() { cout << "Destroy B3" << endl; }
};

struct A3 : public enable_shared_from_this<A3>
{
    shared_ptr<B3> b;
    A3() : b(make_shared<B3>(shared_from_this())){};
    ~A3() { cout << "Destroy A3" << endl; }
};

int main()
{
    A0 *a0p = new A0();
    delete a0p;

    A1 *a1p = new A1();
    delete a1p;

    // A2 *a2p = new A2();
    // delete a2p;

    {
        auto a3p = make_shared<A3>();
    }

    return 0;
}