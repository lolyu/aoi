# object layout
* C++ introduces three categories of simple classes and structs:
    * aggregate
    * trivial
    * standard-layout
    * POD(plain old data)

## aggregate
* An `aggregate` is an array or a class with:
    * no user-provided constructor
    * no brace-or-equal-initializers for non-static data members
    * no private or protected non-static data members
    * no virtual functions

## trivial
* continuous in memory layout, could be coped via `memcpy`
* **a trivial type is a class or struct for which the compiler provides all the special members, either implicitly or explicitly defaulted by us.**
* a trivial class/struct members can have different access specifiers, which leads to the fact that a trivial type is not usable in `C` code
* support static initialization
    * what is static initialization?
        * `static initialization` means initialization from literals that can be stored in the executable itself and loaded to initialize.
        * `dynamic initialization` means initialization that involves calling functions(constructors) and must take place at runtime.
    * so in order to support static initialization, no user-provided special member functions 

## standard-layout
* A standard-layout class has the same memory layout of the equivalent `C` struct or union.
* when using inheritance, only one class in the whole inheritance tree can have non-static data members.


## POD
* a POD has two distinct properties:
    * static initialization
    * the POD has the same layout as struct compiling in `C`
* a class that is both trivial and standard-layout is a POD

## references
* https://en.cppreference.com/w/cpp/language/classes#Trivial_class
* https://andreasfertig.blog/2021/01/cpp20-aggregate-pod-trivial-type-standard-layout-class-what-is-what/
* https://stackoverflow.com/questions/4178175/what-are-aggregates-and-pods-and-how-why-are-they-special/
* https://stackoverflow.com/questions/15094514/what-exactly-does-static-initialization-mean
