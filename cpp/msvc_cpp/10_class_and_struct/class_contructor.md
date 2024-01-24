# class constructor

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




## references
* https://en.cppreference.com/w/cpp/language/constructor
