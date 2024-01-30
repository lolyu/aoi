# virtual function
* when you declare any function virtual, virtual function table is created **at compile time**
    * virtual function table is per class
    * virtual function table is just an array of virtual function pointers
    * when the class is inherited by another one, the derived class will duplicate the base class virtual function table and replace the virtual function pointers with addresses of the derived class overridden ones.
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

using namespace std;

class Parent
{
public:
    Parent(int p = 10) : p(p) 
    {
        cout << "Parent Constructor" << endl;
    }

    virtual void Foo() {}
    virtual void FooNotOverridden() {}
    virtual void Bar() = 0;

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
    void Bar() override {}

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
vtable for 'Parent' @ 0x555555557d18 (subobject @ 0x7fffffffdfe0):
[0]: 0x5555555552b0 <Parent::Foo()>
[1]: 0x5555555552c0 <Parent::FooNotOverridden()>
[2]: 0x7ffff7e7f120 <__cxa_pure_virtual>
(gdb) c
Continuing.
Breakpoint 3, Derived::getd (this=0x7fffffffdfe0) at virtual_demo.cpp:33
33              return 10;
(gdb) info vtbl this
vtable for 'Derived' @ 0x555555557ce0 (subobject @ 0x7fffffffdfe0):
[0]: 0x5555555553ca <Derived::Foo()>
[1]: 0x5555555552c0 <Parent::FooNotOverridden()>
[2]: 0x5555555553da <Derived::Bar()>
(gdb) c
Continuing.
Breakpoint 2, Derived::Derived (this=0x7fffffffdfe0, d=11) at virtual_demo.cpp:28
28              cout << "Derived Constructor" << endl;
(gdb) info vtbl this
vtable for 'Derived' @ 0x555555557ce0 (subobject @ 0x7fffffffdfe0):
[0]: 0x5555555553ca <Derived::Foo()>
[1]: 0x5555555552c0 <Parent::FooNotOverridden()>
[2]: 0x5555555553da <Derived::Bar()>
```

## pure virtual function
* pure virtual function is a placeholder in the virtual function table, means that it must be overridden by the derived classes.
    * from the above example, the function pointer slot for `Bar()` is a placeholder called `__cxa_pure_virtual`.

## multiple inheritance
```cpp
#include <iostream>

//
// # sudo bash -c 'echo 0 > /proc/sys/kernel/randomize_va_space'
// # g++ -fno-stack-protector -z execstack -no-pie  -g multiple_inheritance.cpp
//

class Base0
{
public:
    Base0(int b0 = 10) : b0(b0)
    {
        std::cout << "Base0()" << std::endl;
    }
    virtual ~Base0() = default;

    virtual void foo() {};
    virtual void bar() {};
    virtual void setb0(int b) { b0 = b; };

private:
    int b0;
};

class Base1
{
public:
    Base1(int b1 = 10) : b1(b1)
    {
        std::cout << "Base1()" << std::endl;
    }
    virtual ~Base1() = default;

    virtual void foo() {};
    virtual void bar() {};
    virtual void setb1(int b) { b1 = b; };

private:
    int b1;
};

class Derived : public Base0, public Base1
{
public:
    Derived() = default;
    Derived(int b0, int b1) : Base0(0), Base1(0) { newb0 = b0; newb1 = b1; };
    virtual ~Derived() = default;

