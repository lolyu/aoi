# asyncio

## core concepts
* three awaitable objects
    * coroutine
        * defined with `async def`
    * task
        * when a coroutine is wrapped into a `Task` with functions like `asyncio.create_task`, the coroutine is automatically scheduled to run
    * future
        * low-level awaitable objects that represents an eventual result of an asynchronous operation
        * `loop.run_in_executor`

## APIs
* `asyncio.run(coro, *, debug=False)`
    * runs the passed coroutine, it will manage the asyncio event loop, finalizing asynchronous generators, and close the threadpool.
    * always create a new event loop and closes it at the end.
    * **NOTE**: only one loop could be running in the same thread.
* `asyncio.create_task(coro, *, name=None)`
    * create a `Task` object out of `coro` and schedule its execution.
    * the task is executed in the loop returned by `asyncio.get_running_loop()`
    * it will submits the coroutine to run in the background, concurrently with the current task and all other tasks, switching between them at the `await` points
* `asyncio.sleep(delay, result=None)`
* `asyncio.gather(*aws, return_exceptions=False)`

## transport and protocol
* transport is an abstraction for a socket while protocol is the application using the socket


## references
* https://realpython.com/async-io-python/
* https://docs.python.org/3.10/library/asyncio-task.html
* https://bbc.github.io/cloudfit-public-docs/
* http://www.haifux.org/
* http://allanrbo.blogspot.com/2011/12/raw-sockets-with-bpf-in-python.html