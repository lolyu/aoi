#include <iostream>
#include <string>

using namespace std;

class A
{
public:
    A(int priv_a = 10, int pub_a = 10) : private_a(priv_a), public_a(pub_a) {}
    int public_a;

private:
    int private_a;
};

class B : private A
{
public:
    B(int priv_a = 100, int pub_a = 100) : A(priv_a, pub_a) {}
    //
    // this introduce public_a into the scope of class B
    using A::public_a;
    // using A::private_a;
    // this will fail because private_a is not accessible via A, it is a private member
};

int main()
{
    B b;
    cout << b.public_a << endl;
    return 0;
}