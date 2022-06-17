import asyncio


async def say_after(delay, what):
    await asyncio.sleep(delay)
    print(what)
    return delay


async def raise_exception(delay):
    await asyncio.sleep(delay)
    raise RuntimeError("Ooops!!!")


async def main():
    todos = [say_after(i, f"hello No.{i}") for i in range(5)]
    todos.append(raise_exception(1))
    batch = asyncio.gather(*todos)
    try:
        results = await batch
    except Exception as e:
        print(f"Got exception {repr(e)}")
        # batch.cancel()
    else:
        print(results)
    await batch

if __name__ == "__main__":
    asyncio.run(main())
