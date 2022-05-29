#include "useless.h"

#include <iostream>

using namespace std;

int Useless::ct = 0;

Useless::Useless()
{
    ++ct;
    n = 0;
    pc = nullptr;
    cout << "default constructor called; number of objects: " << ct << endl;
    showObject();
}

Useless::Useless(int k)
    : n(k), pc(new char[n])
{
    ++ct;
    cout << "int constructor called; number of objects: " << ct << endl;
    showObject();
}

Useless::Useless(int k, char ch)
    : n(k), pc(new char[n])
{
    ++ct;
    cout << "int char constructor called; number of objects: " << ct << endl;
    for (int i = 0; i < n; ++i) {
        pc[i] = ch;
    }
    showObject();
}

Useless::Useless(const Useless &f)
    : n(f.n), pc(new char[n])
{
    ++ct;
    cout << "copy constructor called; number of objects: " << ct << endl;
    for (int i = 0; i < n; ++i) {
        pc[i] = f.pc[i];
    }
    showObject();
}

Useless::Useless(Useless &&f)
    : n(f.n), pc(f.pc)
{
    ++ct;
    cout << "move constructor called; number of objects: " << ct << endl;
    f.pc = nullptr;
    f.n = 0;
    showObject();
}

Useless::~Useless()
{
    cout << "destructor called; objects left: " << --ct << endl;
    cout << "deleted object:" << endl;
    showObject();
    delete[] pc;
}

Useless Useless::operator+(const Useless &f) const
{
    cout << "entering operator+()" << endl;
    Useless temp = Useless(n + f.n);
    for (int i = 0; i < n; ++i) {
        temp.pc[i] = pc[i];
    }
    for (int i = 0; i < temp.n; ++i) {
        temp.pc[n + i] = f.pc[i];
    }
    cout << "temp object:" << endl;
    cout << "leaving operator+()" << endl;
    return temp;
}

Useless &Useless::operator=(const Useless &f)
{
    if (this == &f) {
        return *this;
    }
    delete[] pc;
    n = f.n;
    pc = new char[n];
    for (int i = 0; i < n; ++i) {
        pc[i] = f.pc[i];
    }
    return *this;
}

Useless &Useless::operator=(Useless &&f)
{
    if (this == &f) {
        return *this;
    }
    delete[] pc;
    n = f.n;
    pc = f.pc;
    f.n = 0;
    f.pc = nullptr;
    return *this;
}

void Useless::showObject() const
{
    cout << "number of elements: " << n << endl;
    cout << "data address: " << static_cast<void *>(pc) << endl;
}

void Useless::showData() const
{
    if (n == 0) {
        cout << "(object empty)";
    } else {
        for (int i = 0; i < n; ++i) {
            cout << pc[i];
        }
    }
    cout << endl;
}
