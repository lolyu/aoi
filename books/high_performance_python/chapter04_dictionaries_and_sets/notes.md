# dictionaries and sets
![image](https://user-images.githubusercontent.com/35479537/219371828-82d0879b-0fab-40ea-aaff-3096ff31f0b7.png)
* python stores the key/value data into an array, and stores only the index into the array in the hash table.
* cpython uses open addressing to solve collision

* the probing
```python
def index_sequence(key, mask=0b111, PERTURB_SHIFT=5):
    perturb = hash(key) 1
    i = perturb & mask
    yield i
    while True:
        perturb >>= PERTURB_SHIFT
        i = (i * 5 + perturb + 1) & mask
        yield i
```

* cpython uses lazy deletion strategy, it doesn't actually remove the entry in the hash list, it just mark it as deleted, so next probing could continue after reaching this slot.

## resizing
*  It can be shown that a table that is no more than two-thirds full will have optimal space savings while still having a good bound on the number of collisions to expect
*  also cpython resize down the size of dict if sufficiently many elements are deleted
*  **NOTE**: resizing happens only during an insert.



## dictionaries and namespace
* for a variable lookup:
    1. looks inside the `locals()` which has entries for all local variables
    2. looks inside the `globals()` dictionary
    3. looks inside the `__builtin__`
* the search in the local scope is the only part of the the variable lookup that doesn't require a dictionary lookup

```
import time

from time import perf_counter

a = 100

def f():
    b = a
    for i in range(1000):
        c = b * b
    return c

def g():
    for i in range(1000):
        c = a * a
    return c


def h():
    for i in range(1000):
        time.perf_counter()


def p():
    for i in range(1000):
        perf_counter()


if __name__ == "__main__":
    start = time.perf_counter()
    for i in range(10000):
        f()
    end = time.perf_counter()
    print(f"f: {end - start}")

    start = time.perf_counter()
    for i in range(10000):
        g()
    end = time.perf_counter()
    print(f"g: {end - start}")

    start = time.perf_counter()
    for i in range(10000):
        h()
    end = time.perf_counter()
    print(f"h: {end - start}")

    start = time.perf_counter()
    for i in range(10000):
        p()
    end = time.perf_counter()
    print(f"p: {end - start}")

    """
    f: 0.30927319498732686
    g: 0.3693258680286817
    h: 0.7664594450034201
    p: 0.5075510949827731
    """
```

## references
* https://github.com/zpoint/CPython-Internals/blob/master/BasicObject/dict/dict.md
