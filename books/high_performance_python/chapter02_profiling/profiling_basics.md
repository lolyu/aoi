# profiling to find bottlenecks

## native decorator
```python
def timefn(fn):

    @wraps(fn)
    def measure_time(*args, **kwargs):
        t1 = time.time()
        result = fn(*args, **kwargs)
        t2 = time.time()
        print(f"@timefn: {fn.__name__} took {t2 - t1} seconds")
        return result

    return measure_time
```

## timeit
* **NOTE**: `timeit` temporarily disables garbage collection during timing

```
>>> import timeit
>>> t = timeit.Timer(stmt='"-".join(str(n) for n in range(100))')
>>> t.repeat()
[17.797464383766055, 17.803440203890204, 17.844047939404845]
>>> t.timeit()
17.742329083383083
```

### cmdline
```
$ python3 -m timeit '"-".join(str(n) for n in range(100))'
100000 loops, best of 3: 17.9 usec per loop
$ python3 -m timeit '"-".join([str(n) for n in range(100)])'
100000 loops, best of 3: 15 usec per loop
$ python3 -m timeit '"-".join(map(str, range(100)))'
100000 loops, best of 3: 11.4 usec per loop
```

## unix time command
```
$ /usr/bin/time -p python3 julia_set.py 
Length of x: 1000
Total elements: 1000000
@timefn: calculate_z_serial_purepython took 5.62045955657959 seconds
calculate_z_serial_purepython took 5.620551824569702 seconds
300.0
real 6.32
user 6.28
sys 0.04
```
* output:
    * `real`: the wall clock or elapsed time
    * `user`: the amount of CPU time spent on the task outside kernel functions
    * `sys`: the time spent in kernel-level functions

### verbose output explained
```
$ /usr/bin/time -v python3 julia_set.py
Length of x: 1000
Total elements: 1000000
@timefn: calculate_z_serial_purepython took 5.529727458953857 seconds
calculate_z_serial_purepython took 5.529858589172363 seconds
300.0
        Command being timed: "python3 julia_set.py"
        User time (seconds): 6.18
        System time (seconds): 0.06
        Percent of CPU this job got: 99%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:06.24
        Average shared text size (kbytes): 0
        Average unshared data size (kbytes): 0
        Average stack size (kbytes): 0
        Average total size (kbytes): 0
        Maximum resident set size (kbytes): 116800
        Average resident set size (kbytes): 0
        Major (requiring I/O) page faults: 0
        Minor (reclaiming a frame) page faults: 29614
        Voluntary context switches: 1
        Involuntary context switches: 36
        Swaps: 0
        File system inputs: 0
        File system outputs: 144
        Socket messages sent: 0
        Socket messages received: 0
        Signals delivered: 0
        Page size (bytes): 4096
        Exit status: 0
```

## cProfile
```
$ python3 -m cProfile -o profile.stats julia_set.py                                                                                                                                                                   1 ↵
Length of x: 1000
Total elements: 1000000
calculate_z_serial_purepython took 8.715491533279419 seconds
$ python3
Python 3.6.9 (default, Nov 25 2022, 14:10:45) 
[GCC 8.4.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import pstats
>>> p = pstats.Stats("profile.stats")
>>> p.sort_stats()
<pstats.Stats object at 0x7fbb834576a0>
>>> p.print_stats()
Wed Feb  1 13:27:54 2023    profile.stats

         36237407 function calls (36236997 primitive calls) in 9.322 seconds

   Random listing order was used

   ncalls  tottime  percall  cumtime  percall filename:lineno(function)
        3    0.000    0.000    0.000    0.000 {method 'setter' of 'property' objects}
       32    0.000    0.000    0.000    0.000 {method '__contains__' of 'frozenset' objects}
      265    0.000    0.000    0.000    0.000 {method 'add' of 'set' objects}
        7    0.000    0.000    0.000    0.000 {method 'remove' of 'set' objects}
        1    0.000    0.000    0.000    0.000 {method 'union' of 'set' objects}
      134    0.000    0.000    0.000    0.000 {method 'get' of 'dict' objects}
        3    0.000    0.000    0.000    0.000 {method 'setdefault' of 'dict' objects}
>>> p.print_callers()
   Random listing order was used

Function                                                                                   was called by...
                                                                                               ncalls  tottime  cumtime
{method 'setter' of 'property' objects}                                                    <-       1    0.000    0.000  /usr/lib/python3.6/logging/__init__.py:1590(LoggerAdapter)
                                                                                                    2    0.000    0.000  /usr/lib/python3.6/threading.py:738(Thread)
{method '__contains__' of 'frozenset' objects}                                             <-      32    0.000    0.000  /usr/lib/python3.6/collections/__init__.py:357(namedtuple)
{method 'add' of 'set' objects}                                                            <-       3    0.000    0.000  /usr/lib/python3.6/_weakrefset.py:20(__enter__)
                                                                                                    8    0.000    0.000  /usr/lib/python3.6/_weakrefset.py:81(add)
                                                                                                   56    0.000    0.000  /usr/lib/python3.6/abc.py:132(__new__)
                                                                                                   26    0.000    0.000  /usr/lib/python3.6/collections/__init__.py:357(namedtuple)
                                                                                                  100    0.000    0.000  /usr/lib/python3.6/tokenize.py:21(<module>)
                                                                                                   72    0.000    0.000  /usr/lib/python3.6/tokenize.py:137(_all_string_prefixes)
>>> p.print_callees()
   Random listing order was used

Function                                                                                   called...
                                                                                               ncalls  tottime  cumtime
{method 'setter' of 'property' objects}                                                    -> 
{method '__contains__' of 'frozenset' objects}                                             -> 
{method 'add' of 'set' objects}                                                            -> 
{method 'remove' of 'set' objects}                                                         -> 
{method 'union' of 'set' objects}                                                          -> 
{method 'get' of 'dict' objects}                                                           -> 
{method 'setdefault' of 'dict' objects}                                                    -> 
{method 'items' of 'dict' objects}                                                         -> 
{method 'values' of 'dict' objects}                                                        -> 
{method 'update' of 'dict' objects}                                                        -> 
{method 'bit_length' of 'int' objects}                                                     -> 
{built-in method from_bytes}                                                               -> 
{method 'append' of 'list' objects}                                                        -> 
{method 'extend' of 'list' objects}                                                        ->      16    0.000    0.000  <frozen importlib._bootstrap_external>:1202(<genexpr>)
{method 'sort' of 'list' objects}                                                          ->      13    0.000    0.000  /usr/lib/python3.6/enum.py:868(<lambda>)
```



## references
* https://docs.python.org/3.7/library/timeit.html
* https://docs.python.org/3/library/profile.html
