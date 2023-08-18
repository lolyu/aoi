# C++ memory order
* memory order specifies how memory accesses(read/write) are to be ordered around an atomic operation.

## formal description

* `sequenced-before`
    * an asymmetric, transitive, pair-wise relationship between evaluations within the **same thread**
    * if `A` is sequenced before `B`, the evaluation of `A` will be completed before `B`

* `carries dependency`
    * if `A` carries dependency into `B`: `A` must be sequenced before `B`, and the evaluation of `B` depends on the evaluation of `A`

* `release sequence`
    * after a release operation `A` is performed on an atomic object `M`, `release sequence` is the longest continuous subsequence of the modification order of `M` is consisted of:
        * any writes performed by the same thread after `A`
        * atomic read-modify-write operations made to `M` by any thread

* **`synchronizes with`**:
    * if thread `A` performs a store release and thread `B` performs a load acquire, and the load in thread `B` reads the value written by the store in `A`
    * then the store in thread `A` synchronizes with the load in thread `B`
    * `synchronizes with` is a runtime relationship between threads

* **`dependency-ordered before`**
    * between threads, evaluation `A` is `dependency-ordered before` evaluation of `B` if any of the following is true:
        * `A` performs a release on an atomic `M`, and in thread `B`, `B` performs a consume operation on `M` and reads the value written by any part of the `release sequence` headed by `A`
        * `A` is `dependency-ordered before` `X` and `X` carries dependency into `B`

* `inter-thread happens-before`
    * between threads, evaluation `A` inter-thread happens before evaluation `B` if any of the following is true:
        * `A` synchronizes-with `B`
        * `A` is dependency-ordered before `B`

* `happens-before`
    * evaluation `A` happens before `B` if any of the following is true:
        * `A` is sequenced-before `B`
        * `A` inter-thread happens before `B`
            * `A` synchronizes-with `B`
            * `A` is dependency-ordered before `B`
    * if one evaluation `A` modifies a memory location, and another evaluation `B`(either read/write) occurs at the same memory, if at least one of the evaluations is not an atomic operation, the behavior is undefined(race condition) unless there exists a **`happens-before** relationship between those two threads.

* `simply happens-before`:
    * evaluation `A` simply happens before `B` if any of the following is true:
        * `A` is sequenced-before `B`
        * ~~`A` inter-thread happens before `B`~~
            * `A` synchronizes-with `B`
            * ~~`A` is dependency-ordered before `B`~~
    * without consume operations, `simply happens-before` equals to `happens-before`

* `visible side-effects`:
    * the side-effect `A` on a scalar `M`(a write) is visible with respect to value computation `B` on `M`(a read) if both of the following are true:
        * `A` happens before `B`
        * no side effect `X` to `M` that `A` happens-before `X` and `X` happens-before `B`
    * **inter-thread synchronization is all about defining what side effects become visible under what conditions**

## orderings
### relaxed ordering
* `std::memory_order_relaxed` imposes no ordering and synchronization requirements, so `atomic` only guarantees atomicity and modification order consistency.

![image](https://github.com/lolyu/aoi/assets/35479537/07c3ace1-aa3d-4180-96ca-6c2db18daf4b)

* in the above example, it is allowed that `r1 == r2 == 42`
    * if there is a reordering in the right thread that `C` and `D` are reordered, and the execution order is `D` -> `A` -> `B` -> `C`

* `std::memory_order_relaxed` is typically used in counter increment
* `std::shared_ptr` counter increment uses `std::memory_order_relaxed`, but the decrement requires the release semantics
* `std::memory_order_relaxed`:
    * no synchronization and ordering constraints imposed on the reads/writes
    * only atomicity is guaranteed

#### counter example
* **NOTE**: the thread execution **synchronizes with** the thread `join`, and the `join` is **sequenced before** the read of `counter` in the main thread ->
    * the thread execution **inter-thread happens before** the read of `counter` in the main thread
    * the thread execution **happens before** the read of `counter` in the main thread
    * so the thread execution generates **visible side-effects** on the read of `counter` in the main thread
```cpp
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<int> counter = {0};

void f()
{
    for (int i = 0; i < 1000; ++i)
    {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back(std::thread(f));
    }

    for (auto &thr : threads)
    {
        thr.join();
    }
    std::cout << counter << std::endl;
    return 0;
}
```

### release-acquire ordering
* what is release acquire ordering?
    * release-acquire ordering is to implement the `synchronizes-with` relationship:
        * an atomic store in thread `A` is with `std::memory_order_release`
        * an atomic load in thread `B` from the same variable is tagged `memory_order_acquire`
        * the atomic load in thread `B` reads a value written by the store in thread `A` or a value from later in the release sequence
* what synchronization will be guaranteed in the release acquire ordering?
    * all memory writes(including non-atomic and relaxed atomic) that happened-before the atomic store from the point of view of thread `A`, become **visible side-effects** in thread `B`.
    * so once the atomic load finishes in thread `B`, thread `B` is guaranteed to see everything thread `A` wrote to memory before the atomic store.

* the release-acquire ordering is achieved by the acquire release semantics:
    * `acquire semantic`:
        * applied to read operations, called `load acquire` or `read acquire`
        * prevents memory reordering of the `load acquire` with any read/write operations that follow it in the program order
    * `release semantic`
        * applied to write operations, called `store release` or `write release`
        * prevents memory reordering of `write release` with any read/write operations that precede it in the program order

* so once the `store release` is paired with a `load acquire` in an another thread, the writes before the `store release` is visible to the `load acquire` and the reads afterwards. 
* **NOTE**: the key of pairing `release` with `acquire` is that the **`acquire` must see the memory modifications of the `release` or the memory modifications from later in the release sequence**
* `std::memory_order_acquire`:
    * a load with `std::memory_order_acquire` is an `acquire semantic`
    * all writes in other threads that release the same atomic variable are visible in the current thread
* `std::memory_order_release`:
    * a store with `std::memory_order_release` is a `release semantic`

![image](https://github.com/lolyu/aoi/assets/35479537/f5ff9c4c-d45f-4994-9455-01736c7a78a2)

#### release-acquire example
```cpp
#include <atomic>
#include <thread>
#include <string>
#include <cassert>

