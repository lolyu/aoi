# C++ memory model and atomic operations

## modification order
* `modification order` is composed of all the writes to the object from all threads in the program, starting from the object's initialization

## atomic operations
* an atomic operation is an indivisible operation

## C++ `atomic`
* the standard atomic types could be either lock-free or lock-based
    * the lock-free could be checked via:
        * member function `x.is_lock_free()`
        * static member variable `std::atomic<T>::is_always_lock_free`
        * macros `ATOMIC_<TYPE>_LOCK_FREE`: like `ATOMIC_CHAR_LOCK_FREE`
            * returns 2: always lock-free
            * returns 1: lock-free is a runtime property
            * returns 0: never lock-free

* the standard atomic types are not copyable or assignable
    * because the copy-construction or copy-assignment operations involves two objects, the value must first be read from one object and then written to another
        * **two operations on two separate objects, and the combination cannot be atomic**
* the assignment operators of the standard atomic types return value instead of reference.
    * if a reference is returned, the following code might have a chance to load the value, potentially getting the result of a modification by another thread.

### std::atomic_flag
* `std::atomic_flag` is the simplest standard atomic type, which represents a boolean flag.
* `std::atomic_flag` is guaranteed to be lock-free.
* `std::atomic_flag` must be initialized with `ATOMIC_FLAG_INIT` explicitly.

#### std::atomic<bool>
* `compare_exchange_weak(T &expected, T desired, std::memory_order success, std::memory_order failure)`
    * compares `*this` with `expected` bitwise
        * if same, replace `*this` with `desired`
            * returns `true`
        * else, load `*this` into `expected
            * returns `false`
    * A weak compare-and-exchange operation may fail spuriously, **that is, even when the contents of memory referred to by expected and object are equal, it may return false and store back to expected the same memory contents that were originally there**
        * use `compare_exchange_strong` instead
        * add extra check on the `exptected`

    * why does `spurious failure` matter?
        * `spurious failure` is a false alarm, it means the atomic variable is not actually set


```cpp
bool expected=false;
extern atomic<bool> b; // set somewhere else
while(!b.compare_exchange_weak(expected,true) && !expected);
```
```
class Singleton
{
private:
    static std::atomic<Singleton *> cached_object;

public:
    Singleton() : data(++count) {}

    ~Singleton() = default;

    int get_data() { return data; }

    static Singleton *get_object()
    {
        Singleton *obj = cached_object.load();
        if (!obj)
        {
            obj = new Singleton();
            if (obj)
            {
                Singleton *cached = nullptr;
                // if compare_exchange_weak returns true
                // => the cached_object is replaced with the new object created by this thread
                // if compare_exchange_weak returns false
                // => 1. spurious failure, the cached_object is still a nullptr, should retry
                // => 2. not a spurious failure, the cached_object is not a nullptr(created by another thread), should exit
                while (!cached_object.compare_exchange_weak(cached, obj) && !cached)
                {
                    ;
                }
                if (cached)
                {
                    delete obj;
                    obj = cached;
                }
            }
        }
        return obj;
    }

    static int count;
    int data;
};

int Singleton::count = 0;
std::atomic<Singleton *> Singleton::cached_object;
```

* `compare_exchange_strong(T &expected, T desired, std::memory_order success, std::memory_order failure)`

## references
* https://devblogs.microsoft.com/oldnewthing/20180330-00/?p=98395
