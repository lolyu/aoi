#include <iostream>
#include <string>

class Demo
{
public:
    Demo(const std::string &s);
    virtual ~Demo();

private:
    std::string m_str;
};

Demo::Demo(const std::string &s) : m_str(s)
{
    std::cout << "Init " << m_str << std::endl;
}

Demo::~Demo()
{
    std::cout << "Destruct " << m_str << std::endl;
}

int main()
{
    // Init d0
    // Init d1
    // Init static d2
    // Init d3
    // Destruct d3
    // Destruct d1
    // Destruct d0
    Demo d0("d0");
    {
        Demo d1("d1");
        static Demo d2("static d2");
        Demo d3("d3");
    }
    return 0;
}
