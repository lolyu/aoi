# C++ header file
* the header file is a common place to have declarations, so each `.cpp` file could have one and only declaration.
* Because a header file might potentially be included by multiple files, it cannot contain definitions that might produce multiple definitions of the same name. The following are not allowed, or are considered very bad practice:
    * built-in type definitions at namespace or global scope
    * non-inline function definitions
    * non-const variable definitions
    * aggregate definitions
    * unnamed namespaces
    * using directives

## references
