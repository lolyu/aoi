# operators
* operators have two properties:
    * precedence: specifies the order of operations in expressions that contain more than one operator
    * associativity: specifies whether, in an expression that contains multiple operators with the same precedence, an operand is grouped with the one on its left or the one on its right.

## `alignof`
* `alignof` returns the alignment in bytes of the specified type

```cpp
struct Demo
{
    int i;
    double d;
    alignas(1024) char arr[3];
};

int main()
{
    using namespace std;
    cout << alignof(Demo) << endl;    // 1024

    void *p0 = malloc(sizeof(Demo) * 10);
    cout << p0 << endl;                                            // default aligned

    void *p1 = aligned_alloc(alignof(Demo), sizeof(Demo) * 10);    // aligned to 1024
    cout << p1 << endl;

    return 0;
}
```

## address-of `&`
* `&` is the address-of operator
    * it returns the address of its operand, the operand can be a function designator or an lvalue


### address of class member: static vs non-static
* the pointer to member data, it is more like declaring a member pointer that points to the member data.
* for any instance, the pointer to member data always points to the member data of this instance
    * https://www.ibm.com/docs/en/zos/2.2.0?topic=only-pointers-members-c
```
class D
{
public:
    int n;
    static int nn;
};

int D::nn = 10;

int main()
{
    using namespace std;
    int *p0 = &D::nn;
    int D::*p1 = &D::n;

    D d0, d1;
    d0.n = 10;
    d1.n = 11;

    cout << (d0.*p1) << endl;
    cout << &(d0.*p1) << endl;

    cout << (d1.*p1) << endl;
    cout << &(d1.*p1) << endl;
    return 0;
}
```

## assignment `=`
* two kinds of assignment operators:
    * simple assignment
    * compound assignment
        * `+=`
        * `-=`
        * `*=`
        * `/=`

## cast operator `()`

### class custom cast operator
```cpp
class D
{
public:
    D(int n = 10) : n(n) {}

    operator int() { return n; }

private:
    int n;
};

int main()
{
    D d;
    int n = d;

    return 0;
}
```

## `new` and `delete`
* `new` allocates dynamic memory from heap for the object type specified
    * allocate dynamic memory for the object
    * initialize the object - call the corresponding constructor
    * return a pointer to the object
* `delete` frees the dynamic memory allocated by `new`
    * call the object destructor
    * free the allocated dynamic memory

```cpp
void* operator new (std::size_t size);
void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) noexcept;
void* operator new (std::size_t size, void* ptr) noexcept;                                                // placement new
void operator delete (void* ptr) noexcept;
void operator delete (void* ptr, const std::nothrow_t& nothrow_constant) noexcept;
void operator delete (void* ptr, void* voidptr2) noexcept;
void operator delete (void* ptr, std::size_t size) noexcept;
void operator delete (void* ptr, std::size_t size, const std::nothrow_t& nothrow_constant) noexcept;
```

* **NOTE**: built-int `new` and `delete` are thread-safe
* `delete` is used to free the dynamic memory allocated by `new`
    * **NOTE**:
        * `delete` a pointer to an object not allocated by `new` leads to undefined behavior 
        * `delete` a pointer twice leads to undefined behavior
        * `delete` a `nullptr` does nothing and is harmless
```cpp
D *d = new D(100);
delete d;
// delete d; // core dump
d = nullptr;
delete d; // harmless
```



* when the compiler encounters the `new` operator to allocate an object of type `T`:
    * if user-defined `new` operator is present, issues a call to `T::operator new(sizeof(T))`
    * else, issues a call to `::operator new(sizeof(T))`


### `new` allocation failure
* `new` allocation failure will throw a `std::bad_alloc` exception
* `new(std::nothrow)` will suppress throwing `std::bad_alloc` exception and return `nullptr` in such case

```cpp
#include <iostream>
#include <new>

using namespace std;
#define BIG_NUMBER 10000000000000LL

int main()
{
    try
    {
        int *ptr0 = new int[BIG_NUMBER];
    }
    catch (const std::bad_alloc &e)
    {
        cout << "Allocation failure" << endl;
    }

    int *ptr1 = new (std::nothrow) int[BIG_NUMBER];
    if (ptr1 == nullptr)
    {
        cout << "Allocation failure" << endl;
    }
}
```

## pointer-to-member operators
* `.*` and `->*`
    * the pointer-to-member operators return the value of the specific class member(the right operand) for the object specified(the left operand).
        * **NOTE**: the pointer-to-member operator returns the member value instead of the member address
    * the left operand must be of a class type that is accessible to the member pointed by the right operand.


```cpp
#include <iostream>

class BaseClass
{
public:
    BaseClass() = default;
    void func1() { std::cout << "from BaseClass" << std::endl; }
    int data = 0;
};

class DerivedClass : public BaseClass
{
public:
    DerivedClass() = default;
    void func2() { std::cout << "from DerivedClass" << std::endl; }
};

void (BaseClass::*pmf1)() = &BaseClass::func1;
int BaseClass::*pmd1 = &BaseClass::data;
void (DerivedClass::*pmf2)() = &DerivedClass::func2;
int DerivedClass::*pmd2 = &DerivedClass::data;

int main()
{
    BaseClass b;
    DerivedClass d;

    (b.*pmf1)();
    std::cout << "Base member data " << (b.*pmd1) << std::endl;
    (d.*pmf1)();
    (d.*pmf2)();
    std::cout << "Derived member data " << (d.*pmd1) << std::endl;
    std::cout << "Derived member data " << (d.*pmd2) << std::endl;
    return 0;
}

```