    virtual void foo() override {};
    virtual void bar() override {};
    virtual void setb0(int b) override { newb0 = b; };
    virtual void setb1(int b) override { newb1 = b; };

private:
    int newb0;
    int newb1;
};

int main()
{
    Derived d0{10, 20};
    Derived *dptr = &d0;
    Base0 *b0ptr = static_cast<Base0 *>(dptr);
    Base1 *b1ptr = static_cast<Base1 *>(dptr);

    Base0 b0;
    Base1 b1;

    std::cout << "END" << std::endl;
    return 0;
}
```
```
// nm -C a.out
...
000000000040155c W Derived::bar()
0000000000401542 W Derived::foo()
0000000000401576 W Derived::setb0(int)
0000000000401592 W Derived::setb1(int)
00000000004014ae W Derived::Derived(int, int)
00000000004014ae W Derived::Derived(int, int)
000000000040160e W Derived::~Derived()
00000000004015b8 W Derived::~Derived()
00000000004015b8 W Derived::~Derived()
...
0000000000403de0 V typeinfo for Base0
0000000000403dd0 V typeinfo for Base1
0000000000403d98 V typeinfo for Derived
0000000000402030 V typeinfo name for Base0
0000000000402029 V typeinfo name for Base1
0000000000402020 V typeinfo name for Derived
0000000000403d60 V vtable for Base0
0000000000403d28 V vtable for Base1
0000000000403cb0 V vtable for Derived
                 U vtable for __cxxabiv1::__class_type_info@@CXXABI_1.3
                 U vtable for __cxxabiv1::__vmi_class_type_info@@CXXABI_1.3
000000000040156b W non-virtual thunk to Derived::bar()
0000000000401551 W non-virtual thunk to Derived::foo()
00000000004015ae W non-virtual thunk to Derived::setb1(int)
000000000040163d W non-virtual thunk to Derived::~Derived()
0000000000401604 W non-virtual thunk to Derived::~Derived()
```

```
(gdb) info vtbl &d0
vtable for 'Derived' @ 0x403cc0 (subobject @ 0x7fffffffdfc0):
[0]: 0x4015b8 <Derived::~Derived()>
[1]: 0x40160e <Derived::~Derived()>
[2]: 0x401542 <Derived::foo()>
[3]: 0x40155c <Derived::bar()>
[4]: 0x401576 <Derived::setb0(int)>
[5]: 0x401592 <Derived::setb1(int)>

vtable for 'Base1' @ 0x403d00 (subobject @ 0x7fffffffdfd0):
[0]: 0x401604 <non-virtual thunk to Derived::~Derived()>
[1]: 0x40163d <non-virtual thunk to Derived::~Derived()>
[2]: 0x401551 <non-virtual thunk to Derived::foo()>
[3]: 0x40156b <non-virtual thunk to Derived::bar()>
[4]: 0x4015ae <non-virtual thunk to Derived::setb1(int)>
(gdb) info vtbl &b0
vtable for 'Base0' @ 0x403d70 (subobject @ 0x7fffffffdfb0):
[0]: 0x401412 <Base0::~Base0()>
[1]: 0x401430 <Base0::~Base0()>
[2]: 0x401344 <Base0::foo()>
[3]: 0x401354 <Base0::bar()>
[4]: 0x401364 <Base0::setb0(int)>
(gdb) info vtbl &b1
vtable for 'Base1' @ 0x403d38 (subobject @ 0x7fffffffdfa0):
[0]: 0x401460 <Base1::~Base1()>
[1]: 0x40147e <Base1::~Base1()>
[2]: 0x4013d6 <Base1::foo()>
[3]: 0x4013e6 <Base1::bar()>
[4]: 0x4013f6 <Base1::setb1(int)>
(gdb) p ((size_t **)b0)
$14 = (size_t **) 0x403d70 <vtable for Base0+16>
(gdb) p ((size_t **)b1)
$15 = (size_t **) 0x403d38 <vtable for Base1+16>
(gdb) p ((size_t **)d0)
$16 = (size_t **) 0x403cc0 <vtable for Derived+16>
quit) 
```
* class `Derived` object has two `_vptr`, one for `Base0` and one for `Base1`.
    * one `_vptr` in the `Base0` memory section of `Derived` object
    * one `_vptr` in the `Base1` memory section of `Derived` object
* each class's virtual function table contains the type info meta data of the class at the beginning

* the class object's `_vptr` points to the beginning of the virtual function pointers in the virtual function table
    * `_vptr` is the offset 16 of the class virtual function table.

![image](https://github.com/lolyu/aoi/assets/35479537/55b60f46-3599-40c3-abfa-5f998a34a6d8)

* for class that has multiple base classes, the compiler will generate one large virtual function table. The class subobject's `_vptr` points to different section of the class virtual function table.
    * the class object and the class's first subobject shares the same `_vptr`
    * the other class subjects have its own `_vptr` that points to the offset that stores a copy of the base's virtual function table, but replaced with thunk functions.
```
(gdb) disassemble 0x401551
Dump of assembler code for function _ZThn16_N7Derived3fooEv:
   0x0000000000401551 <+0>:     endbr64 
   0x0000000000401555 <+4>:     sub    $0x10,%rdi
   0x0000000000401559 <+8>:     jmp    0x401542 <Derived::foo()>
End of assembler dump.
```
* the thunk function basically does two things:
    * convert the subobject pointer to the class pointer
    * call the corresponding virtual function defined in the class virtual function table
* `b1ptr->foo()` is same as `((Derived*)b1ptr)->foo()`, and `this` pointer is the `Derived *` instead of `Base1 *`

## virtual inheritance

## virtual destructor

## references
* https://www.vishalchovatiya.com/part-1-all-about-virtual-keyword-in-cpp-how-virtual-function-works-internally/
* https://www.vishalchovatiya.com/part-2-all-about-virtual-keyword-in-cpp-how-virtual-class-works-internally/
* https://www.vishalchovatiya.com/part-3-all-about-virtual-keyword-in-c-how-virtual-destructor-works/
* https://stackoverflow.com/questions/10292973/c-how-does-virtual-function-resolve-this-pointer-scope-issue
