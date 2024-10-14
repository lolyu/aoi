# pointer to implementation idiom
* pimpl idiom is used to hide the implementation away from the class interface:
    * pimpl idiom could reduce the compilation dependencies

* why pimpl idiom could reduce the compilation dependencies and save build time?
    * the pimpl idiom uses the forward declaration and a pointer to the implementation class; all those move the implementation details (class attribute definitions) from the header file to the source file.
        * use the pointer to hide the details of the implementation class, so forward declaration is sufficient here.
        * why cannot simply use the implementation class object as an attribute?
            * to use the implementation class object as an attribute, the class definition must be visible, which introduces compilation dependencies.
                * the key is to use declarations instead of definitions to reduce compilation dependencies.
    * any change to the class attribute definitions (now in the implementation class) will not trigger recompilation for source file that includes this header.

## validation
* build the project from scratch:
```
$ make all
g++    -c -o widget.o widget.cpp
g++    -c -o widget_util.o widget_util.cpp
g++    -c -o main.o main.cpp
g++ -g widget.o main.o widget_util.o -o main
```
* change the `WidgetImpl::draw` and remake, `widget_util.cpp` will not recompile
```
$ make all
g++    -c -o widget.o widget.cpp
g++ -g widget.o main.o widget_util.o -o main
```

# references
* https://learn.microsoft.com/en-us/cpp/cpp/pimpl-for-compile-time-encapsulation-modern-cpp
* https://stackoverflow.com/questions/2832714/header-files-inclusion-forward-declaration
