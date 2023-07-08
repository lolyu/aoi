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
