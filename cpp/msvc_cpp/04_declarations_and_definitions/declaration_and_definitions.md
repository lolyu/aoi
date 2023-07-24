# declarations and definitions
* A `declaration` introduces an identifier and describes its type.
    * `declaration` doesn't allocate storage for it.
    * `declaration` is what the compiler needs to accept references to the identifier
    * **`declaration` tells the compiler to accept a name that is legal**

* A `definition` actually instantiates/implements the identifier
    * a `declaration` of built-in types is a definition because the compiler knows how much space to allocate for it.
    * **`definition` tells the linker to link a name reference to the content of the name**
    * `static` class variable/function is declared inside the class, but should be defined/initialized out of the class.
```cpp
// declare and define int variables i and j.
int i;
int j = 10;

// enum declaration, also a enum definition
enum suits { Spades = 1, Clubs, Hearts, Diamonds };

// class declaration, also a class definition
class CheckBox : public Control
{
public:
    Boolean IsChecked();
    virtual int     ChangeState() = 0;
};
```

## alias
* `typedef` and `using` statements could give alias to another name(should be defined)

## storage classes
* storage specifiers control two independent properties:
    * **`storage duration`**: the lifetime of the storage allocated for the object
    * **`linkage`**: **if the object could be introduced to another scope by declaration**
* six storage specifiers:
    * `auto`: **deprecated**
    * `register`: **deprecated**
    * `static`: only allow for object declarations and function declarations.
        * When used in a declaration of a class member, it declares a static member
        * When used in a declaration of an object, it specifies static storage duration (except if accompanied by thread_local)
        * When used in a declaration at namespace scope, it specifies internal linkage.
    * `extern`: only allowed in the declarations of variables and functions
    * `thread_local`:
    * `mutable`:
* summary:
    * storage specifiers controls:
        * storage duration
            * `auto`: storage duration is the enclosing block duration
                * local variables
            * `static`: storage duration is the program duration
                * all variables declared at namespace scope
            * `thread`: storage duration is the thread duration
                * all variables declared with thread_local
            * `dynamic`: storage duration is the dynamic allocated memory duration
        * linkage: whether the variable could be introduced to another scope by declaration
            * no linkage: visible only in the local scope
            * internal linkage: visible only in the scopes of the current translation unit
            * external linkage: visible to other translation unit
* NOTE:
    * all variables declared at namespace scope are static storage duration, but their linkage differs:
        1. const or static variables have internal linkage
        2. others have external linkage

### storage durations
* four storage durations: determines the lifetime of the storage allocated for the object
    * `automatic`: the storage for this object starts at the beginning of the enclosing code block and is deallocated at the end.
    * `static`: the storage for this object starts at the program start and is deallocated at the program end.
        * all objects declared at namespace scope(including global namespace) have this storage duration, plus those declared with `static` or `extern`
    * `thread`: the storage for the object is allocated when the thread begins and deallocated when the thread ends. Each thread has its own instance of the object.
        * use `thread_local` to explicitly declare `thread` storage duration variables
    * `dynamic`: objects allocated dynamically

### linkage
* three linkages:
    * no linkage: the name could be referred to only from the scope it is in
        * local variables
    * internal linkage: the name can be referred to from all scopes in the current translation unit
        * any of the following names declared at namespace scope have internal linkage:
            * static variables, functions, or function templates
            * const-qualified variable
        * any of the names declared inside an unnamed namespace
    * external linkage: the name can be referred to from the scopes in the other translation units
        * any of the names declared at the namespace scope have external linkage, except those internal linkage ones.

### storage specifiers
#### static
* use `static` in the namespace scope variable/function declaration: the variable/function has internal linkage
    * the static variable is zero-initialized
* use `static` in variable declaration inside a function: static local variable
    * initialized the first time the control passes through the declarations.
    * the destructor for a static local variable is called at program exit.
    * **static local variable initialization is guaranteed to be thread-safe**
* use `static` in class
    * static class member
        * member shared by all instances
        * a static class member must be defined at the file scope
        * an integral data member as `const static` can have an in-class initializer
    * static class function
        * function shared by all instances

#### extern
#### thread_local
* You can specify `thread_local` only on data items with static storage duration, which includes:
    * global data objects (both static and extern)
    * local static objects
    * static data members of classes
* Any local variable declared thread_local is implicitly static if no other storage class is provided; in other words, at block scope `thread_local` is equivalent to `thread_local static`.
* **`thread_local` must be present on both declaration and definition**

### static member


## other specifiers
### auto
* `auto` commands the compiler to use the initialization expression of a declared variable to deduce its type.
* **NOTE**: `auto` drops reference, `const` qualifiers, and `volatile` qualifiers.
```cpp
    const int i = 0;
    const int &ir = i;
    auto j = ir; // j is simply an int

    j = 20;
```

