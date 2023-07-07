# cpp scope
* what is a scope?
    * the context in which a name is visible
    * within a scope, unqualified name lookup can be used to associate the name with its declaration

## scopes
* scopes:
    * block scope
    * function parameter scope
    * namespace scope
    * class scope
    * enum scope
    * template parameter scope

### block scope
* The potential scope of a name declared in a block (compound statement) begins at the point of declaration and ends at the end of the block.
    * The actual scope is the same as the potential scope unless an identical name is declared in a nested block
* special cases:
    * name declared in an exception handler
        * the scope begins at the point of declaration and ends at the end of the exception handler
        * not in scope in another exception handler or in the enclosing block
    * name declared in the `for`, `switch` or `if` statement
        * the scope begins at the point of the declaration and ends at the end of the controlled statement(`for`, `switch` or `if` statement)

### function parameter scope
* the potential scope of a name declared in a function parameter or a function-local predefined variable begins at the point of declaration
    * ends at the end of the function body if it is a function definition
    * ends at the function declarator

### namespace scope
* The potential scope of a name declared in a namespace begins at the point of declaration and includes the rest of the namespace and all namespace definitions with an identical namespace name that follow, plus, for any using-directive that introduced this name or its entire namespace into another scope, the rest of that scope.
* the top-level scope of a translation unit is also a namespace, called `global namespace scope`
* special cases:
    * unnamed namespace
    * inline namespace
### class scope
### enum scope
* unscoped enumeration
* scoped enumeration

### template parameter scope

## references
* https://en.cppreference.com/w/cpp/language/scope
