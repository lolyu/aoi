# exit and abort with thread
* never call `exit` and `abort` in a mult-threaded process, because those two calls will leave the threads in a random state.
    * always join the threads before main thread returns/exits

## exit
* `std::exit` terminates a process but with some cleanups performed:
    1. non-local static objects are destroyed
    2. `std::atexit` handlers are called
    3. current thread thread-local variables are destroyed

## abort
* terminates the process with a `SIGABRT` signal.

## references
* https://en.cppreference.com/w/cpp/utility/program/exit
* https://en.cppreference.com/w/cpp/utility/program/abort