#### auto in range for
```cpp
    std::vector<int> vi{0, 1, 2, 3, 4};
    for (auto i : vi)
    {
        ;
    }

    for (auto &i : vi)
    {
        ;
    }

    for (const auto &i : vi)
    {
        ;
    }
```

### const

#### const pointer vs pointer to const
* const pointer: pointer itself is a const
    * `int *const i`
* pointer to const: pointer that points to a const
    * `const int *i`

#### const member functions
* const member function is those member functions that don't modify the object
    * const member function cannot modify any non-static data member
    * call any member functions that aren't constant
* `const` keyword is required in both the declaration and the definition

#### difference between C/C++
* in `C`, the constant values default to external linkage
* in `C++`, the constant values default to internal linkage

### constexpr
* `const` keyword declares an object as constant, which implies that once initialized, the value of that object won't change.
* `constexpr` keyword instructs the compiler that the object is qualified to be used in a `constant expression`.
    * making variables `constexpr` ensures that those variables have values known at compile-time, and thus are eligible for constant folding when they are used in expressions(even in non-const expressions).
* best practices:
    * Any variable that should not be modifiable after initialization and whose initializer is known at compile-time should be declared as `constexpr`.
    * Any variable that should not be modifiable after initialization and whose initializer is not known at compile-time should be declared as `const`.

#### constant expression
* a `constant expression` is an expression that can be evaluated by the compiler at compile-time.
    * all the values in the `constant expression` must be known at compile-time(compile-time constant)
    * `constant expression` can be used in places that require compile-time evaluation: template parameters and array-size specifiers
    * when the compiler encounters a `constant expression`, it may evaluate the expression at compile-time, and then replace the constant expression with the result of the evaluation.
* an object like `N` can be used in `constant expression` when:
    * declared with `constexpr`
    * compile-time const
    * integral of enumeration type

* compile-time constant: constant whose value is known at compile-time.
    * literals are compile-time constant
* a const variable can be either:
    * compile-time const
        * a const variable is a compile-time const if its initializer is a constant expression
    * runtime const
        * a const variable is a compile-time const if its initializer is a non-constant expression

```cpp
int f()
{
    return random();
}

template <int N>
struct fixed_size_list
{
    fixed_size_list() = default;
    ~fixed_size_list() = default;
};

enum days
{
    zero,
    one,
    two,
};

int main()
{
    // i is a compile-time const
    const int i = 0;
    // j is a compile-time const
    const int j = 10;
    // sum is a compile-time const
    const int sum = i + j;
    // r is a runtime const
    const int r = f();

    // legal
    constexpr int sum0 = i + j;
    // illegal, f() could be evaluated at compile-time
    // constexpr int r0 = f();

    int ii = 10;
    int nums0[r];
    int nums1[ii];
    int nums2[sum0];

    // illegal, r could not be used in a constant expression
    // auto fsl0 = fixed_size_list<r>();
    // illegal, runtime const could not be used in constant expression
    // auto fsl1 = fixed_size_list<ii>();
    // legal, sum0 is a constexpr, could be used in constant expression
    auto fsl2 = fixed_size_list<sum0>();
    // legal, enum type value could be used in constant expression
    auto fsl3 = fixed_size_list<days::two>();

    return 0;
}
```

#### constexpr function
* a `constexpr` function is a function whose return value could be evaluated at compile-time if the consuming code requires:
    * used to initialize a `constexpr` variable
    * used as a non-type template argument
* a `constexpr` function must accept and return only literals.
* when its arguments are `constexpr` values, the `constexpr` function produces a compile-time constant
* when its arguments are non-`constexpr` values, the `constexpr` function produce a runtime return
* when the consuming code doesn't require, the `constexpr` function produce a runtime return

```cpp
template <typename T, int N>
constexpr int length(const T (&nums)[N])
{
    return N;
}
```

### extern
* `extern` could be applied to a global variable/function, or template declaration
    * `extern` applied to a non-`const` global variable declaration: the global variable is defined in another translation unit
        * the `extern` must be added to every declaration except the definition
    * `extern` applied to a `const` global variable declaration: the global `const` variable has external linkage
        * the `extern` keyword must be applied to all declarations in all translation unit
    * `extern` applied to a template declaration: the template has already been instantiated somewhere else, instructs the compiler to reuse this instantiation.



## references
* https://stackoverflow.com/questions/1410563/what-is-the-difference-between-a-definition-and-a-declaration
* https://stackoverflow.com/questions/14116003/whats-the-difference-between-constexpr-and-const
* https://www.learncpp.com/cpp-tutorial/compile-time-constants-constant-expressions-and-constexpr/
