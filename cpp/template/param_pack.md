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

## references
* 
