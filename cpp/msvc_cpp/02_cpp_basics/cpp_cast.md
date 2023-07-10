# cast

## implicit cast
* When an expression contains operands of different built-in types, and no explicit casts are present, the compiler uses built-in standard conversions to convert one of the operands so that the types match:
    * promotion: no warning
        * any `signed` or `unsigned` integral type -> `long long` to `double`
        * `bool` or `char` -> any other built-in type
        * `short` -> `int`, `long`, `long long`
        * `int`, `long` -> `long long`
        * `float` -> `double`
    * coersion: warn about potential data loss
    * signed <--> unsigned conversions
    * pointer conversions: a C-style array is implicitly converted to a pointer to the first element in the array
 

## explicit cast
* C-style cast
```c++
(int) x; // old-style cast, old-style syntax
int(x); // old-style cast, functional syntax
```
* C++ style cast:
    * `static_cast`: for casts that are checked at compile time only
        * `static_cast` can also be used to cast between pointer-to-base and pointer-to-derived
            * compiler could not tell whether such conversions will be safe
    * `dynamic_cast`: safe, runtime-checked cast of pointer-to-base to pointer-to-derived
        * with overhead of the runtime-check
    * `const_cast`: cast away `const`-ness of cast `const`-ness in
    * `reinterpret_cast`: casts between unrelated types
        * cast a pointer to `POD` struct to a pointer to the struct first element
## references
