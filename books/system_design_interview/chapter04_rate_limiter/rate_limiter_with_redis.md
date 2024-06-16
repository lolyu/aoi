# Use Redis to implement rate limiters

* algorithms to implement:
    * token bucket
    * leaking bucket
    * fixed window counter
    * sliding window log
    * sliding window counter

* All the rate limiters will be implemented with Lua scripting in Redis, so the operation atomicity is guaranteed.
* Also, as with Redis, the rate limiter is easy to scale out.

## fixed window counter
```lua
-- token bucket with Redis
-- KEYS[1] - request type
-- KEYS[2] - current minute
-- KEYS[3] - threshold

local request_type = KEYS[1]
local current_min = KEYS[2]
local threshold = tonumber(KEYS[3])
local separator = ":"

local key = request_type .. separator .. current_min

local count = redis.call("GET", key)
if count == false then
    redis.call("INCR", key)
    redis.call("EXPIRE", key, 60)
    return 1
else
    count = tonumber(count)
    if count < threshold then
        redis.call("INCR", key)
        return 1
    else
        return 0
    end
end

```

```
$ redis-cli SCRIPT LOAD "$(cat token_bucket.lua)"
"102e5fd512da253a94f2a6da46b0e38ffc02cb23"
$ redis-cli
127.0.0.1:6379> evalsha 102e5fd512da253a94f2a6da46b0e38ffc02cb23 3 DUMMY_TEST 31 2
(integer) 1
127.0.0.1:6379> evalsha 102e5fd512da253a94f2a6da46b0e38ffc02cb23 3 DUMMY_TEST 31 2
(integer) 1
127.0.0.1:6379> evalsha 102e5fd512da253a94f2a6da46b0e38ffc02cb23 3 DUMMY_TEST 31 2
(integer) 0
127.0.0.1:6379> evalsha 102e5fd512da253a94f2a6da46b0e38ffc02cb23 3 DUMMY_TEST 31 2
(integer) 0
```
## leaking bucket
## sliding window log
* this script is inspired from https://engineering.classdojo.com/blog/2015/02/06/rolling-rate-limiter/, but with the following improvements:
    * keep track of only allowed request time in the log cache, so if a user keeps sending requests, as long as the allowed requests are expired, the new requests could be allowed.

```lua
-- token bucket with Redis
-- KEYS[1] - request type
-- KEYS[2] - interval
-- KEYS[2] - threshold


local request_type = KEYS[1]
local interval = tonumber(KEYS[2])
local threshold = tonumber(KEYS[3])
local separator = ":"

local key = "RATE_LIMITER" .. separator .. request_type
local curr_time_t = redis.call("TIME")
local curr_time = tonumber(curr_time_t[1]) + tonumber(curr_time_t[2]) / 1000000
local curr_window_start = curr_time - interval

redis.call("ZREMRANGEBYSCORE", key, 0, curr_window_start)

local count = redis.call("ZCARD", key)
if count < threshold then
    redis.call("ZADD", key, curr_time, curr_time)
    redis.call("EXPIRE", key, interval)
    return 1
else
    return 0
end
```
```
$ redis-cli SCRIPT LOAD "$(cat sliding_window_log.lua)"
"4012c521bfe6205a76e148fad54c61a2f3019477"
$ redis-cli
127.0.0.1:6379> evalsha 4012c521bfe6205a76e148fad54c61a2f3019477 3 DUMMY_TEST 10 3
(integer) 1
127.0.0.1:6379> evalsha 4012c521bfe6205a76e148fad54c61a2f3019477 3 DUMMY_TEST 10 3
(integer) 1
127.0.0.1:6379> evalsha 4012c521bfe6205a76e148fad54c61a2f3019477 3 DUMMY_TEST 10 3
(integer) 1
127.0.0.1:6379> evalsha 4012c521bfe6205a76e148fad54c61a2f3019477 3 DUMMY_TEST 10 3
(integer) 0
127.0.0.1:6379> evalsha 4012c521bfe6205a76e148fad54c61a2f3019477 3 DUMMY_TEST 10 3
(integer) 0
127.0.0.1:6379> zrange RATE_LIMITER:DUMMY_TEST 0 -1                                    // only keeps track of allowed requests
1) "1718524490.01138"
2) "1718524490.7824099"
3) "1718524491.304518"
127.0.0.1:6379> zrange RATE_LIMITER:DUMMY_TEST 0 -1                                    // key is expired after 10 seconds
(empty array)
```
