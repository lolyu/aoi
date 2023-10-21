# mutex practices


## guidelines
* `RAII`(resource acquisition is initialization)
* use non-recursive mutex
* don't call `lock()` and `unlock()` explicitly, use the local guard object(stored onto the stack)
* when using a guard object, always remember the locks acquired to enforce the locking sequence to avoid dead lock


## `RAII` with `mutex`
* `std::mutex` is usually not accessed directly, use the following to manage locking:
    * `std::unique_lock`
    * `std::lock_guard`
    * `std::scoped_lock`

### `std::mutex`
* `std::mutex`:
    * `lock()`: locks the mutex
    * `unlock()`: unlock the mutex`

* **NOTE**: `unlock()` synchronizes-with the subsequent `lock()` that obtains the ownership of the same mutex

### `std::lock_guard`
* `std::lock_guard`:
    * constructor: optionally locks the giving mutex
    * destructor: unlocks the underlying mutex
