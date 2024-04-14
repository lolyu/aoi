# scoped locking in CPP
* there are multiple mutex ownership wrapper to provide scoped locking in C++:
    * `std::lock_guard`
    * `std::scoped_lock`
    * `std::unique_lock`
    * `std::shared_lock`

## lock_guard
* `lock_guard` is a RAII-style mutex ownership wrapper
    * `lock_guard` is non-copyable
    * `lock_guard` only allows one mutex

* demo code: https://github.com/lolyu/aoi/blob/master/books/ldxcfwqbc/02_thread_synchronization/lock_guard.cpp


## `scoped_lock`
* `scoped_lock` is a RAII-style mutex ownership wrapper
    * `scoped_lock` is non-copyable
    * `scoped_lock` allows ownership of multiple mutexes, offers deadlock-avoiding

* demo code: https://github.com/lolyu/aoi/blob/master/books/ldxcfwqbc/02_thread_synchronization/scoped_lock.cpp

## `unique_lock`
* `std::unique_lock` is a general purpose mutex ownership wrapper
    * 
