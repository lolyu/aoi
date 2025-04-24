# thread synchronization primitives

## atomic operation
* an `atomic operation` is an operation that will always be executed without any other process being able to read or change state that is read or changed during the operation:
    * **a read/write to a byte, half-word, or word (within the same cache line) is guaranteed to be atomic (thanks to cache coherence protocol)**
    * on multiprocessor or multicore systems, atomic operation can be achieved by:
        * lock on the bus
        * CAS (compare and set)

## locking
* two locking mechanisms:
    * mutex
        * if a thread tries to lock a mutex and it fails, it will go to sleep and allow other threads to run.
        * pros:
            * will not consume CPU once locking fails
        * cons:
            * reschedule overhead (puthing thread to sleep/waking thread up)
    * spinlock
        * if a thread tries to lock a spinlock and it fails, it will continuously retry locking it.
        * pros:
            * thread retrying locking a spinlock could continue to work immediately once other thread releases - better throughput
            * thread retrying locking can take advantage of full runtime quantum 
        * cons:
            * retrying locking consumes CPU
* spinlock outperforms mutex if the critical section can be finished in short quantum

## references
* https://wiki.osdev.org/Atomic_operation
