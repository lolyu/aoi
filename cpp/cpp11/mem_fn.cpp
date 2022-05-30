#include <functional>
#include <iostream>

class A
{
public:
    A(int a)
        : a(a) {}

    A multiply(const A &other)
    {
        return A(a * other.a);
    }

    friend std::ostream &operator<<(std::ostream &os, const A &a);

private:
    int a;
};

std::ostream &operator<<(std::ostream &os, const A &a)
{
    os << a.a;
    return os;
}

int main()
{
    auto fmultiply = std::bind(&A::multiply, std::placeholders::_1, std::placeholders::_2);
    auto mmultiply = std::mem_fn(&A::multiply);
    A a0(100);
    A a1(200);
    auto a2 = fmultiply(a0, a1);
    auto a3 = mmultiply(a0, a1);
    std::cout << a2 << std::endl;
    std::cout << a3 << std::endl;
    return 0;
}