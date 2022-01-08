#include <cstring>
#include "strng.h"

using namespace std;

int SString::numObj = 0;

SString::SString()
{
    mSize = 10;
    mStr = new char[mSize + 1];
    numObj++;
}

SString::SString(const char *s)
{
    mSize = strlen(s);
    mStr = new char[mSize + 1];
    strcpy(mStr, s);
    numObj++;
}

SString::SString(const SString &ss)
{
    mSize = ss.mSize;
    mStr = new char[mSize + 1];
    strcpy(mStr, ss.mStr);
    numObj++;
}

SString::~SString()
{
    delete[] mStr;
}

SString &SString::operator=(const SString &ss)
{
    if (&ss != this)
    {
        mSize = ss.mSize;
        delete mStr;
        mStr = new char[mSize + 1];
        strcpy(mStr, ss.mStr);
    }
    return *this;
}

ostream &operator<<(ostream &os, const SString &ss)
{
    os << ss.mStr;
}
