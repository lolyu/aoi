# copy semantics
* the customizing of the copy semantics in C++ is achieved through the use of copy constructors and copy assignment operators
* if you don't declare a copy constructor/assignment, the compiler will generate a member-wise copy copy constructor/assignment.
    * for the compiler-generated copy constructor, it has a single argument of type `reference to class-name`, an exception is when all base classes and member classes have copy constructors declared as taking `const class-name&`.
    * for the compiler-generated copy assignment, it has a single argument of type `reference to class-name`, an exception is when all base classes and member classes have copy assignment declared as taking `const class-name&`.
* **NOTE**: If you implement either one, recommend to implement both

## copy constructor vs copy assignment
* Will operator `=` invokes copy constructor or copy assignment?
    * **NOTE**: copy constructor is used to initialize a new instance from an old instance and the copy assignment is used to change an existing instance to have the same values as the rvalue.
    * the key is the left operand:
        * if the left operand is a new instance, `=` invokes copy constructor
        * if the left operand is an existing instance, `=` invokes copy assignment

## references
* 
