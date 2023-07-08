# C++ main function

 * `main()` is required for all C++ programs, it is where the source code starts execution.
    * before a program enters the `main` function, all static class members without explicit initializer are set to zero
  

## `main()` signature
```cpp
int main();
int main(int argc, char *argv[]);
```
* if no return value is specified in `main()`, the compiler returns `0`
* arguments:
    * `argc`: an integer that contains the count of arguments that follow in `argv`
    * `argv`: an array of null-terminated strings of command-line arguments
        * `argv[0]` is the program name
        * `argv[1]` ~ `argv[argc - a]`: the arguments from command-line
        * `argv[argc]`: `NULL`


## program termination
* three methods to exit a program:
    * `return`
        * destructors will be called for `main()` locally scoped objects
        * destructors will be called for globally scoped objects
        * destructors will be called for static objects
    * `exit`
        * destructors will not be called for `main()` locally scoped objects
        * destructors will be called for globally scoped objects
        * destructors will be called for static objects
    * `abort`: terminates the program immediately
        * no destructors will be called
        * `atexit` functions will be bypassed

## example
```cpp
#include "stdlib.h"
#include <iostream>

class A
{
public:
    A(int i) : i(i)
    {
        std::cout << "A construct " << i << std::endl;
    }

    ~A()
    {
        std::cout << "A destruct " << i << std::endl;
    }

private:
    int i;
};

A a0{10};
static A a1{11};

int main(int, char *[])
{
    A a{12};

    // return 0;
    // exit(EXIT_SUCCESS);
    abort();
}
```

### return output
```
A construct 10
A construct 11
A construct 12
A destruct 12
A destruct 11
A destruct 10
```
### exit output
```
A construct 10
A construct 11
A construct 12
A destruct 11
A destruct 10
```
### abort output
```
A construct 10
A construct 11
A construct 12
[1]    2885643 abort (core dumped)  ./a.out
```
