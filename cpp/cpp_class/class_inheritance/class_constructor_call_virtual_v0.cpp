#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeinfo>

using namespace std;

class Derived;

class Base {
private:
    typedef Base* (*Creator)();
    static std::map<string, Creator> _creators;

public:
    Base(int i = 10)
        : i(i)
    {
        cout << typeid(this).name() << endl;
    }

    void init()
    {
        foo();
        bar();
    }

    template <class T>
    static void registerDerived(std::string type)
    {
        _creators[type] = &creator<T>;
    }

    template <class T>
    static Base* createDerived(std::string type)
    {
        auto it = _creators.find(type);
        if (it != _creators.end()) {
            return it->second();
        }
        return nullptr;
    }

    virtual ~Base() = default;

private:
    template <class T>
    static Base* creator()
    {
        return new T;
    }

private:
    virtual void
    foo()
    {
        cout << "Base::foo" << endl;
    }
    virtual void bar() { cout << "Base::bar" << endl; }
    int i;
};

std::map<string, Base::Creator> Base::_creators = {};

class Derived : public Base {
public:
    Derived() = default;
    Derived(int i, int j)
        : Base(i), j(j)
    {
        cout << typeid(this).name() << endl;
    }

private:
    void foo() { cout << "Derived::foo" << endl; }
    void bar() { cout << "Derived::bar" << endl; }
    int j;
};

int main()
{
    Base::registerDerived<Derived>(typeid(Derived).name());
    Base* p = Base::createDerived<Derived>(typeid(Derived).name());
    delete p;
    return 0;
}
