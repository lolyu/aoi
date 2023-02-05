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

* `real` - (`user` + `sys`) is the amount of time spent waiting for I/O 

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

## line_profiler to get per-line time usage
* how to improve?
    1. start a hypothesis that was easy to test
    2. change the code so that only the hypothesis could be tested
    3. gather the evidence to support the conclusion

### demo profiling
* the original code
```python
def calculate_z_serial_purepython(maxiter, zs, cs):
    """Calculate output list using Julia update rule"""
    output = [0] * len(zs)
    for i in range(len(zs)):
        n = 0
        z = zs[i]
        c = cs[i]
        while abs(z) < 2 and n < maxiter:
            z = z * z + c
            n += 1
        output[i] = n
    return output
```
* profiling output with `line_profiler`
```
$ kernprof -l -v julia_set.py
```
```
Total time: 40.5077 s
File: julia_set.py
Function: calculate_z_serial_purepython at line 67

Line #      Hits         Time  Per Hit   % Time  Line Contents
==============================================================
    67                                           @profile
    68                                           def calculate_z_serial_purepython(maxiter, zs, cs):
    69                                               """Calculate output list using Julia update rule"""
    70         1       1108.0   1108.0      0.0      output = [0] * len(zs)
    71   1000001     313205.0      0.3      0.8      for i in range(len(zs)):
    72   1000000     295904.0      0.3      0.7          n = 0
    73   1000000     338161.0      0.3      0.8          z = zs[i]
    74   1000000     306916.0      0.3      0.8          c = cs[i]
    75  34219980   15143836.0      0.4     37.4          while abs(z) < 2 and n < maxiter:
    76  33219980   12825233.0      0.4     31.7              z = z * z + c
    77  33219980   10934614.0      0.3     27.0              n += 1
    78   1000000     348714.0      0.3      0.9          output[i] = n
    79         1          1.0      1.0      0.0      return output
```
* updated version
```python
@profile
def calculate_z_serial_purepython(maxiter, zs, cs):
    """Calculate output list using Julia update rule"""
    output = [0] * len(zs)
    for i in range(len(zs)):
        n = 0
        z = zs[i]
        c = cs[i]
        while n < maxiter and abs(z) < 2:
            z = z * z + c
            n += 1
        output[i] = n
    return output
```
* profile output
```
Total time: 40.2861 s
File: julia_set.py
Function: calculate_z_serial_purepython at line 67

Line #      Hits         Time  Per Hit   % Time  Line Contents
==============================================================
    67                                           @profile
    68                                           def calculate_z_serial_purepython(maxiter, zs, cs):
    69                                               """Calculate output list using Julia update rule"""
    70         1       1562.0   1562.0      0.0      output = [0] * len(zs)
    71   1000001     309564.0      0.3      0.8      for i in range(len(zs)):
    72   1000000     295289.0      0.3      0.7          n = 0
    73   1000000     334389.0      0.3      0.8          z = zs[i]
    74   1000000     305345.0      0.3      0.8          c = cs[i]
    75  34219980   15341014.0      0.4     38.1          while n < maxiter and abs(z) < 2:
    76  33219980   12500265.0      0.4     31.0              z = z * z + c
    77  33219980   10873508.0      0.3     27.0              n += 1
    78                                           
    79   1000000     325133.0      0.3      0.8          output[i] = n
    80         1          1.0      1.0      0.0      return output
```
* conclusion: No performance difference to change the order of the logic result.
* In python2.\*, there might be some performance speedup, but in python3.\*, no much difference


## memory_profiler to get per-line memory usage
* `memory_profiler` runs much slower than `line_profiler`
* **NOTE**: to use `memory_profiler` or `line_profiler` to get per-function usage, please use `@profile` decorator over target functions.

