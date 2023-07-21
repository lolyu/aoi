# declarations and definitions
* A `declaration` introduces an identifier and describes its type.
    * `declaration` doesn't allocate storage for it.
    * `declaration` is what the compiler needs to accept references to the identifier
    * **`declaration` tells the compiler to accept a name that is legal**

* A `definition` actually instantiates/implements the identifier
    * a `declaration` of built-in types is a definition because the compiler knows how much space to allocate for it.
    * **`definition` tells the linker to link a name reference to the content of the name**
    * `static` class variable/function is declared inside the class, but should be defined/initialized out of the class.
```cpp
// declare and define int variables i and j.
int i;
int j = 10;

// enum declaration, also a enum definition
enum suits { Spades = 1, Clubs, Hearts, Diamonds };

// class declaration, also a class definition
class CheckBox : public Control
{
public:
    Boolean IsChecked();
    virtual int     ChangeState() = 0;
};
```

## alias
* `typedef` and `using` statements could give alias to another name(should be defined)

## storage classes
* storage specifiers control two independent properties:
    * **`storage duration`**: the lifetime of the storage allocated for the object
    * **`linkage`**: **if the object could be introduced to another scope by declaration**
* six storage specifiers:
    * `auto`: **deprecated**
    * `register`: **deprecated**
    * `static`: only allow for object declarations and function declarations.
        * When used in a declaration of a class member, it declares a static member
        * When used in a declaration of an object, it specifies static storage duration (except if accompanied by thread_local)
        * When used in a declaration at namespace scope, it specifies internal linkage.
    * `extern`: only allowed in the declarations of variables and functions
    * `thread_local`:
    * `mutable`:
* summary:
    * storage specifiers controls:
        * storage duration
            * `auto`: storage duration is the enclosing block duration
                * local variables
            * `static`: storage duration is the program duration
                * all variables declared at namespace scope
            * `thread`: storage duration is the thread duration
                * all variables declared with thread_local
            * `dynamic`: storage duration is the dynamic allocated memory duration
        * linkage: whether the variable could be introduced to another scope by declaration
            * no linkage: visible only in the local scope
            * internal linkage: visible only in the scopes of the current translation unit
            * external linkage: visible to other translation unit
* NOTE:
    * all variables declared at namespace scope are static storage duration, but their linkage differs:
        1. const or static variables have internal linkage
        2. others have external linkage

### storage durations
* four storage durations: determines the lifetime of the storage allocated for the object
    * `automatic`: the storage for this object starts at the beginning of the enclosing code block and is deallocated at the end.
    * `static`: the storage for this object starts at the program start and is deallocated at the program end.
        * all objects declared at namespace scope(including global namespace) have this storage duration, plus those declared with `static` or `extern`
    * `thread`: the storage for the object is allocated when the thread begins and deallocated when the thread ends. Each thread has its own instance of the object.
        * use `thread_local` to explicitly declare `thread` storage duration variables
    * `dynamic`: objects allocated dynamically

### linkage
* three linkages:
    * no linkage: the name could be referred to only from the scope it is in
        * local variables
    * internal linkage: the name can be referred to from all scopes in the current translation unit
        * any of the following names declared at namespace scope have internal linkage:
            * static variables, functions, or function templates
            * const-qualified variable
        * any of the names declared inside an unnamed namespace
    * external linkage: the name can be referred to from the scopes in the other translation units
        * any of the names declared at the namespace scope have external linkage, except those internal linkage ones.

### storage specifiers
#### static
* use `static` in the namespace scope variable/function declaration: the variable/function has internal linkage
    * the static variable is zero-initialized
* use `static` in variable declaration inside a function: static local variable
    * initialized the first time the control passes through the declarations.
    * the destructor for a static local variable is called at program exit.
    * **static local variable initialization is guaranteed to be thread-safe**
* use `static` in class
    * static class member
        * member shared by all instances
        * a static class member must be defined at the file scope
        * an integral data member as `const static` can have an in-class initializer
    * static class function
        * function shared by all instances

#### extern
#### thread_local
* You can specify `thread_local` only on data items with static storage duration, which includes:
    * global data objects (both static and extern)
    * local static objects
    * static data members of classes
* Any local variable declared thread_local is implicitly static if no other storage class is provided; in other words, at block scope `thread_local` is equivalent to `thread_local static`.
* **`thread_local` must be present on both declaration and definition**

### static member


## references
* https://stackoverflow.com/questions/1410563/what-is-the-difference-between-a-definition-and-a-declaration
