#include <iostream>

#include "stacktp.h"

template <template <typename T> class Container>
class Crab {
private:
    Container<int> s1;
    Container<double> s2;

public:
    Crab() = default;
    bool push(int a, double x);
    bool pop(int &a, double &x);
};

template <template <typename T> class Container>
bool Crab<Container>::push(int a, double x)
{
    return s1.push(a) && s2.push(x);
}

template <template <typename T> class Container>
bool Crab<Container>::pop(int &a, double &x)
{
    return s1.pop(a) && s2.pop(x);
}

int main()
{
    using namespace std;
    Crab<Stack> nebula;
    int a;
    double x;
    nebula.push(10, 10);
    nebula.push(20, 20);
    nebula.push(30, 30);
    nebula.pop(a, x);
    cout << a << " " << x << endl;
    return 0;
}
