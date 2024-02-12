# class constructor
* if a class member has no default constructor, the class member must be explicitly initialized in the member initializer list.

## member initializer list
* a constructor can optionally have member initializer list, which initialize the object listed before the constructor body runs:
    * class non-static member
    * base class
    * class itself (delegating constructor)
* **NOTE**: the identifer order in the member initializer list doesn't control the initialization order.

### member initialization order
1) If the constructor is for the most-derived class, virtual bases are initialized in the order in which they appear in depth-first left-to-right traversal of the base class declarations (left-to-right refers to the appearance in base-specifier lists).
2) Then, direct bases are initialized in left-to-right order as they appear in this class's base-specifier list.
3) Then, non-static data member are initialized in order of declaration in the class definition.
4) Finally, the body of the constructor is executed.

## explicit constructor
* if a class has a constructor that takes a single parameter, or if all parameters except one have a default value, the parameter type can be implicitly converted to the class type.
```cpp
Box(int size);

Box b = 42;
```
* to disable this implicit conversion, use `explicit` keyword:
```cpp
explicit Box(int size);

Box b = 42;   // not allowed
```


## references
* https://en.cppreference.com/w/cpp/language/constructor
