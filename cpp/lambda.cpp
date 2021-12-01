#include <iostream>

class A
{
public:
    A(int a = 10) : m_a(a) {}

    int get()
    {
        return m_a;
    }

    int method()
    {
        // lambda to reference class members
        return [this]()
        { return m_a += 10; }();
    }

private:
    int m_a;
};

int main()
{
    using namespace std;
    A a;

    cout << a.method() << endl;
    cout << a.get() << endl;

    int x = 100;
    auto f = [=]() mutable
    { return x += 10; };
    cout << f() << endl;
    cout << f() << endl;
    cout << x << endl;

    // // noexcept not working
    // []() noexcept
    // { throw std::runtime_error("aloha"); }();

    // []() throw()
    // {
    //     throw std::runtime_error("aloha");
    // }
    // ();
    return 0;
}