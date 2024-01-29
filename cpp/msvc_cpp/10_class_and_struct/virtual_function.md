# virtual function
* when you declare any function virtual, virtual function table is created **at compile time**
    * virtual function table is per class
    * virtual function table is just an array of virtual function pointers
* each class object will have a hidden virtual table pointer (`_vptr`), which points to the class's virtual function table.
    * `_vptr` is per object
    * `_vptr` is initialized/constructed during the constructor initialization list

## virtual table pointer init
* **NOTE**: the `_vptr` is initialized to the class's virtual function table in the constructor initialization list:
    * after the construction of bases
    * before the construction of members

### demonstration with gdb
```cpp
#include <iostream>

class Parent
{
public:
    Parent(int p = 10) : p(p) 
    {
        cout << "Parent Constructor" << endl;
    }

    virtual void Foo() {}
    virtual void FooNotOverridden() {}

    virtual ~Parent() = default;

private:
    int p;
};

class Derived : public Parent
{
public:
    Derived(int d = 11) : Parent(d + 1), d(getd())
    {
        cout << "Derived Constructor" << endl;
    }

    int getd() const
    {
        return 10;
    }

    void Foo() override {}

    virtual ~Derived() = default;

private:
    int d;
};

int main()
{
    Derived d0;
    return 0;
}
```
* set breakpoints at function `Parent::Parent`, `Derived::Derived` and `Derived::getd`
```
(gdb) run
Starting program: a.out 

Breakpoint 1, Parent::Parent (this=0x7fffffffdfe0, p=12) at virtual_demo.cpp:10
10              cout << "Parent Constructor" << endl;
(gdb) info vtbl this
vtable for 'Parent' @ 0x555555557d20 (subobject @ 0x7fffffffdfe0):
[0]: 0x5555555552b0 <Parent::Foo()>
[1]: 0x5555555552c0 <Parent::FooNotOverridden()>
(gdb) c
Continuing.
Parent Constructor

Breakpoint 3, Derived::getd (this=0x7fffffffdfe0) at virtual_demo.cpp:32
32              return 10;
(gdb) info vtbl this
vtable for 'Derived' @ 0x555555557cf0 (subobject @ 0x7fffffffdfe0):
[0]: 0x5555555553ca <Derived::Foo()>
[1]: 0x5555555552c0 <Parent::FooNotOverridden()>
(gdb) c
Continuing.

Breakpoint 2, Derived::Derived (this=0x7fffffffdfe0, d=11) at virtual_demo.cpp:27
27              cout << "Derived Constructor" << endl;
(gdb) info vtbl this
vtable for 'Derived' @ 0x555555557cf0 (subobject @ 0x7fffffffdfe0):
[0]: 0x5555555553ca <Derived::Foo()>
[1]: 0x5555555552c0 <Parent::FooNotOverridden()>
```

## references
* https://www.vishalchovatiya.com/part-1-all-about-virtual-keyword-in-cpp-how-virtual-function-works-internally/
* https://www.vishalchovatiya.com/part-2-all-about-virtual-keyword-in-cpp-how-virtual-class-works-internally/
* https://www.vishalchovatiya.com/part-3-all-about-virtual-keyword-in-c-how-virtual-destructor-works/
* https://stackoverflow.com/questions/10292973/c-how-does-virtual-function-resolve-this-pointer-scope-issue
