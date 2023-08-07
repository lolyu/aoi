# alias
* two ways to create alias:
    * `typedef`
    * `using` statement

```cpp
typedef void (*func)(int);
using func = void (*)(int);
```

* **NOTE**: `typedef` doesn't support template
```cpp
template<typename T> using ptr = T*;

// the name 'ptr<T>' is now an alias for pointer to T
ptr<int> ptr_int;
```

## typedef in struct declaration
```cpp
typedef struct {
   unsigned x;
   unsigned y;
} POINT;
```
* in C++, the struct `POINT` is treated as a class name, only with the following restrictions:
    * the name cannot appear after a `class`, `struct`, or `union` prefix.
    * the name cannot be used as a constructor or destructor name within a class declaration

## references
* https://stackoverflow.com/questions/612328/difference-between-struct-and-typedef-struct-in-c
