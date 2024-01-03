# initialization
* variable initialization happens in three scenarios:
    * variable definition
    * function argument: pass by value
    * function return: return by value


## zero initialization
* zero initialization: initialize the memory area of the variable to zero bits:
    * numeric variables are initialized to `0`
    * char variables are initialized to `'\0'`
    * pointers are initialized to `nullptr`
    * array, POD types, structs, and unions have their members initialized to zero value

* zero initialization happens when:
    * at program start up, for all static duration variables.
    * during value initialization, for scalar types and POD types that are initialized by empty braces: `{}`
    * for arrays that have only a subset of members initialized


## default initialization
* default initialization: initialized with default constructor
    * default initialization happens when called with no initialization expression or with `new` keyword

```cpp
T t1;
T t2 = new T;
```

* scalar variables are default initialized with indeterminate values
* when the array variables are default initialized, its members are default initialized.

## value initialization
* value initialization: happens when the object is constructed with an empty initializer(either `{}` or `()`).

```cpp
T();
new T();
T object{};
T {};
new T{};
Class::Class(...) : member () { ... };
Class::Class(...) : member {} { ... };
```


* the value initialization does the following
    * for classes with at least one public constructor, the default constructor is called
    * for non-union classes with no declared constructor, the object is zero-initialized, and the default constructor is called
    * for arrays, the element is value-initialized
    * for other cases, the variable is zero-initialized

```
T t = t();
T *t = new T();
T t{};
```

## copy initialization
* copy initialization: initialization of one object uses a different object:
    * a variable initialized with an equal sign
    * an argument passed to a function
    * an object returned from a function
    * an exception thrown or caught
    * a non-static data member is initialized using an equal sign

* **NOTE**: copy initialization could not invoke explicit constructors
```cpp
shared_ptr<int> sp = new int(100); // faulty, as the constructor is explicit
```

## direct initialization
* direct initialization is initialization using (non-empty) braces or parentheses. Unlike copy initialization, it can invoke explicit constructors. It occurs in the following cases:
    * a variable is initialized with non-empty braces or parentheses
    * a variable is initialized with the new keyword plus non-empty braces or parentheses
    * a variable is initialized with static_cast
    * in a constructor, base classes and non-static members are initialized with an initializer list
    * **in the copy of a captured variable inside a lambda expression**

```cpp
T object ( arg );
T object ( arg1, arg2, ... );
T object { arg };
T ( other )
T ( arg1, arg2, ... )
static_cast< T >( other )
new T( args, ... )
Class::Class() : member( args, ... ) { ... }
[arg]() { ... }
```


* **NOTE**: direct initialization invokes constructors
```cpp
class MyObject
{
public:
    MyObject(double d = 0.0f) : d(d)
    {
        std::cout << "Initialize MyObject with " << d << std::endl;
    }

    MyObject(const MyObject &mo)
    {
        std::cout << "Copy initialize MyObject with " << mo.d << std::endl;

        d = mo.d;
    }

private:
    double d;
};

int main()
{
    int i{100};
    MyObject mo = static_cast<double>(i); // direct initialization

    return 0;
}
```

## list initialization
* two types:
    * direct list initialization
    * copy list initialization(since C++20)
* direct list initialization
```cpp
T object { arg1, arg2, ... };
T object{.des1 = arg1 , .des2 { arg2 } ... };
T { arg1, arg2, ... }
T {.des1 = arg1 , .des2 { arg2 } ... }
new T { arg1, arg2, ... }
new T {.des1 = arg1 , .des2 { arg2 } ... }
```
* copy list initialization(since 
```cpp
T object = { arg1, arg2, ... };
T object = {.des1 = arg1 , .des2 { arg2 } ... };
function ({ arg1, arg2, ... })
function ({.des1 = arg1 , .des2 { arg2 } ... })
return { arg1, arg2, ... };
return {.des1 = arg1 , .des2 { arg2 } ... };
object [{ arg1, arg2, ... }]
object [{.des1 = arg1 , .des2 { arg2 } ... }]
object = { arg1, arg2, ... }
object = {.des1 = arg1 , .des2 { arg2 } ... }
```

## aggregate initialization
* aggregate initialization is a form of list initialization for arrays or class types (often structs or unions) that have:
    * no private or protected members
    * no user-provided constructors, except for explicitly defaulted or deleted constructors
    * no base classes
    * no virtual member functions

## union and struct initialization
* if a union doesn't have a default constructor, you can initialize it with a single value.
    * the value is used to initialize the first non-static field
* for struct, the first value in the initializer list is used to initialize the first field.

## references
* https://en.cppreference.com/w/cpp/language/value_initialization
* https://en.cppreference.com/w/cpp/language/default_initialization
* https://en.cppreference.com/w/cpp/language/direct_initialization
* https://en.cppreference.com/w/cpp/language/list_initialization
* https://en.cppreference.com/w/cpp/language/aggregate_initialization
