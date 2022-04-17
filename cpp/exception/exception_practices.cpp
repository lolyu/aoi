#include <iostream>
#include <stdexcept>
#include <string>

class Base
{
public:
    class base_exception : public std::logic_error
    {
    public:
        base_exception(const std::string &s);
        const char *what() const noexcept override;

    protected:
        std::string msg;
    };
};

class Derived : public Base
{
public:
    class derived_exception : public base_exception
    {
    public:
        derived_exception(const std::string &s);
    };
};

int main()
{
    auto ec0 = Derived::derived_exception("hello_exception");
    std::cout << ec0.what() << std::endl;
    auto ec1 = Base::base_exception("hello_exception");
    std::cout << ec1.what() << std::endl;
    return 0;
}

Base::base_exception::base_exception(const std::string &s)
    : logic_error(s)
{
    msg = "base_exception: ";
    msg += logic_error::what();
}

const char *Base::base_exception::what() const noexcept
{
    return msg.c_str();
}

Derived::derived_exception::derived_exception(const std::string &s)
    : base_exception(s)
{
    msg = "derived_exception: ";
    msg += logic_error::what();
}
