# pointer to implementation idiom
* pimpl idiom is used to hide the implementation away from the class interface:
    * pimpl idiom could reduce the compilation dependencies

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
