# atomic weapons
* challenges:
    * the code written doesn't execute as the programmer wants
* why? three cause:
    * compiler optimization
    * processor OoO(out of order) execution
        * out-of-order execution: a processor executes instructions in an order governed by the availability of input data and execution units, rather than by their original order in a program.
    * cache coherency

* two key concepts:
    * sequential consistency
    * race condition
![image](https://github.com/lolyu/aoi/assets/35479537/687de69f-ca10-4104-ac66-e326f33b5902)

* transformation: the optimization from the compiler/processor/cache that reorders the source code's loads and stores
    * reorder
    * invent
    * remove


## compiler optimization

## SC-DRF
* sequential consistency for data race free programs
 

## references
* https://en.wikipedia.org/wiki/Out-of-order_execution
