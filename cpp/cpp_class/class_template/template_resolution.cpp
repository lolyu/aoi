#include <functional>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

using namespace std;

class Base
{
public:
    template <typename T>
    void foo(const T &t)
    {
        bar(t);
        funcList.push_back([this, t]() { bar(t); });
    }

    template <typename T>
    void bar(const T &t)
    {
        cout << typeid(t).name() << endl;
        cout << "Base::bar(template) " << t << endl;
    }

    virtual void bar(const string &s)
    {
        cout << "Base::bar(string) " << s << endl;
    }

    const vector<function<void()>> &getFunctionList()
    {
        return funcList;
    }

private:
    vector<function<void()>> funcList;
};

class Derived : public Base
{
public:
    virtual void bar(const string &s) override
    {
        cout << "Derived::bar(string) " << s << endl;
    }
};

int main()
{
    Base *bp = new Derived();
    bp->foo(100);
    bp->foo(string("Helloworld"));
    bp->foo('f');
    for (auto func : bp->getFunctionList()) {
        func();
    }
    delete bp;
    return 0;
}