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
## references
* https://preshing.com/20130823/the-synchronizes-with-relation/
* https://stackoverflow.com/questions/38565650/what-does-release-sequence-mean#:~:text=In%20summary%3A%20the%20release%20sequence,anyway%20due%20to%20being%20atomic).
