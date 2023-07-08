# C++ value category
* Each C++ expression has a type, and belongs to a value category, which is the basis the compiler follows to create/copy/move temporary objects during expression evaluation.
    * an expression is characterized by two properties:
        * `type`
        * `value category`

  ![image](https://github.com/lolyu/aoi/assets/35479537/eb938422-720b-451d-9202-5efc2a978356)

* expression value category:
    * `glvalue`: expression whose evaluation determines the identify of an object or a function
    * `rvalue`
    * `lvalue`: has an address that the program can access 
    * `xvalue`: has an address no longer accessible by the program but can be used to initialize an `rvalue` reference
        * `xvalue` stands for expiring value, refers to an object, usually nears its lifetime
    * `prvalue`: no address that is accessible by the program, like literal

## lvalue vs rvalue
* `lvalue`: has an address, could be used with `&` operator
* `rvalue`: has no address, could not be used with `&` operator

```cpp
int a = 100;
int *ap = &a;
```
* `a` is a `lvalue`, it has an address, could 
* `100` is a `rvalue`

### lvalue reference vs rvalue reference
* lvalue reference: reference to a `lvalue`
* rvalue reference: reference to a `rvalue`

```cpp
int a = 5;
int &ref_a = a;   // lvalue reference
int &&ref_right = 5;   // rvalue reference
```
