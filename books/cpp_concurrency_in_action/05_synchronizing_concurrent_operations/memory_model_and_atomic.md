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

### std::atomic_flag
* `std::atomic_flag` is the simplest standard atomic type, which represents a boolean flag.
* `std::atomic_flag` is guaranteed to be lock-free.
