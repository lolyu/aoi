# C++ value category
* Each C++ expression has a type, and belongs to a value category, which is the basis the compiler follows to create/copy/move temporary objects during expression evaluation.
    * an expression is characterized by two properties:
        * `type`
        * `value category`

  ![image](https://github.com/lolyu/aoi/assets/35479537/eb938422-720b-451d-9202-5efc2a978356)

* **NOTE**: value categories are a taxonomy of **expressions** instead of classifications of objects or variables or types.
* expression value category:
    * `glvalue`: expression whose evaluation determines the identity of an object or a function
        * **`glvalue`s are expressions that evaluates to locations**
    * `rvalue`
    * `lvalue`: has an address that the program can access 
    * `xvalue`: has an address no longer accessible by the program but can be used to initialize an `rvalue` reference
        * `xvalue` stands for expiring value, refers to an object, usually nears its lifetime
        * **`xvalue`s can be safely moved from**
    * `prvalue`: no address that is accessible by the program, like literal
        * **`prvalue`s are used for initialization, don't denote objects**

![image](https://github.com/lolyu/aoi/assets/35479537/79190089-669d-4862-8738-d2a8fac5cff9)
![image](https://github.com/lolyu/aoi/assets/35479537/adc455d9-c7d2-4c2f-af08-58b8fb6dfa51)


## lvalue vs rvalue
* `lvalue`: has an address, could be used with `&` operator
* `rvalue`: has no address, could not be used with `&` operator

```cpp
int a = 100;
int *ap = &a;
```
* `a` is a `lvalue`, it has an address 
* `100` is a `rvalue`

### lvalue reference vs rvalue reference
* lvalue reference: reference to a `lvalue`
    * lvalue references can be used to alias an existing object
* rvalue reference: reference to a `rvalue`
    * rvalue references can be used to extend the lifetime of temporary objects
    * when rvalue reference is used in an expression, it is a lvalue

```cpp
int a = 5;
int &ref_a = a;   // lvalue reference
int &&ref_right = 5;   // rvalue reference
```


## references
* https://medium.com/@barryrevzin/value-categories-in-c-17-f56ae54bccbe
