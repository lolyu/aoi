#include <iostream>

class Base
{
public:
    void nonvirtul_func() const
    {
        std::cout << "nonvirtul_func func from base" << std::endl;
    }

    virtual void virtual_func() const
    {
        std::cout << "virtual func from base" << std::endl;
    }

    void print() const
    {
        std::cout << "print fron Base" << std::endl;
        nonvirtul_func();
        virtual_func();
    }
};

class Derived : public Base
{
public:
    void virtual_func() const override
    {
        std::cout << "virtual func from derived" << std::endl;
    }

    void nonvirtul_func() const
    {
        std::cout << "nonvirtul_func func from derived" << std::endl;
    }
};

int main()
{
    Derived d0;
    d0.print();
    return 0;
}
