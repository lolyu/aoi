# consistency models
* consistency vs coherence?
    * consistency refers to the uniformity of multiple objects at the same time
    * coherence refers to the uniformity of the same object across time
* what is memory consistency?
    * memory consistency defines shared memory correctness, that is, provides rules about loads and stores and how they act upon memory
    * memory consistency defines rules for the apparent order and visibility of updates
* what is memory coherence?
    * memory coherence seeks to make the caches of a shared-memory system as functionally invisible as the caches in a single-core system

* why do memory consistency and coherence matter?
    * due to the multi-processor environment

![image](https://github.com/lolyu/aoi/assets/35479537/00b98803-e398-49be-85fc-855a53f43015)


## strong consistency models
### strict consistency
* `strict consistency`: a write to a variable by any processor needs to be seen instantaneously by all processors

### sequential consistency
* `sequential consistency`
    * the execution order in the same processor is the same as the program order
    * the execution order of a program on different processors is undefined
* the total order of operations is called `memory order`(or total order), in `sequential consistency`, `memory order` **respects** each core's `program order`.
    * **respects** means that `op1 <p op2` implies `op1 <m op2`
    * `op1 <p op2` means that `op1` precedes `op2` in the processor's program order
    * `op1 <m op2` means that `op1` precedes `op2` in the memory order
* **NOTE**: in order to preserve sequential order of execution between processors, all operations must appear to execute atomically.

## references
* https://course.ece.cmu.edu/~ece847c/S15/lib/exe/fetch.php?media=part2_2_sorin12.pdf
