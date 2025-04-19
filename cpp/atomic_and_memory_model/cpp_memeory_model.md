# CPP memory model

* sequential consistency

```
# LoadLoad
if L(a) <p L(b) -> L(a) <m L(b)
# LoadStore
if L(a) <p S(b) -> L(a) <m S(b)
# StoreStore
if S(a) <p S(b) -> S(a) <m S(b)
# StoreLoad
if S(a) <p L(b) -> S(a) <m L(b)
```

* happens-before
    * program order relationship
* synchronizes-with

## memory models

|memory models|memory order|
|-|-|
|sequential consistent ordering|memory_order_seq_cst|
|release acquire ordering|<br>memory_order_acquire</br><br>memory_order_release</br><br>memory_order_acq_rel</br>|
|release consume ordering|<br>memory_order_consume</br><br>memory_order_release</br>|
|relaxed ordering|memory_order_relaxed|

* C++ memory orders:
    * `memory_order_seq_cst`
    * `memory_order_release`
        * no operation before this store can be reordered after this store.
    * `memory_order_acquire`
        * no operation after this load can be reordered before this load.
        * if this load `synchronizes-with` a store with `memory_order_release`, any writes (including non-atomic and relaxed atomic) before that store is visible to operations after this load.
        * **NOTE**: this synchronization is established only between the threads with release acquire semantics; other threads can see different order of memory accesses
    * `memory_order_consume`
        * no operation that **depends** on this load can be reordered before this load.
        * if this load is `dependency-ordered after` a store with `memory_order_release`, only the writes to the atomic is visible.
    * `memory_order_acq_rel`
        * sequential consistency
    * `memory_order_relaxed`
        * only atomic and modification order consistency guarantee
        * use case:
            * increment counters
            * thread loop stop flag

## references
* https://en.cppreference.com/w/cpp/atomic/memory_order
