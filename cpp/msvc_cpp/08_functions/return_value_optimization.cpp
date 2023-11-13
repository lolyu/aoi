#include <iostream>
using namespace std;

class ABC
{
public:
    const char *a;
    ABC()
    {
        cout << "Constructor" << endl;
    }
    ABC(const char *ptr)
    {
        cout << "Constructor" << endl;
    }
    ABC(const ABC &obj)
    {
        cout << "copy constructor" << endl;
    }
    ABC(ABC &&obj)
    {
        cout << "Move constructor" << endl;
    }
    ~ABC()
    {
        cout << "Destructor" << endl;
    }
};

ABC fun123()
{
    ABC obj;
    return obj;
}

ABC xyz123()
{
    return ABC();
}

int main()
{
    // constructor
    ABC abc;
    cout << endl;

    // without NRVO: constructor -> copy/move constructor -> destructor -> copy/move constructor -> destructor
    // with NRVO: constructor
    ABC obj1(fun123()); // NRVO
    cout << endl;

    // without NRVO: constructor -> copy/move constructor -> destructor -> copy/move constructor -> destructor
    // with NRVO: constructor
    ABC obj2(xyz123()); // RVO, not NRVO
    cout << endl;

    // without NRVO: constructor -> move constructor
    // with NRVO: constructor
    ABC xyz = "Stack Overflow"; // RVO
    return 0;
}
