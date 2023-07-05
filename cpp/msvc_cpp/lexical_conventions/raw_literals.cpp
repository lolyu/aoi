#include <cstddef>
#include <cstdio>

// Literal operator for user-defined INTEGRAL literal
void operator"" _dump(unsigned long long int lit)
{
    printf("operator \"\" _dump(unsigned long long int) : ===>%llu<===\n", lit);
};

// Literal operator for user-defined FLOATING literal
void operator"" _dump(long double lit)
{
    printf("operator \"\" _dump(long double)            : ===>%Lf<===\n", lit);
};

// Literal operator for user-defined CHARACTER literal
void operator"" _dump(char lit)
{
    printf("operator \"\" _dump(char)                   : ===>%c<===\n", lit);
};

void operator"" _dump(wchar_t lit)
{
    printf("operator \"\" _dump(wchar_t)                : ===>%d<===\n", lit);
};

void operator"" _dump(char16_t lit)
{
    printf("operator \"\" _dump(char16_t)               : ===>%d<===\n", lit);
};

void operator"" _dump(char32_t lit)
{
    printf("operator \"\" _dump(char32_t)               : ===>%d<===\n", lit);
};

// Literal operator for user-defined STRING literal
void operator"" _dump(const char *lit, size_t n)
{
    printf("operator \"\" _dump(const     char*, size_t): ===>%s<===\n", lit);
    printf("n == %ld\n", n);
};

void operator"" _dump(const wchar_t *lit, size_t n)
{
    printf("operator \"\" _dump(const  wchar_t*, size_t): ===>%ls<===\n", lit);
    printf("n == %ld\n", n);
};

void operator"" _dump(const char16_t *lit, size_t n)
{
    printf("operator \"\" _dump(const char16_t*, size_t):\n");
    printf("n == %ld\n", n);
};

void operator"" _dump(const char32_t *lit, size_t n)
{
    printf("operator \"\" _dump(const char32_t*, size_t):\n");
    printf("n == %ld\n", n);
};

// Raw literal operator
void operator"" _dump_raw(const char *lit)
{
    printf("operator \"\" _dump_raw(const char*)        : ===>%s<===\n", lit);
};

template <char...>
void operator"" _dump_template(); // Literal operator template

int main(int argc, const char *argv[])
{
    42_dump;
    3.1415926_dump;
    3.14e+25_dump;
    'A'_dump;
    L'B'_dump;
    u'C'_dump;
    U'D'_dump;
    "Hello World"_dump;
    L"Wide String"_dump;
    u8"UTF-8 String"_dump;
    u"UTF-16 String"_dump;
    U"UTF-32 String"_dump;
    42_dump_raw;
    3.1415926_dump_raw;
    3.14e+25_dump_raw;

    // There is no raw literal operator or literal operator template support on these types:
    //  'A'_dump_raw;
    // L'B'_dump_raw;
    // u'C'_dump_raw;
    // U'D'_dump_raw;
    //   "Hello World"_dump_raw;
    //  L"Wide String"_dump_raw;
    // u8"UTF-8 String"_dump_raw;
    //  u"UTF-16 String"_dump_raw;
    //  U"UTF-32 String"_dump_raw;
}