## prefix and postfix ++/--
* prefix increment/decrement modifies the value before the value evaluation
    * `++x` equals to `x += 1`
    * `--x` equals to `x -= 1`
    * the result is the operand
* postfix increment/decrement does the value evaluation before modifying the value
    * the result is a prvalue copy of the original value of the operand

```cpp
A &operator++(A&); // prefix overload
A operator++(A&, int); // suffix overload
```

## relational operators
* `>`, `<`, `>=`, `<=`

### comparing pointers
* two pointers can be compared if:
    1. both point to the same type
    2. one pointer points to `0`
    3. one pointer is of `void *`
        * the other is implicitly converted to `void *` to do the comparison
    4. one type is a class type derived from another type
    5. at least one of the pointers is explicitly converted to `void *`
    6. both point to the same array or to the element one beyond the end of the array

* for nonstatic class member pointers:
    * for union, pointers to different data members compare equal
    * for non-union, and two members with the same access specifier(`public`, `protected`, and `private`), the one declared last is greater
    * for non-union, and two members with different access specifiers, the result is undefined

## sizeof
* `sizeof` returns the size of its operand with respect to the size of type `char`
* `sizeof` cannot be used with dynamically allocated arrays.
* **NOTE**: if an unsized array is the last element of a structure, the `sizeof` operator returns the size of the structure without the array.

```cpp
#include <iostream>
#include <cstring>

struct Demo
{
    int i;
    double d;
    char data[];
};

int main()
{
    char *data = new char[40];
    std::cout << sizeof(data) << std::endl;
    std::cout << sizeof(Demo) << std::endl;
    std::cout << alignof(Demo::i) << std::endl;
    std::cout << alignof(Demo::d) << std::endl;
    Demo *d = new (data) Demo();
    d->i = 10;
    d->d = 10.10;
    strcpy(d->data, "helloworld");
    std::cout << sizeof(d) << std::endl;
};
```

## subscription operator []
* for a subscript operation: `A[B]`, it equals to `*(A + B)`
    * so `A[B]` equals to `B[A]`

```cpp
#include <iostream>
#include <cstring>

struct Demo
{
    int i;
    double d;
    char data[];
};

int main()
{
    char *data = new char[40];
    std::cout << sizeof(data) << std::endl;
    std::cout << sizeof(Demo) << std::endl;
    std::cout << alignof(Demo::i) << std::endl;
    std::cout << alignof(Demo::d) << std::endl;
    Demo *d = new (data) Demo();
    d->i = 10;
    d->d = 10.10;
    strcpy(d->data, "helloworld");
    std::cout << sizeof(d) << std::endl;
};
```

## typeid
* `typeid` returns the actual derived type of the object referred to by a pointer or reference.
    * together with `dynamic_cast`, are provided for `RTTI`(runtime type identification) support in C++
* `typeid(expr)` returns an lvalue of type `const std::type_info` that represents the type of expression `expr`
* If `expr` is a reference or a dereferenced pointer to a polymorphic class, `typeid` will return a `type_info` object that represents the object that the reference or pointer denotes at run time.
* If `expr` is a reference or a dereferenced pointer to a non-polymorphic class, `typeid` will return a `type_info` object that represents the object that the reference or dereferenced pointer denotes at compile time(no runtime info).
* If `expr` is a pointer but not dereferenced, `typeid` will return a `type_info` object that represents the pointer or reference type

```cpp
#include <iostream>
#include <typeinfo>

// ./a.out |  c++filt -t

class Base0
{
public:
    Base0() = default;
};

class Derived0 : public Base0
{
public:
    Derived0() = default;
};

class Base1
{
public:
    Base1() = default;
    virtual void func() { std::cout << "Base1" << std::endl; }
};

class Derived1 : public Base1
{
public:
    Derived1() = default;
    virtual void func() { std::cout << "Derived1" << std::endl; }
};

int main()
{
    Base0 *p0 = new Base0;
    Base0 *p1 = new Derived0;
    Base0 &b0 = *p1;
    Base1 *p2 = new Base1;
    Base1 *p3 = new Derived1;
    Base1 &b1 = *p3;

    std::cout << typeid(p0).name() << std::endl; // Base0*
    std::cout << typeid(p1).name() << std::endl; // Base0*
    std::cout << typeid(p2).name() << std::endl; // Base1*
    std::cout << typeid(p3).name() << std::endl; // Base1

    std::cout << typeid(*p0).name() << std::endl; // Base0
    std::cout << typeid(*p1).name() << std::endl; // Base0
    std::cout << typeid(*p2).name() << std::endl; // Base1
    std::cout << typeid(*p3).name() << std::endl; // Derived1

    std::cout << typeid(b0).name() << std::endl; // Base0
    std::cout << typeid(b1).name() << std::endl; // Derived1

    return 0;
}
```

## unary +/-
* `+A` returns the value of `A`, but `A` is promoted following the rule of integral promotion
    * if `A` is of `char`, `+A` returns an int
* `-A` returns the negative of its operand also with integral promotion

## references
* https://stackoverflow.com/questions/670734/pointer-to-class-data-member
* https://cplusplus.com/reference/new/operator%20new/
* https://cplusplus.com/reference/new/operator%20delete/
