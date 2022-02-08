#include <iostream>

using namespace std;

class A
{
public:
    A(int i = 10) : i(i) {}

private:
    int i;
};

class B : public A
{
public:
    B(int i = 10, int j = 11) : A(i), j(j) {}

    virtual void vdemo() = 0;

private:
    int j;
};

class C : public B
{
public:
    C(int i = 10, int j = 11, int k = 12) : B(i, j), k(k) {}

    virtual void vdemo() { cout << "helloworld" << endl; };

private:
    int k;
};

int main()
{
    C c;
    c.vdemo();
    return 0;
}