std::atomic<bool> guard;
std::string *data = nullptr;

void producer()
{
    data = new std::string("helloworld");
    guard.store(true, std::memory_order_release);
}

void consumer()
{
    bool expected = true;
    // compare_exchange_weak returns true:
    // guard is set to true, which equals to expected, so guard is changed to false, loop breaks
    // compare_exchange_weak returns false:
    // 1. spurious failure: guard is set to true, and expected is changed to the same as guard(true), need
    // to continue the loop to assign false to guard
    // 2. not spurious failure: guard is set to false, and expected is changed to the same as guard(false),
    // need to continue the loop to pair a store true in the producer, also need to change expected
    // back to true
    while (!guard.compare_exchange_weak(expected, false, std::memory_order_acquire) and !expected)
    {
        expected = true;
    }
    // the assert never breaks
    assert(*data == "helloworld");
}

int main()
{
    std::thread thrd1(producer);
    std::thread thrd2(consumer);
    thrd1.join();
    thrd2.join();
    return 0;
}
```

### release-consume ordering
* what is release consume ordering?
    * release-consume ordering is to implement the `dependency-ordered before` relationship
        * an atomic store in thread `A` is with `std::memory_order_release`
        * an atomic load in thread `B` from the same variable is tagged `memory_order_consume`
        * the atomic load in thread `B` reads a value written by the store in thread `A` ~~or a value from later in the release sequence~~
* what synchronization will be guaranteed in the release acquire ordering?
    * all memory writes(including non-atomic and relaxed atomic) that happened-before the atomic store from the point of view of thread `A`, become **visible side-effects** within the operations in thread `B` into which the load operation carries dependency.
    * in other words, if `A` store release on a variable `M`, the load acquire in `B` and reads afterward could only see the writes to `M` before the `A` store release.
    * **`release consume` ordering limits what the load consume thread could see**
* `std::memory_order_consume`:
    * a load operation with `std::memory_order_consume` means no reads/writes in the current thread dependent on the value loaded can be reordered before this load
    * writes to data-dependent variables in other threads that release the atomic variable are visible in the current thread

#### release-consume example
```cpp
#include <atomic>
#include <thread>
#include <string>
#include <cassert>

std::atomic<bool> guard;
std::string *data = nullptr;

void producer()
{
    data = new std::string("helloworld");
    guard.store(true, std::memory_order_release);
}

void consumer()
{
    bool expected = true;
    // compare_exchange_weak returns true:
    // guard is set to true, which equals to expected, so guard is changed to false, loop breaks
    // compare_exchange_weak returns false:
    // 1. spurious failure: guard is set to true, and expected is changed to the same as guard(true), need
    // to continue the loop to assign false to guard
    // 2. not spurious failure: guard is set to false, and expected is changed to the same as guard(false),
    // need to continue the loop to pair a store true in the producer, also need to change expected
    // back to true
    while (!guard.compare_exchange_weak(expected, false, std::memory_order_consume) and !expected)
    {
        expected = true;
    }
    // the assert **could** breaks
    assert(*data == "helloworld");
}

int main()
{
    std::thread thrd1(producer);
    std::thread thrd2(consumer);
    thrd1.join();
    thrd2.join();
    return 0;
}
```

### sequential consistent ordering
* what is sequential consistent ordering?
    * for atomic operations of `std::memory_order_seq_cst`, they follows:
        * release-acquire ordering
        * a single total modification order exists
            * all threads see those operations'
            * execution as a sequence following this single total modification order
* total sequential ordering requires a full memory fence CPU instruction on all multi-core systems. This may become a performance bottleneck since it forces the affected memory accesses to propagate to every core.
* `std::memory_order_seq_cst`:
    * a load with `std::memory_order_seq_cst` is a load acquire
    * a store with `std::memory_order_seq_cst` is a store release
    * a RMW(read-modify-write`) with `std::memory_order_seq_cst` is both load acquire and store release
    * a total order exsits in which all threads observe all modifications in the same order

#### sequential consistent ordering example
* as there exists a total order, so `read_x_then_y` and `read_y_then_x` observes the same execution order of `write_x` and `write_y`.
```cpp
#include <thread>
#include <atomic>
#include <cassert>

std::atomic<bool> x = {false};
std::atomic<bool> y = {false};
std::atomic<int> z = {0};

void write_x()
{
    x.store(true, std::memory_order_seq_cst);
}

void write_y()
{
    y.store(true, std::memory_order_seq_cst);
}

void read_x_then_y()
{
    while (!x.load(std::memory_order_seq_cst))
        ;
    if (y.load(std::memory_order_seq_cst))
    {
        ++z;
    }
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_seq_cst))
        ;
    if (x.load(std::memory_order_seq_cst))
    {
        ++z;
    }
}

int main()
{
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load() != 0); // will never happen
}
```

## references
* https://preshing.com/20130823/the-synchronizes-with-relation/
* https://stackoverflow.com/questions/38565650/what-does-release-sequence-mean#:~:text=In%20summary%3A%20the%20release%20sequence,anyway%20due%20to%20being%20atomic).
* https://en.cppreference.com/w/cpp/atomic/memory_order
