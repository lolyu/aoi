# initialization
* variable initialization happens in three scenarios:
    * variable definition
    * function argument: pass by value
    * function return: return by value


## zero initialization
* zero initialization: initialize the memory area of the variable to zero bits:
    * numeric variables are initialized to `0`
    * char variables are initialized to `'\0'`
    * pointers are initialized to `nullptr`
    * array, POD types, structs, and unions have their members initialized to zero value

* zero initialization happens when:
    * at program start up, for all static duration variables.
    * during value initialization, for scalar types and POD types that are initialized by empty braces: `{}`
    * for arrays that have only a subset of members initialized


##
