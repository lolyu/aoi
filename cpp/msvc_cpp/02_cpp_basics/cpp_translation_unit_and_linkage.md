# translation unit and linkage

## translation unit

* translation unit: an implementation file and all the headers that it includes directly or indirectly
* one definition rule(`ODR`): a variable/function name can be declared multiple times within its scope, but it can only be defined once in a C++ program.
    * so no variable/function definition inside the header to avoid breaking `ODR` if it is included multiple times

* the best way to make a variable visible across multiple files is to declare it in a header file and define the name in one implementation file.

## linkage
* global names might have two different linkages:
    * external linkage: visible from any translation unit in the program
    * internal linkage: visible only within the translation unit
        * internal linkage by default:
            * `const`
            * `constexpr`
            * `static`
            * `typedef`
