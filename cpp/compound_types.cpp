#include <iostream>
#include <cstring>
#include <cstddef>

/*
struct
syntax:
```
struct attr-spec-seq name { struct declaration list }
```
* attr-spec-seq: attributes
* struct declaration list: variable declarations, bit field operations, static assert declarations
* a pointer to a struct can be cast to a pointer to its first member, and vice versa
* for struct with flexible array member, if the member is accessed via . or ->, the struct behaves as if the array member had the longest size fitting in the memory allocated for this object
* an unnamed member of struct whose type is struct without name is known as anonymous struct, it is considering as a member of the enclosing struct/union
* NOTE: struct definition doesn't establish scope, so its member variables are visible in the surrounding scope where the struct is defined

union
* a pointer to a union can be cast to a pointer to each of its members, and vice versa
* every member of an anonymous union is considered to be a member of the enclosing struct or union.

enum
* the enum definition could starts with either `enum`, `enum class` or `enum struct`
* unscoped enumeration: defined with `enum`
* unscoped enumerators can be implicitly converted to their underlying type
* if the enumerator-list is empty, the underlying type is as if the enumeration had a single enumerator with value 0
* the name of an unscoped enumeration may be omitted: such declaration only introduces the enumerators into the enclosing scope
* scoped enumeration: defined with `enum class` or `enum struct`

initialization
* all members are not initialized explicitly are zero-initialzed(struct, union, enum)
* when initializing a union, the initializer list must have only one member, which initializes the first member of the union unless a designated initializer is used
*

references:
* https://en.wikipedia.org/wiki/Flexible_array_member
* https://en.cppreference.com/w/c/language/struct
* https://en.cppreference.com/w/c/language/struct_initialization
* https://en.cppreference.com/w/c/language/_Static_assert
* https://en.cppreference.com/w/c/language/union
* https://en.cppreference.com/w/cpp/language/enum
*/

struct S1
{
    short num;
    long size;
};

struct S2
{
    int n;
    char name[]; // incomplete type
};

struct S3
{
    struct
    {
        int n;
        short m;
        long long i;
    };
};

enum E1
{
    a,
    b,
    c = 10,
    d,
    e = 1,
    f,
    g = f + c
};

struct X
{
    enum direction
    {
        left = 'l',
        right = 'r'
    };
};

enum class fruit
{
    orange,
    apple
};

int main()
{
    using namespace std;
    auto s1p = new S1{20, 10};
    cout << s1p->size << endl;
    cout << s1p->num << endl;
    auto fp = &s1p->num;
    cout << reinterpret_cast<S1 *>(fp)->num << endl;
    cout << reinterpret_cast<S1 *>(fp)->size << endl;
    delete s1p;

    // S2 s2{1, {3.2}};
    // initialization, sizeof, and assignment ignores the flexible array
    S2 s2;
    cout << sizeof(s2) << endl;
    cout << s2.name[20] << endl; // undefined behavior
    S2 *s2p = reinterpret_cast<S2 *>(malloc(sizeof(S2) + 20));
    strcpy(s2p->name, "helloworld");
    cout << s2p->name << endl;
    free(s2p);

    S3 s3{10, 20};
    cout << s3.m << endl;
    cout << s3.n << endl;
    cout << offsetof(S3, S3::i) << endl;
    // cout << offsetof(S3, i) << endl;

    E1 e = a;
    int t = d;
    cout << t << endl;

    X x;
    cout << (char)x.left << endl;
    cout << (char)X::right << endl;

    int i = static_cast<int>(fruit::apple);
    cout << i << endl;

    enum byte : unsigned char
    {
    };                              // byte is a new integer type
    byte b = static_cast<byte>(42); // OK as of C++17 (direct-list-initialization)

    cout << b << endl;

    enum unscoped
    {
        banana,
        apple
    };
    i = 1;
    switch (i)
    {
    case apple:
        cout << "APPLE" << endl;
        break;
    case banana:
        cout << "BANANA" << endl;
        break;
    }
    return 0;
}