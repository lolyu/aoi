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
|`const` or `volatile`|yes, when applied to reference|
|reference qualifiers(`&` or `&&`)|yes|

* **NOTE**: only functions that take different argument initializers could be differentiated by the compiler

```cpp

void f(int i) {}            // [1]
void f(int &i) {}           // [2]
void f(const int i) {}      // [3] not allowed, take the same initializer as [1]
void f(int *p) {}           // [4]
void f(const int *p) {}     // [5]

int main()
{
    f(100);                 // use [1]
    int i = 100;
    f(i);                   // ambiguous call, both [1] and [2] works
    f(&i);
    const int ci = 100;
    f(&ci);
    return 0;
}
```
* for `f(int i)` and `f(int &i)`:
    * if called with a variable, like `f(i)`, it is ambiguous, because those two both can take `i` as argument initializer
    * if called with a literal, like `f(100)`, it is not, because only `f(int i)` could take literal `100` as argument initializer
* `const` and `volatible` could only be used with either reference or pointer to overload function(**not on the base type**)

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
        * candidate functions are functions in which the actual argument in that position can be converted to the type of the parameter.
    * and the best match is selected in the intersection of all the sets.
        * if the intersection contains more than one function, an ambiguous error is generated

### argument conversions
* when the compiler tries to match arguments against the parameters in function declarations, conversion happens if no exact match can be found.
* the compiler will choose the best implicit conversion sequence among all possible implicit conversion sequences.
* rank of implicit conversion sequences:
    * exact match
        * no conversion or trivial conversion
    * promotion
    * standard conversion
    * user-defined conversion
    * ellipsis


## references
* https://en.cppreference.com/w/cpp/language/overload_resolution
