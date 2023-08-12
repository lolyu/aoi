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

## references
* https://stackoverflow.com/questions/670734/pointer-to-class-data-member
* https://cplusplus.com/reference/new/operator%20new/
* https://cplusplus.com/reference/new/operator%20delete/