![image](https://user-images.githubusercontent.com/35479537/216768321-2134c555-0a98-4f4e-abf2-0fe9b0d8f096.png)
* the yellow lines mark the time range that `calc_pure_python` function runs
* the blue lines mark the time range that `calc_z_serial_purepython` function runs
* those markers help to understand the memory change within function calls
```
$ mprof run julia_set.py                
mprof: Sampling memory every 0.1s
running new process
running as a Python program...
Length of x: 1000
Total elements: 1000000
calculate_z_serial_purepython took 5.514853477478027 seconds
$ mprof plot --output output.jpg
Using last profile data.
```

### demo profiling
* code
```python
import time


@profile
def f(num, count):
    a = [num] * count
    time.sleep(2)
    g(a)


@profile
def g(a):
    time.sleep(4)
    return [_+i for i,_ in enumerate(a)]


if __name__ == "__main__":
    f(-2, 10000)

```
```
$ mprof run demo.py
$ mprof plot --output output.jpg
```
* output

![image](https://user-images.githubusercontent.com/35479537/216769333-04dcbf22-5436-4080-9fcd-21ff2b364a73.png)

### set breakpoints depending on the memory used
```
$ python -m memory_profiler --pdb-mmem=100 my_script.py
```
* `my_script.py` will step into the pdb debugger as soon as the code uses more than 100MB in the decorated function

## bytecode
```
>>> dis.dis(calculate_z_serial_purepython)
 74           0 LOAD_CONST               1 (0)
              2 BUILD_LIST               1
              4 LOAD_GLOBAL              0 (len)
              6 LOAD_FAST                1 (zs)
              8 CALL_FUNCTION            1
             10 BINARY_MULTIPLY
             12 STORE_FAST               3 (output)

 75          14 SETUP_LOOP              94 (to 110)
             16 LOAD_GLOBAL              1 (range)
             18 LOAD_GLOBAL              0 (len)
             20 LOAD_FAST                1 (zs)
             22 CALL_FUNCTION            1
             24 CALL_FUNCTION            1
             26 GET_ITER
        >>   28 FOR_ITER                78 (to 108)
             30 STORE_FAST               4 (i)

 76          32 LOAD_CONST               1 (0)
             34 STORE_FAST               5 (n)

 77          36 LOAD_FAST                1 (zs)
             38 LOAD_FAST                4 (i)
             40 BINARY_SUBSCR
             42 STORE_FAST               6 (z)

 78          44 LOAD_FAST                2 (cs)
             46 LOAD_FAST                4 (i)
             48 BINARY_SUBSCR
             50 STORE_FAST               7 (c)

 79          52 SETUP_LOOP              44 (to 98)
        >>   54 LOAD_FAST                5 (n)
             56 LOAD_FAST                0 (maxiter)
             58 COMPARE_OP               0 (<)
             60 POP_JUMP_IF_FALSE       96
             62 LOAD_GLOBAL              2 (abs)
             64 LOAD_FAST                6 (z)
             66 CALL_FUNCTION            1
             68 LOAD_CONST               2 (2)
             70 COMPARE_OP               0 (<)
             72 POP_JUMP_IF_FALSE       96

 80          74 LOAD_FAST                6 (z)
             76 LOAD_FAST                6 (z)
             78 BINARY_MULTIPLY
             80 LOAD_FAST                7 (c)
             82 BINARY_ADD
             84 STORE_FAST               6 (z)

 81          86 LOAD_FAST                5 (n)
             88 LOAD_CONST               3 (1)
             90 INPLACE_ADD
             92 STORE_FAST               5 (n)
             94 JUMP_ABSOLUTE           54
        >>   96 POP_BLOCK

 83     >>   98 LOAD_FAST                5 (n)
            100 LOAD_FAST                3 (output)
            102 LOAD_FAST                4 (i)
            104 STORE_SUBSCR
            106 JUMP_ABSOLUTE           28
        >>  108 POP_BLOCK

 84     >>  110 LOAD_FAST                3 (output)
            112 RETURN_VALUE
```
* data explaned:
    * first column: line number
    * second column:
        * `>>`: jump points
    * thrid column: operation address
    * fourth column: operation name
    * fifth column: operation parameter
    * sixth column: annotation
* more lines of bytecode will execute much more slowly than fewer equivalent lines of bytecode that use built-in functions


## references
* https://docs.python.org/3.7/library/timeit.html
* https://docs.python.org/3/library/profile.html
* https://github.com/pythonprofilers/memory_profiler
* https://github.com/pyutils/line_profiler
