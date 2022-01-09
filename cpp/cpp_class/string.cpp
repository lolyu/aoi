#include <cstring>
#include "string.h"

int String::numObjects = 0;
const int String::demoConst0 = 101;
char String::CINBUFFER[String::CINLIM];

void String::initStr(const char *s, int allocatedSize)
{
    mStrSize = std::strlen(s);
    if (allocatedSize == -1)
    {
        mLength = mStrSize + 1;
    }
    else if (allocatedSize >= 0)
    {
        mLength = std::max(static_cast<size_t>(allocatedSize), mStrSize + 1);
    }
    mStr = new char[mLength];
    std::strcpy(mStr, s);
}

String::String()
{
    std::cout << "Default constructor called" << std::endl;
    initStr("", 4);
    numObjects++;
}

String::String(const char *s)
{
    std::cout << "Custom constructor called" << std::endl;
    initStr(s);
    numObjects++;
}

String::String(const String &ss)
{
    std::cout << "Copy constructor called" << std::endl;
    initStr(ss.mStr);
    numObjects++;
}

String::~String()
{
    std::cout << "Destructor called" << std::endl;
    delete[] mStr;
    numObjects--;
}

String &String::operator=(const String &ss)
{
    std::cout << "Assignment called" << std::endl;
    if (this != &ss)
    {
        delete[] mStr;
        initStr(ss.mStr);
    }
    return *this;
}

String &String::operator=(const char *s)
{
    std::cout << "Assignment called" << std::endl;

    delete[] mStr;
    initStr(s);
    return *this;
}

std::ostream &operator<<(std::ostream &os, const String &ss)
{
    os << ss.mStr;
    return os;
}

std::istream &operator>>(std::istream &is, String &ss)
{
    is.get(String::CINBUFFER, String::CINLIM);
    if (is)
    {
        delete[] ss.mStr;
        ss.initStr(String::CINBUFFER);
    }
    while (is && is.get() != '\n')
    {
        continue;
    }
    return is;
}

bool operator<(const String &ls, const String &rs)
{
    return std::strcmp(ls.mStr, rs.mStr) < 0;
}

bool operator>(const String &ls, const String &rs)
{
    return rs < ls;
}

bool operator==(const String &ls, const String &rs)
{
    return strcmp(ls.mStr, rs.mStr) == 0;
}

char &String::operator[](int i)
{
    return mStr[i];
}

// C++ allows class methods to be overloaded based on its constness
// const function could only return const reference/pointer or call const methods
const char &String::operator[](int i) const
{
    return mStr[i];
}

// if we put static here, C++ regards this member function local to this file(internal linkage)
// if multiple translation units all define their own implementations of this member function with static to avoid collision
// at linking, calling this member function from different places will result in different behavior
// https://stackoverflow.com/questions/45277976/why-can-static-member-function-definitions-not-have-the-keyword-static
int String::howMany()
{
    return numObjects;
}

String::operator charp_t() const
{
    return mStr;
}
