#include <iostream>
#include <string>

class CustomException;
class Demo;

void f0();
void f1();
void f2();
void f3();

/*
https://docs.microsoft.com/en-us/cpp/cpp/exceptions-and-stack-unwinding-in-cpp?view=msvc-170
stack unwinding happens after an exception thrown is matched by a catch handler.
stack unwinding will call the destructors of all automatic objects that are fully constructed but not destructed between:
    * the beiginning of the try block assciated with the matching catch handler
    * the throw statement that throws the exception
The destructors will be called in the reverse order of objects' constructions
*/

int main()
{
    try {
        f0();
    } catch (...) {
        std::exception_ptr p = std::current_exception();
        std::cout << (p ? p.__cxa_exception_type()->name() : "NULL") << std::endl;
    }

    f0();
    return 0;
}

class CustomException
{
public:
    CustomException(const std::string &message)
        : message(message) {}

    std::string msg() const
    {
        return message;
    }

private:
    std::string message;
};

class Demo
{
public:
    Demo(const std::string &location = "UNKNOWN")
        : location(location)
    {
        std::cout << "Demo::Demo[" + location + "]" << std::endl;
    }

    ~Demo()
    {
        std::cout << "Demo::~Demo[" + location + "]" << std::endl;
    }

private:
    std::string location;
};

void f0()
{
    Demo d0("f0");
    f1();
    std::cout << "f0 ends" << std::endl;
}

void f1()
{
    Demo d1("f1");
    f2();
    std::cout << "f1 ends" << std::endl;
}

void f2()
{
    Demo d2("f2");
    try {
        f3();
    } catch (const CustomException &e) {
        std::cout << e.msg() << std::endl;
        throw;
    }
    std::cout << "f2 ends" << std::endl;
}

void f3()
{
    Demo d3("f3");
    throw CustomException("HelloWorld");
    std::cout << "f3 ends" << std::endl;
}
