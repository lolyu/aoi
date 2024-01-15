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

![image](https://github.com/lolyu/aoi/assets/35479537/1720ecd2-9cb4-4058-9d95-552c016221ec)

### member function
* `shared_ptr<T, delete>::reset(T *t)`: replace the managed object with `*t`
    * if `*this` already owns an object and it is the last `std::shared_ptr` owning it, the object is destroyed
* `shared_ptr<T, delete>::swap(shared_ptr<T, delte> t)`: exchanges the stored pointer values and the ownerships of `*this` and `t`
* `shared_ptr<T, delete>::use_count`: returns the numbers of `shared_ptr` instances referring to the same managed object
* `shared_ptr<T, delete>::unique`: checks if the managed object is only managed by current `shared_ptr` instance
* `shared_ptr<T, delete>::owner_before`:

### shared_ptr vs make_shared

```cpp
std::shared_ptr<A> sp0 = std::shared_ptr<A>(new A());            // [1]
std::shared_ptr<A> sp1 = std::make_shared<A>();                  // [2]
```
* as a `std::shared_ptr` manages two entities: the control block and the object being managed, `std::make_shared<A)()`([2]) performs a single heap-allocation for the space necessary for both the control block and the data.
* `std::shared_ptr<A>(new A())` performs two heap allocations.

### aliasing shared_ptr constructor
```cpp
std::shared_ptr(shared_ptr<A> &&a, element_type *ptr) noexcept;
```
* the aliasing constructor:
    * shares the ownership information with `a`
        * shares the reference count with `a`
    * points to the object managed by `ptr`
        * shares the data with `ptr`

```cpp
struct Bar { 
    // some data that we want to point to
};

struct Foo {
    Bar bar;
};

shared_ptr<Foo> f = make_shared<Foo>(some, args, here);
shared_ptr<Bar> specific_data(f, &f->bar);

// ref count of the object pointed to by f is 2
f.reset();

// the Foo still exists (ref cnt == 1)
// so our Bar pointer is still valid, and we can use it for stuff
some_func_that_takes_bar(specific_data);
```

### casting a shared_ptr
* `dynamic_pointer_cast`
* `static_pointer_cast`
* `const_pointer_cast`
```cpp
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

class Base
{
    public:
        Base() { std::cout << "Base constructor" << std::endl; }
        virtual ~Base() { std::cout << "Base destructor" << std::endl; }
        virtual void show() const { std::cout << "Base show" << std::endl; }
    private:
};

class Derived0: public Base
{
    public:
        Derived0() { std::cout << "Derived0 constructor" << std::endl; }
        ~Derived0() { std::cout << "Derived0 destructor" << std::endl; }
        void show() const { std::cout << "Derived0 show" << std::endl; }
};

class Derived1: public Base
{
    public:
        Derived1() { std::cout << "Derived1 constructor" << std::endl; }
        ~Derived1() { std::cout << "Derived1 destructor" << std::endl; }
        void show() const { std::cout << "Derived1 show" << std::endl; }
};

int main()
{
    using namespace std;
    vector<shared_ptr<Base>> elements = {
        make_shared<Derived0>(),
        make_shared<Derived1>(),
        make_shared<Derived1>(),
        make_shared<Derived0>(),
    };

    cout << "All elements:" << endl;
    for (auto &e : elements)
    {
        e->show();
    }

    vector<shared_ptr<Base>> elements0;
    copy_if(elements.begin(), elements.end(), back_inserter(elements0), [](const shared_ptr<Base> &ptr) -> bool
    {
        return dynamic_pointer_cast<Derived0>(ptr).get() != nullptr;
    });

    cout << "Derived0:" << endl;
    for (auto &e : elements0)
    {
        e->show();
    }

    vector<shared_ptr<Base>> elements1;
    copy_if(elements.begin(), elements.end(), back_inserter(elements1), [](const shared_ptr<Base> &ptr) -> bool
    {
        return dynamic_pointer_cast<Derived1>(ptr).get() != nullptr;
    });

    cout << "Derived1:" << endl;
    for (auto &e : elements1)
    {
        e->show();
    }

    return 0;
}
```

### pass `shared_ptr` to a function
* three ways:
    * pass `shared_ptr` by value
        * invokes the `shared_ptr` constructor, allocates the control block, and increments the reference count - **overhead**
        * the callee becomes an owner, which could keep the managed object alive
    * pass `shared_ptr` by reference(const reference)
        * on construction, no reference increment
        * the callee can become an owner if it creates a `shared_ptr` based on the reference
    * pass `shared_ptr` by the managed object or pointer
        * the callee could not share the ownership
* if the callee has to share the ownership(the caller doesn't guarantee the `shared_ptr` is always in-scope during the callee execution time), pass `shared_ptr` by value
* if the caller can guarantee that the `shared_ptr` is always in-scope during the callee execution time, pass by reference, by raw pointer or by managed object.

## weak_ptr
* `std::weak_ptr` holds a non-owning reference to an object that is managed by `std::shared_ptr`
    * `std::weak_ptr` must be converted to `std::shared_ptr` to access the managed object

### member functions
* `weak_ptr<T>::reset`: release the ownership of the managed object
* `weak_ptr<T>::swap(weak_ptr<T> &r )`: exchanges the stored pointer values and ownership of `*this` and `r`
* `weak_ptr<T>::use_count`: returns the numbers of `shared_ptr` instances referring to the same managed object
* `weak_ptr<T>::lock()`: creates a new `shared_ptr` that shares the ownership of the managed object
    * if there is no managed object, the returned `shared_ptr` is empty
* `weak_ptr<T>::expired()`: same as `use_count() == 0`, returns `true` is the managed object has already been deleted

### use `weak_ptr` to break cyclic dependency
```cpp
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Controller
{
    public:
    int num;
    string status;
    vector<weak_ptr<Controller>> others;
    
    explicit Controller(int i) : num(i), status("on")
    {
        cout << "Creating controller " << num << endl;
    }

    ~Controller()
    {
        cout << "Destroying controller " << num << endl; 
    }

    void check_status() const
    {
        for_each(others.begin(), others.end(), [](weak_ptr<Controller> wp)
        {
            auto sp = wp.lock();
            if (sp)
            {
                cout << "status of " << sp->num << " = " << sp->status << endl;
            }
            else
            {
                cout << "null object" << endl;
            }
        });
    }
};

void run_test()
{
    vector<shared_ptr<Controller>> v{
        make_shared<Controller>(0),
        make_shared<Controller>(1),
        make_shared<Controller>(2),
        make_shared<Controller>(3),
        make_shared<Controller>(4),
        make_shared<Controller>(5),
        make_shared<Controller>(6),
        make_shared<Controller>(7),
    };

    for (auto &sp : v)
    {
        for_each(v.begin(), v.end(), [&](shared_ptr<Controller> &sp_other){
            if (sp->num != sp_other->num)
            {
                sp->others.push_back(weak_ptr<Controller>(sp_other));
                cout << "push back to controller " << sp->num << ": " << sp_other->num << endl;
            }
        });
    }

    for (auto &sp : v)
    {
        cout << "controller " << sp->num << " use count = " << sp.use_count() << endl;
        sp->check_status();
    }
}

int main()
{
    run_test();
    return 0;
}
```

## other tools

### enable_shared_from_this
* `enable_shared_from_this` allows an object `t` that is currently managed by a `std::shared_ptr` named `pt` to safely generate additional `std::shared_ptr` instances that share the ownership of `t` with `pt`
    * if `t` is not managed by a `shared_ptr`, `shared_ptr` constructor will throw a `std::bad_weak_ptr` exception.
* two member functions:
    * `shared_from_this`: returns a `shared_ptr` which shares the ownership of `*this`
    * `weak_from_this`: returns a `weak_ptr` which shares the ownership of `*this`
* `enable_shared_from_this` has a `weak_ptr` member, and it is initialized by the `shared_ptr` constructor, the `shared_ptr` constructor can detect if `T` is derived from `enable_shared_from_this`. If it is, the `shared_ptr` constructor will assign `*this` to the `weak_ptr` member.
    * `shared_from_this` creates a `shared_ptr` out of the `weak_ptr` member
    * `weak_from_this` returns the `weak_ptr` member

```cpp
#include <iostream>
#include <memory>

class Good : public std::enable_shared_from_this<Good>
{
public:
    Good() = default;
    ~Good() = default;

    std::shared_ptr<Good> get_shared_self()
    {
        return shared_from_this();
    }
};

class Bad
{
public:
    Bad() = default;
    ~Bad() = default;
    std::shared_ptr<Bad> get_shared_self()
    {
        return std::shared_ptr<Bad>(this);
    }
};

int main()
{
    auto good0 = std::make_shared<Good>();
    auto good1 = good0->get_shared_self();

    // double delete of bad
    auto bad0 = std::make_shared<Bad>();
    // auto bad1 = bad0->get_shared_self();

    // shared_from_this must be called from an object managed by shared_ptr
    Good good2;
    try
    {
        auto good3 = good2.get_shared_self();
    }
    catch (const std::bad_weak_ptr &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
```

## references
* https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
* https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared
* https://stackoverflow.com/questions/27109379/what-is-shared-ptrs-aliasing-constructor-for
