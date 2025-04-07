# thread synchronization primitives

## atomic operation
* an `atomic operation` is an operation that will always be executed without any other process being able to read or change state that is read or changed during the operation:
    * a read/write to a byte, half-word, or word (within the same cache line) is guaranteed to be atomic (thanks to cache coherence protocol)
    * on multiprocessor or multicore systems, atomic operation can be achieved by:
        * lock on the bus
        * CAS (compare and set)

## locking

## mutex

## spin lock


## references
* https://wiki.osdev.org/Atomic_operation
