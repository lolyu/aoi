#ifndef _STRING_H_
#define _STRING_H_
#include <iostream>

typedef char *charp_t;

class String
{
public:
    String();

    String(const char *s);

    String(const String &ss);

    ~String();

    String &operator=(const String &ss);

    String &operator=(const char *s);

    size_t size() const { return mStrSize; }

    friend std::ostream &operator<<(std::ostream &os, const String &ss);

    friend std::istream &operator>>(std::istream &is, String &ss);

    friend bool operator<(const String &ls, const String &rs);

    friend bool operator>(const String &ls, const String &rs);

    friend bool operator==(const String &ls, const String &rs);

    char &operator[](int i);

    const char &operator[](int i) const;

    explicit operator charp_t() const;

    static int howMany();

    static int getObjectNum() { return numObjects; }

    // static int demoConst0 = 99;                  C++ forbids in-class initialization of non-const static member
    // const static int demoConst0 = 99;            C++ allows in-class initialization of const static int member
    // const static double demoConst0 = 99;         C++ forbids in-class initialization of const static non-int member
    const static int demoConst0;

private:
    void initStr(const char *s, int allocatedSize = -1);

    char *mStr;
    size_t mLength;
    size_t mStrSize;
    static int numObjects;
    static const size_t CINLIM = 80;
    // forward declaration
    static char CINBUFFER[];
};

#endif
