import asyncio
import time

# shield protects the wrapped coroutine from cancellation
# https://stackoverflow.com/questions/52505794/python-asyncio-how-to-wait-for-a-cancelled-shielded-task


async def coro():
    print("Starting")
    await asyncio.sleep(2)
    print("Done sleeping")


async def cancel_it(some_task):
    await asyncio.sleep(0.5)
    some_task.cancel()
    print("Cancellation effected")


async def main():
    loop = asyncio.get_event_loop()
    task0 = loop.create_task(coro())
    to_be_cancelled = cancel_it(asyncio.shield(task0))
    task1 = loop.create_task(to_be_cancelled)
    await task0

    print(task0.cancelled())


if __name__ == "__main__":
    asyncio.run(main())
