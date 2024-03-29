#include <iostream>
#include <cstring>

using namespace std;

class Demo
{
public:
    Demo(const char *s = "helloworld") : ps(strlen(s) ? new char[strlen(s) + 1] : nullptr)
    {
        strcpy(ps, s);
        cout << "construct string: " << ps << endl;
    }

    Demo(const Demo &demo) : Demo(demo.ps)
    {
    }

    Demo &operator=(Demo &demo)
    {
        // for copy assignment, always do:
        // 1. identity test
        // 2. copy and swap
        if (this == &demo)
        {
            return *this;
        }
        Demo temp(demo);
        swap(temp);
        return *this;
    }

    ~Demo()
    {
        cout << "delete string: " << ps << endl;
        delete[] ps;
    }

    void show() const
    {
        cout << ps << endl;
    }

private:

    void swap(Demo &demo)
    {
        char *temp = demo.ps;
        demo.ps = ps;
        ps = temp;
    }

private:
    char *ps = nullptr;
};  

int main()
{
    Demo d0("Beat it!");
    Demo d1("Never meant");
    d0.show();
    d1.show();
    d1 = d0;
    d0.show();
    d1.show();
    return 0;
}
