# cpp type system

* terminology:
    * scalar type: types that hold single value of a defined range
        * arithmetic types
            * int
            * float
        * enum
        * pointer
        * `std::nullptr_t`
    * compound type
    * variable
    * object
    * POD type

* built-in types
    * `int`: integral literal default type
    * `double`: float literal default type
    * `bool`
    * `char`
    * `wchar_t`
    * `unsigned char`: used to represent a byte
    * `unsigned int`: default choice for bit flags
    * `long long`
 

* void type
    * `void *`: pointer to void is used to reference raw memory

* const
* string types
* user-defined types
    * `class`
    * `struct`
    * `union`
    * `enum`
* pointer types
    * it is not encouraged to use raw pointers for object ownership in C++
        * use smart pointers(`RAII`: resource acquisition is initialization)
    * the pointer declaration doesn't allocate the memory needed to store the data value(backing store)
