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
* value initialization: happens when the object is constructed with an empty initializer.
* the value initialization does the following
    * for classes with at least one public constructor, the default constructor is called
    * for non-union classes with no declared constructor, the object is zero-initialized, and the default constructor is called
    * for arrays, element is value-initialized
    * for other cases, the variable is zero-initialized

```
T t = t();
T *t = new T();
T t{};
```


## references
* https://en.cppreference.com/w/cpp/language/value_initialization
* https://en.cppreference.com/w/cpp/language/default_initialization
