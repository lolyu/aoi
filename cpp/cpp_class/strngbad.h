#ifndef _STRNGBAD_H_
#define _STRNGBAD_H_
#include <iostream>

class StringBad
{
private:
    char *str;
    int len;
    static int num_strings;

public:
    StringBad(const char *s);
    StringBad();
    StringBad(const StringBad &sb);
    ~StringBad();
    StringBad &operator=(const StringBad &sb);

    friend std::ostream &operator<<(std::ostream &os, const StringBad &s);
};

#endif
