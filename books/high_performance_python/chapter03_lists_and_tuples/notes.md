# lists and tuples
* lists and tuples is implemented as a consecutive memory block storing an array of pointers that points to the actual data.
* so lists and tupels are heterogeneous
* the only difference between list and tuple is that list is dynamic while tuple is static

## lists versus tuples
* main differences:
    * lists are dynamic arrays, mutable and allow for resizing
    * tuples are static arrays, immutabble, don't allow for resizing, the data within then cannot be changed after they have been created
    * tuples are cached by the Python runtime, which means that we don't need to talk to the kernel to reserve memory every time we want to use one

## list

* list resize equation in Python3.7
```
M = N + (N >> 3) + (3 if N < 9 else 6)
```
```
>>> a = []
>>> sys.getsizeof(a)
64
>>> for i in range(100):
...     a.append(0)
...     print(sys.getsizeof(a), end=' ')
...
96 96 96 96 128 128 128 128 192 192 192 192 192 192 192 192 264 264 264 264 264 264 264 264 264 344 344 344 344 344 344 344 344 344 344 432 432 432 432 432 432 432 432 432 432 432 528 528 528 528 528 528 528 528 528 528 528 528 640 640 640 640 640 640 640 640 640 640 640 640 640 640 768 768 768 768 768 768 768 768 768 768 768 768 768 768 768 768 912 912 912 912 912 912 912 912 912 912 912 912
```
* a list of size 0 uses 64 bytes memory
* a list of size 1 uses 96 bytes memory, allocated extra 32 bytes = 8 * 4, 8 is the pointer size of 64-bits machine, 4 is calculated out of the resize equation: 4 = 1 + 0 + 3

## tuple
* tuples consumes less memory compared to lists of the same size.
```
>>> b = tuple()
>>> sys.getsizeof(b)
48
```
* for tuples of size 1 ~ 20, when they are no longer used, the spaced isn't immediately freed by the Python garbage collector: up to 20000 of each size are saved for future use.
* this means when a new tuple of size 1 ~ 20 is needed, the Python interpreter doesn't need to allocate new memory block for it.
* this caching enables the tuple creation much faster than list creation

```
>>> def f():
...     import time
...     s = time.time()
...     for i in range(10000):
...             l = [1, 2, 3, 4, 5, 6, 7, 8]
...     print(time.time() - s)
...     s = time.time()
...     for i in range(10000):
...             l = (1, 2, 3, 4, 5, 6, 7, 8)
...     print(time.time() - s)
...
>>>
>>> f()
0.0008590221405029297
0.00021147727966308594
```

## references
