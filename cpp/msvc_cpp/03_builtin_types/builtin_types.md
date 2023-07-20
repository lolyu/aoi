# built-in types
* three main categories:
    * integral
    * float
    * void

## nullptr
* `nullptr` is a constant of type `nullptr_t`
* `nullptr` could be converted to any raw pointer type
* use `nullptr` instead of `NULL`
 
## void
* `void` is used to describe an empty set of values
* `void` is only valid in three scenarios:
    * `void` as function return type
    * `void` as function parameter list
    * void pointer


### void pointer
* `void` pointer can point to any variable(not `const or `volatile`)
* `void` pointer cannot be dereferenced
* `void` pointer can point to a free function(a function that's not a member of a class) - this need explicit cast

```cpp
    std::pair<const char *, double> p0 = std::make_pair<const char *, double>(nullptr, 20);
    void *ptr = &p0;
```

## bool
* `bool`: either `true` or `false`
    * in integral promotion
        * `true` -> 1
        * `false`-> 0


## how to get numeric limits?
```cpp
    std::cout << std::numeric_limits<float>::max() << std::endl;
    std::cout << std::numeric_limits<float>::min() << std::endl;
    std::cout << std::numeric_limits<int>::min() << std::endl;
    std::cout << std::numeric_limits<int>::max() << std::endl;
```
