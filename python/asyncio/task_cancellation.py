import asyncio


async def child_coroutine(index):
    print(f"Start child {index}")
    try:
        await asyncio.sleep(10000)
    except asyncio.CancelledError:
        print(f"Child {index} got cancelled")
        raise
    finally:
        print(f"Stop child {index}")


async def parent_coroutine(children: list):
    print("Start parent")
    try:
        await asyncio.gather(*children)
    except asyncio.CancelledError:
        print("Parent got cancelled")
        raise
    finally:
        print("Stop parent")


# if the task called with cancel() is awaiting another Future object, the Future object is cancelled first
# with asyncio.CancelledError first. Then the cancel error will be propagated to the parent task that awaits
# for this task calling cancel()

# if a parent task calls asyncio.gather with multiple child tasks, if one of the child tasks calls cancel,
# the child task will propagate the asyncio.CancelledError to the parent task,  then the parent task will
# try to cancel all the other unfinished child tasks listed in asyncio.gather.


async def main():
    children = [asyncio.create_task(child_coroutine(_)) for _ in range(3)]
    parent = asyncio.create_task(parent_coroutine(children))
    await asyncio.sleep(0)
    # children[0].cancel()
    parent.cancel()
    await asyncio.sleep(2)


if __name__ == "__main__":
    asyncio.run(main())
