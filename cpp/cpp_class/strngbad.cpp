#include <cstring>
#include "strngbad.h"

using std::cout;
using std::endl;

int StringBad::num_strings = 0;

StringBad::StringBad(const char *s)
{
    len = std::strlen(s);
    str = new char[len + 1];
    std::strcpy(str, s);
    num_strings++;
    cout << num_strings << ": \"" << str << "\" object created." << endl;
}

StringBad::StringBad()
{
    len = 3;
    str = new char[len + 1];
    std::strcpy(str, "C++");
    num_strings++;
    cout << num_strings << ": \"" << str << "\" object created." << endl;
}

// the copy constructor is used when an object is initialized:
// 1. initialization
// 2. function argument passing
// 3. function return
// the default copy constructor is shallow copy(field by field copy)
// shallow copy copies references by value
// deep copy duplicates the objects pointed by references
StringBad::StringBad(const StringBad &sb)
{
    len = sb.len;
    str = new char[len + 1];
    std::strcpy(str, sb.str);
    num_strings++;
    cout << num_strings << ": \"" << str << "\" object created." << endl;
}

StringBad::~StringBad()
{
    cout << "\"" << str << "\" object deleted, " << --num_strings << " left." << endl;
    delete[] str;
}

StringBad &StringBad::operator=(const StringBad &sb)
{
    if (this != &sb)
    {
        delete[] str;
        len = sb.len;
        str = new char[len + 1];
        std::strcpy(str, sb.str);
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const StringBad &s)
{
    os << s.str;
    return os;
}
