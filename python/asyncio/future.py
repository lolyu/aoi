import asyncio


async def set_after(future, delay, value):
    await asyncio.sleep(delay)
    if not future.cancelled():
        future.set_result(value)


async def main():
    loop = asyncio.get_running_loop()
    future_obj = loop.create_future()
    loop.create_task(set_after(future_obj, 1, "...world"))
    print("hello...")
    print(await future_obj)


if __name__ == "__main__":
    asyncio.run(main())
