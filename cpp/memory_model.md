# memory model
*  a consistency model specifies a contract between the programmer and a system, wherein the system guarantees that if the programmer follows the rules for operations on memory, memory will be consistent and the results of reading, writing, or updating memory will be predictable.
*  how to define and categorize consistency mode?
    * `issue`: describes the restrictions that define how a process can issue operations
    * `view`: defines the order of operations visible to processes


## memory modes

### strong consistency models
#### strict consistency
* `strict consistency` guarantees that a write to a variable by any processor needs to be seen instantaneously by all processors

#### sequential consistency
* `sequential consistency` guarantees that the execution order of a program in the same processor (or thread) is the same as the program order, while the execution order of a program on different processors (or threads) is undefined.


![image](https://github.com/lolyu/aoi/assets/35479537/59a91135-2b46-41c0-a82b-558b3183fdfe)
* on `processor 1`, the execution order is guaranteed to be `A1` -> `B1` -> `C1`, on `processor 2`, the execution order is guaranteed to be `A2` -> `B2`
    *  execution order between processors is undefined, sequential consistency doesn't restrict how memory access operations on different processors are interleaved.

* two requirements
    * `program order`: Program order guarantees that each process issues a memory request ordered by its program.
    * `write atomicity`: Write atomicity defines that memory requests are serviced based on the order of a single FIFO queue.

## causal consistency
* `causal consistency` guarantees that all processors observe causally related operations in the same order.
* causally related:
    * if some processor performs a write operation `A`, and some (the same or another) processor that observed `A` then performs a write operation `B`, then it is possible that `A` is the cause of `B`;

* two requirements:
    * writes that are potentially causally related must be seen by all processes in the same order.
    * concurrent writes may be seen in a different order on different machines.

## references
* https://www.cs.colostate.edu/~cs551/CourseNotes/Consistency/TypesConsistency.html
* https://en.wikipedia.org/wiki/Sequential_consistency
* https://en.wikipedia.org/wiki/Causal_consistency
* https://en.wikipedia.org/wiki/Processor_consistency
