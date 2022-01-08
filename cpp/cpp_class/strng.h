#ifndef _STRNG_H_
#define _STRNG_H_
#include <iostream>

class SString
{
public:
    SString();

    SString(const char *s);

    SString(const SString &ss);

    ~SString();

    SString &operator=(const SString &ss);

    friend std::ostream &operator<<(std::ostream &os, const SString &ss);

private:
    char *mStr;
    int mSize;
    static int numObj;
};

#endif