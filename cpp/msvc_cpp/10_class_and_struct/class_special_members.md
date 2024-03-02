# C++ class special members
* special members are those that compiler can be asked to automatically generated code for:
    * default constructor
    * destructor
    * copy constructor && copy assignment operator
    * move constructor && move assignment operator

* special member functions can be explicitly defaulted with `= default`
* special member functions can be explicitly deleted with `= deleted`

* the special member can be:
    * `not-declared`
    * `implicitly-declared`
        * `deleted`
        * `defaulted`
    * `user-declared`
        * `deleted`
        * `defaulted`
        * `user-defined`

* `= delete` doesn't mean the function is dispeared, **deleted members still participate in overload resolution**
* `deleted` vs `not-decalred`:
    * `not-declared` -> member doesn't exist -> does not participate in overload resolution
    * `deleted` -> member exists -> participates in overload resolution


```cpp
struct X
{
    X() {}              // user-declared, user-defined
    X();                // user-declared, user-defined
    X() = default;      // user-declared, defaulted
    X() = delete;       // user-declared, deleted
};

struct X
{
    template <class ...Args> X(Args&& ...args);
    // the default destructor is not declared
};

struct X
{
    template <class ...Args> X(Args&& ...args);
    X() = default;      // user-declared, defaulted
};

struct X
{
    template <class ...Args> X(Args&& ...args);
    X() = delete;       // user-declared, deleted
};
```

## special members implicitly declaration rule
![image](https://github.com/lolyu/aoi/assets/35479537/c7fc68f4-4f88-44f5-b2dd-e614c31386e4)
* if the defaulted special member would have to do something illegal(calling another deleted function), the `defaulted` can mean `deleted`
    * in this context, `defaulted` move members -> `deleted` move members
        * the `deleted` move members behaves like `not-declared`, means that those move members don't exist and don't participate in method overload resolution
* if the user declares any non-special constructor, this will inhibit the implicit declaration of the default constructor - `not-declared`
* user-declared default constructor will not inhibit any other special member.
* a user-declared destructor:
    * will inhibit the implicit declaration of the move members
        * **rule of five**
    * deprecate the implicitly-defaulted copy members
        * **rule of three**
* a user-declared copy constructor will inhibit the default constructor and move members.
* a user-declared copy assignment will inhibit the move members
* a user-declared move member will implicitly delete the copy members

## class design rules for special members

### rule of zero
* let the compiler generates the special member functions or use `= default` to explicitly declare them.

### rule of three
* when you have a destructor for the class, which means that the class is managing resources, it is better to define copy constructor and copy assignment to ensure deep copy.
* use the copy-and-swap idiom to implement the copy assignment

### rule of five
* with rule of three, if you want to enable the class to support move semantics, and the move constructor && assignment operator can provide `noexcept` guarantee and can be more efficiently than copy, it is better to define move constructor and move assignment operator
```cpp
#include <iostream>
#include <cstddef>
#include <cstring>
#include <utility>

class CString
{
public:
    CString(const char *str = "") : _str(nullptr)
    {
        if (str)
        {
            size_t len = strlen(str);
            _str = new char[len];
            std::memcpy(_str, str, len);
            _str[len] = '\0';
        }
    }

    ~CString()
    {
        delete[] _str;
    }

    CString(const CString &cs) : CString(cs._str)
    {
        // copy constructor
    }

    CString &operator=(const CString &cs)
    {
        // copy assignment
        // implement with copy constructor and move assignment
        return *this = CString(cs);
    }

    CString(CString &&cs) noexcept : CString(std::exchange(cs._str, nullptr))
    {
        // move constructor
    }

    CString &operator=(CString &&cs) noexcept
    {
        // move assignment
        if (this != &cs)
        {
            _str = std::exchange(cs._str, nullptr);
        }
        return *this;
    }

    void print() const
    {
        std::cout << (_str ? _str : "Empty CString!!!") << std::endl;
    }

private:
    char *_str;
};

int main()
{
    CString cs0("HelloWorld");
    CString cs1("HelloSONiC");
    
    cs0.print();
    cs1.print();
    cs0 = cs1;
    cs0.print();
    cs1.print();
    cs0 = std::move(cs1);
    cs0.print();
    cs1.print();
    return 0;
};
```

## references
* https://cplusplus.com/doc/tutorial/classes2/
* https://safecpp.com/RuleOfDesDeMovA.html
* https://accu.org/conf-docs/PDFs_2014/Howard_Hinnant_Accu_2014.pdf
* https://www.youtube.com/watch?v=vLinb2fgkHk
* https://en.cppreference.com/w/cpp/language/rule_of_three
