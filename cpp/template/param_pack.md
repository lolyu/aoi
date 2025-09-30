# param pack
* a `variadic template` is a template whose definition captures a `parameter pack` in its **template arguments** or **function arguments**.
    * a parameter pack is captured by introducing an identifier prefixed by `...`
    * a parameter pack can later be used in a pattern expanded by `...`

## expand a parameter pack
A parameter pack can be expanded in the following contex:
1. in `initializer-lists` or in the arguments to a function call.
```cpp
void print_strings(std::convertible_to<std::string_view> auto &&...strs)
{
    for (auto s : std::initializer_list<std::string_view>{strs...})     // expand to initializer list
    {
        std::cout << s << std::endl;
    }
}
```

2. in base specifier lists or member-initializer list
```cpp
struct Base0
{
    Base0() { std::cout << "Base0::Base0" << std::endl; };
};

struct Base1
{
    Base1() { std::cout << "Base1::Base1" << std::endl; };
};

template <typename... T>
struct Derived : public T...                                                // base specifier list
{
    Derived() : T()... { std::cout << "Derived::Derived" << std::endl; };    // member-initializer list
};
```

3. in template argument list
```cpp
template <typename ...T>
auto make_tuple(T ...values)
{
    return std::tuple<T...>{values...};
}
```

4. in function/template parameters
```cpp
template <typename ...T>
auto function_results(T (&...f)())
{
    return std::tuple<T...>{f()...};
}
// conceptually like:
// function_results(T1(&f1)(), T2(&f2)(), etc.)
```

5. in using declaration
```cpp
template<typename ...Base>
struct MyStruct :  Base... {
    MyStruct();
    using Base::f...;
    // Conceptually equivalent to:
    //   using Base_1::f;
    //   using Base_2::f;
    //   ...
};
```

6. in lambda capture list
```cpp
void
f(auto...arg)
{
    auto with_copy = [arg...]{
        /* do something with arg... */
    };
    with_copy();
}
```

7. in the alignment specifier
```cpp
template <typename... T>
struct alignas(T...) storage
{
    char contents[std::max({sizeof(T)...})];
};
```

## `sizeof...(pack)`
## `folds`
```cpp
int sum(std::convertible_to<int> auto ...i)
{
    return (0 + ... + i);
}
```
* folds: https://en.cppreference.com/w/cpp/language/fold.html

## capture parameter packs
* parameterpackets can only be captured in three context:
    * template parameter pack
    * function parameter pack
    * init-capture pack

### template parameter pack
### function parameter pack
### init-capture pack


## idioms

### recurse over argument lists
```cpp
void printall(const auto &...args)
{
    // binary left fold
    (std::cout << ... << args);
    std::cout << std::endl;
}
```

### recurse over template parameters
```cpp
template <char... Cs>
struct string_holder
{
    static constexpr std::size_t len = sizeof...(Cs);
    static constexpr char value[] = {Cs..., '\0'};
    constexpr operator const char *() const { return value; }
    constexpr operator std::string() const { return {value, len}; }
};

template <size_t N, char... Digits>
auto index_string()
{
    if constexpr (N < 10)
        return string_holder<N + '0', Digits...>{};
    else
        return index_string<N / 10, (N % 10) + '0', Digits...>();
}

const char *STR_1024 = index_string<1024>();
```

## references
* 
