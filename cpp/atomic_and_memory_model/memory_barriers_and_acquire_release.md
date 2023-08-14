# memory barriers and acquire release

## memory barriers
* four types of memory barriers:
    * `LoadLoad`
    * `StoreStore`
    * `LoadStore`
    * `StoreLoad`

### `LoadLoad`
* `LoadLoad` prevents reordering loads before the barrier with the loads after the barrier in the program order.
* `LoadLoad` guarantees the loads after the barrier reads more recent changes than loads before the barrier.

```cpp
if (isPublished)
{
    LOADLOAD_BARRIER();
    return value;
}
```

### `StoreStore`
* `StoreStore` prevents reordering stores before the barrier with the stores after the barrier in the program order.
* `StoreStore` guarantees that: if the stores after the barrier are observed -> the stores before the barrier should be observed

```cpp
value = x;
STORESTORE_BARRIER();
isPublished = true;
```

### `LoadStore`
* `LoadStore` prevents reordering loads before the barrier with the stores after the barrier in the program order.


### `StoreLoad`
* `StoreLoad` not only prevents reordering stores before the barrier with the load after the barrier in the program order but also ensures that all stores before the barrier are visible to all processors.
    * `StoreLoad` is stronger than a `StoreStore` followed by a `LoadLoad`

## acquire release semantics

* `acquire semantics`:
    * applies to reads
    * also called `read-acquire` or `load-acquire`
    * prevents memory reordering of `read-acquire` with any read/write operations that follow it in the program order
    * `read-acquire` is a combination of `LoadLoad` and `LoadStore`
![image](https://github.com/lolyu/aoi/assets/35479537/3859cf42-0a62-4caf-a613-138667fedb47)


* `release semantics`:
    * applies to writes
    * also called `write-release` or `store-release`
    * `write-release` is a combination of `LoadStore` and `StoreStore`
    * prevents memory reordering of `write-release` with any read/write operations that precede it in the program order

![image](https://github.com/lolyu/aoi/assets/35479537/8032ba5f-12a6-41bf-b425-a30512e5c773)


## references
* https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/
* https://preshing.com/20120913/acquire-and-release-semantics/
