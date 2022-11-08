#include <iostream>

struct Demo
{
    Demo() { std::cout << "Default constructor" << std::endl; }
    Demo(Demo &demo) { std::cout << "Copy constructor" << std::endl; }
    Demo(Demo &&demo) { std::cout << "Move constructor" << std::endl; }
};

void f(Demo &&demo)
{
    // it's especially important to bear in mind that a parameter is always an lvalue, even if its type is an rvalue reference.
    // https://stackoverflow.com/questions/65471068/why-is-function-parameter-always-an-lvalue
    // so demo here is always a lvalue, pass it to the Demo constructor always invokes Demo(Demo &demo)
    Demo demo_copy(demo);
    Demo demo_move(std::move(demo));
}

int main()
{
    Demo demo;
    f(std::move(demo));
    return 0;
}
