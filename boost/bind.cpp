// #include <boost/bind.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <algorithm>

using namespace std;

int f(int a, int b)
{
    return a + b;
}

int g(int a, int b, int c)
{
    return a + b + c;
}

int h(int &a, int &b)
{
    return a++ + b++;
}

void print(int x)
{
    cout << x << endl;
}

struct F2
{
    int s;

    typedef void result_type;
    void operator()(int x) { s += x; }
};

class A
{
public:
    int m_a = 0;
    void inc(int x)
    {
        m_a += x;
    }
};

int main()
{
    int a = 10, b = 20;
    print(boost::bind(f, 10, 20)());                                                 // 30
    print(boost::bind(f, boost::placeholders::_1, boost::placeholders::_1)(10, 20)); // 20
    print(boost::bind(h, a, boost::placeholders::_2)(a, b));                         // 30
    print(a);                                                                        // 10
    print(b);                                                                        // 21
    // values are stored into the function object by copy of value, use boost::ref or boost::cref to store values by reference
    print(boost::bind(h, boost::ref(a), boost::placeholders::_2)(a, b)); // 31
    print(a);                                                            // 11
    print(b);                                                            // 22

    F2 f2 = {0};
    int ar[] = {1, 2, 3};
    // By default, bind makes a copy of the provided function object
    std::for_each(ar, ar + 3, boost::bind(f2, boost::placeholders::_1));
    print(f2.s); // 0
    std::for_each(ar, ar + 3, boost::bind(boost::ref(f2), boost::placeholders::_1));
    print(f2.s); // 6

    A ao;
    boost::bind(&A::inc, ao, boost::placeholders::_1)(100);
    print(ao.m_a); // 0
    boost::bind(&A::inc, boost::ref(ao), boost::placeholders::_1)(100);
    print(ao.m_a); // 100
    return 0;
}