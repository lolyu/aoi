# lua scripting in redis
* `eval`
    * first argument: Lua 5.1 script, should not contains function
    * second argument: number of key names
        * `KEYS[1]`, `KEYS[2]`, `KEYS[3]`
    * additional arguments:
        * within the number of key names are key names
        * others could be accessed with:
            * `ARGV[1]`, `ARGV[2]`, `ARGV[3]`
    * all Redis commands must be analyzed before execution to determine which keys the command will operate on.
        * so keys used must be passed explicitly.
```
127.0.0.1:6379> eval 'return {KEYS[1], KEYS[2], KEYS[3], ARGV[1], ARGV[2], ARGV[3]}' 3 1 2 3 1 2 3
1) "1"
2) "2"
3) "3"
4) "1"
5) "2"
6) "3"
```
