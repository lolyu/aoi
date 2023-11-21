# function overloading
* C++ allows to specify more than one function of the same name in the same scope.
* At compile-time, the compiler chooses which overload to use based on the types and number of arguments passed in by the caller.
    * **`function signature`: the parameter list of the function**
    * the compiler uses the `function signature` to distinguish among overloads

## overloading considerations
|function declaration element|used for overloading?|
|-|-|
|return type|no|
|number of arguments|yes|
|type of arguments|yes|
|presence or absence of ellipsis|yes|
|`const` or `volatile`|yes, when applied to entire function|
|reference qualifiers(`&` or `&&`)|yes|

* **NOTE**:
    * functions takes a parameter of a given type and a reference to that type are considered to have the same function signature

```cpp
void f(int i);
void f(int &i); // not allowed
```

### function default argument and overload
* the function default argument is not part of a function's type.
    * the default argument allows you to call a function without specifying all of the arguments
    * it does not allow you to create a pointer to the function that does not specify the types of all the arguments
* the type/signature of a function that has default parameters is the same as the type/signature of the function if the parameters were there without any default values
* the default value is replaced in compile time, so the default value must be specified in the function declaration.
    * so in the following example, `foo()` is replaced with `foo(0)` in the compile time.
```cpp
int foo(int a = 0);

foo();
```

```cpp
void f(int i)
{
    cout << i << endl;
}

void f(int i, int j = 100)
{
    cout << i << endl;
    cout << j << endl;
}

// ambiguous function call
// for 100, there are two candidates, f(int i) and f(int i, int j = 100)
f(100);
```

## overloaded function resolution
* the function matching is invoked based on the `best match` among the function declarations in the current scope to the arguments supplied, and **it is done in compile-time**.
* the best match means either:
    * an exact match
    * a trivial conversion was performed
    * an integral promotion was performed
    * a standard conversion to the desired parameter type exists
    * a user-defined conversion(conversion operator or a constructor) to the desired parameter type exists
    * arguments represented by an ellipsis was found
* how to find the match?
    * the compiler creates a set of candidate functions for each argument
    * and the best match is selected in the intersection of all the sets.
        * if the intersection contains more than one function, an ambiguous error is generaed


## references
* https://en.cppreference.com/w/cpp/language/overload_resolution
