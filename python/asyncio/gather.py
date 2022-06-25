import asyncio


async def say_after(delay, what):
    print(f"Start {what}")
    try:
        await asyncio.sleep(delay)
    except asyncio.CancelledError:
        print(f"{what} got canceled!!!")
        raise
    print(what)
    return delay


async def raise_exception(delay):
    await asyncio.sleep(delay)
    raise RuntimeError("Ooops!!!")


async def main():
    todos = [say_after(i, f"hello No.{i}") for i in range(5)]
    todos.append(raise_exception(1))
    todos = [asyncio.create_task(_) for _ in todos]
    # even create tasks for those coroutines, they will not be scheduled to run immediately
    # the main will continue to run
    # the main loop only switch between tasks with seeing the await statement
    # await asyncio.sleep()
    print("Start main")
    batch = asyncio.gather(*todos)
    try:
        results = await batch
    except Exception as e:
        print(f"Got exception {repr(e)}")
        # simply cancelling the gather won't cancel any unfinished tasks
        batch.cancel()
        for todo in todos:
            todo.cancel()
    else:
        print(results)
    await asyncio.sleep(5)

if __name__ == "__main__":
    asyncio.run(main())
