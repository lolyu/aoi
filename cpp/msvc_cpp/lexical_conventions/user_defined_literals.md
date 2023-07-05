# user-defined literals
* six basic literals:
    * integer
    * float-point
    * character
    * string
    * bool
    * pointer

## user-defined literals
```cpp
return_type operator ""identifier(input)
```
* for the literal operator, only the following parameter lists are allowed:
```cpp
ReturnType operator "" _a(unsigned long long int);   // Literal operator for user-defined INTEGRAL literal
ReturnType operator "" _b(long double);              // Literal operator for user-defined FLOATING literal
ReturnType operator "" _c(char);                     // Literal operator for user-defined CHARACTER literal
ReturnType operator "" _d(wchar_t);                  // Literal operator for user-defined CHARACTER literal
ReturnType operator "" _e(char16_t);                 // Literal operator for user-defined CHARACTER literal
ReturnType operator "" _f(char32_t);                 // Literal operator for user-defined CHARACTER literal
ReturnType operator "" _g(const char*, size_t);      // Literal operator for user-defined STRING literal
ReturnType operator "" _h(const wchar_t*, size_t);   // Literal operator for user-defined STRING literal
ReturnType operator "" _i(const char16_t*, size_t);  // Literal operator for user-defined STRING literal
ReturnType operator "" _g(const char32_t*, size_t);  // Literal operator for user-defined STRING literal
ReturnType operator "" _r(const char*);              // Raw literal operator
template<char...> ReturnType operator "" _t();       // Literal operator template
```
* `_r` and `t` are for raw literals to provide a custom interpretation of an input sequence that's different than the compiler's normal behavior.
    * raw literals only work for integral and floating-point user-defined literals


## references
* https://en.cppreference.com/w/cpp/language/user_literal
