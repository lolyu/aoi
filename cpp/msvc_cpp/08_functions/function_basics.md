# function basics
* function definition:
    * return type
    * function name
    * parameter list
    * function body
* when the function is called, arguments are passed to the function.
* **to understand the function pointer:**: the `right-left rule`: https://cseweb.ucsd.edu/~ricko/rt_lt.rule.html

## function body
### function local variables
* function local variables is allocated onto the stack.
* function could return either:
    * by value
    * by reference

```cpp
int f()
{
    return 10;
}

int i = f();
```
* in the above code, the literal `10` is first copied to a common place first(**where?**), then copied to the target destination.
    * so if the return type is a class, its copy/move constructor is called twice.

### return value optimization
* without `RVO` or `NRVO`, when function returns, the return value will be copied/moved to a memory that the function caller could reference.
* How does the compiler implement function return class/struct?
   * if a function returns a struct/class, the caller usually allocate a space on the stack, and pass the pointer to the space to the function via a hidden argument.
   * so when the function returns a struct/class, it is copied/moved to the memory space on the caller stack -> one copy/move constructor call will be made.
* when a local variable is returned by value from a function, the compiler always has some optimizations - the returned value is constructed directly in the storage to which the function's return value would otherwise be moved/copied to
    * `NRVO`: named return value optimization
    * `RVO`: return value optimization
```cpp
#include <iostream>
using namespace std;

// g++ rvo.cpp -fno-elide-constructors

class ABC
{
public:
    const char *a;
    ABC()
    {
        cout << "Constructor" << endl;
    }
    ABC(const char *ptr)
    {
        cout << "Constructor" << endl;
    }
    ABC(const ABC &obj)
    {
        cout << "copy constructor" << endl;
    }
    ABC(ABC &&obj)
    {
        cout << "Move constructor" << endl;
    }
    ~ABC()
    {
        cout << "Destructor" << endl;
    }
};

ABC fun123()
{
    ABC obj;
    return obj;
}

ABC xyz123()
{
    return ABC();
}

int main()
{
    // constructor
    ABC abc;
    cout << endl;

    // without RVO: constructor -> copy/move constructor -> destructor
    // with RVO: constructor -> destructor
    fun123();
    cout << endl;

    // without NRVO: constructor -> copy/move constructor -> destructor -> copy/move constructor -> destructor
    // with NRVO: constructor
    ABC obj1(fun123()); // NRVO
    cout << endl;

    // without NRVO: constructor -> copy/move constructor -> destructor -> copy/move constructor -> destructor
    // with NRVO: constructor
    ABC obj2(xyz123()); // RVO, not NRVO
    cout << endl;

    // without NRVO: constructor -> move constructor
    // with NRVO: constructor
    ABC xyz = "Stack Overflow"; // RVO

    return 0;
}
```

### return multiple values from a function
* How to achieve returning multiple values from a function?
    * return a class/struct
    * return a tuple/pair
    * use pass-by-reference parameters

```cpp
#include <iostream>
#include <tuple>
#include <map>

std::tuple<int, int, int> f(int i)
{
    return {i, i*i, i*i*i};
}

std::pair<int, std::string> g()
{
    return {100, "helloworld"};
}

int main()
{
    // structured binding
    auto [x, y, z]  = f(10);
    std::tie(x, y, z) = f(20);
    std::tie(x, std::ignore) = g();

    return 0;
}
```

#### return tuple/pair with structured binding and `std::tie`

## function declaration
* function declaration consists of:
    * return type
    * function name
    * parameter list
    * optional keywords

* optional keywords:
    * `constexpr`:
        * if used with a variable, it means the variable must be initialized before compile-time, and must be initialized with a literal type
        * if used with a function, it means its return value could be computed at compile-time if the consuming code requires it.
        * https://learn.microsoft.com/en-us/cpp/cpp/constexpr-cpp?view=msvc-170
    * `extern`
    * `static`
    * `inline`
    * `noexcept`
    * `volatile`
    * `const`
    * `virtual`
    * `override`
    * `final`
    * `static`

## type deduction in return types


## references
* 
