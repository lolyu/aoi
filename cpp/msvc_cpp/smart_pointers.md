# smart pointer
* smart pointers are crucial to RAII(resource acquisition is initialization)
* the memory/resource is managed by the C++ scoping rules:
    * resource acquisition is made when the smart pointer is initialized
    * resource release is made when the smart pointer is out-of-scope and destructed

* C++ smart pointers:
    * `unique_ptr`: allows exactly one owner of the underlying pointer, the ownership can be transferred
    * `shared_ptr`: reference-counted
    * `weak_ptr`: provide access to an object owned by one or more `shared_ptr` instances, but not participated in reference counting.
        * when to use:
            * when you want to observe an object, but do not require it to remain alive.
            * break circular references between shared_ptr instances.


## unique_ptr
* **NOTE**: `unique_ptr` cannot be copied to another `unique_ptr`, passed by value to a function, `unique_ptr` can only be moved.
* `unique_ptr` is as efficient as raw pointer in C++.

![image](https://github.com/lolyu/aoi/assets/35479537/e0fa99f5-c9b0-417d-bed3-793132475006)

### member functions
* `unique_ptr<T, delete>::reset(T *t)`: owns the new object, and delete the old one if it is non-empty
* `unique_ptr<T, delete>::release()`: releases the owned object, the caller is responsible for the object release.
* `unique_ptr<T, delete>::swap(unique_ptr<T, delete> t)`: swaps the owned object with another `unique_ptr`

```cpp
#include <iostream>
#include <memory>
#include <cassert>

class Demo
{
public:
    Demo(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

    void show() const { std::cout << x << " " << y << " " << z << std::endl; }

private:
    int x;
    int y;
    int z;
};

int main()
{
    std::unique_ptr<Demo> ptr0;
    std::unique_ptr<Demo> ptr1(nullptr);
    assert(ptr0.get() == nullptr);
    assert(ptr1.get() == nullptr);

    Demo *d0 = new Demo(10, 10, 10);
    Demo *d1 = new Demo(20, 20, 20);
    // use reset to assign to existed unique_ptr
    // ptr0 owns d0, ptr1 owns d1
    ptr0.reset(d0);
    ptr1.reset(d1);

    // ptr0 owns d1, ptr1 owns d0
    ptr0.swap(ptr1);

    // ptr1 release d0
    ptr1.release();
    // ptr0 reset with d0, d1 is freed
    ptr0.reset(d0);

    // when ptr0 will be destroyed, d0 is freed
    return 0;
}
```

### make_unique

```cpp
class Demo
{
public:
    Demo(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

    void show() const { std::cout << x << " " << y << " " << z << std::endl; }

private:
    int x;
    int y;
    int z;
};

auto ptr0 = std::make_unique<Demo>();
auto ptr1 = std::make_unique<Demo>(10, 20, 30);
auto ptr2 = std::make_unique<Demo[]>(3);
```

#### unique_ptr vs make_unique
```cpp
void function(std::unique_ptr<A> ptrA, std::unique_ptr<B> ptrB);

function(std::unique_ptr<A>(new A()), std::unique_ptr<B>(new B()));               // [1]
function(std::make_unique<A>(), std::make_unique<B>());                           // [2]
```
* calling `std::unique_ptr<A>(new A())` performs two allocations, one for the block allocation, one for the `unique_ptr`
* the execution order of `[1]` could be:
    * `new A()`
    * `new B()`
    * `std::unique_ptr<A>()`
    * `std::unique_ptr<B>()`
* if `new B()` throws an exception, memory leaks as the block allocated by `new A()` will never be freed.
* **NOTE**: this is not a program since C++17, as the evaluation of one argument to a function is required to fully execute before the evaluation of another.

## shared_ptr
* `shared_ptr` allows more than one owner, it can be copied, passed to function by value, and assigned to another `shared_ptr` instance.
* `shared_ptr` manages two entities:
    * the control block
    * the object block

### shared_ptr vs make_shared

## references
* https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
* https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared
