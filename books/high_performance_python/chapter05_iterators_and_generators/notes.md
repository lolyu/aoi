# iterators and generators


## list vs generators
```python
# slow version and consumes large memory
divisible_by_three = len([n for n in fibonacci_gen(100_000) if n % 3 == 0])

# use a generator
divisible_by_three = sum(1 for n in fibonacci_gen(100_000) if n % 3 == 0)
```

* Why generators are good?
    * generators could save memory with comparable same exeuction time
    * generators could refact the code into chunks that do simpler things
        * usually in computation, **there are two phases: generating data and transforming data.**
    * generators are lazy-evaluated, only generate data need in one-round.
