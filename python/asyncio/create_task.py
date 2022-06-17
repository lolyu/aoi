import asyncio


backgroud_tasks = set()


async def say_after(delay, what):
    await asyncio.sleep(delay)
    print(what)


async def main():
    for i in range(5):
        print(f"Create task No.{i}")
        task = asyncio.create_task(say_after(i, f"hello No.{i}"))
        backgroud_tasks.add(task)

        task.add_done_callback(backgroud_tasks.discard)

    for task in list(backgroud_tasks):
        await task


if __name__ == "__main__":
    # asyncio.run should always be the main entry for the asyncio programs
    asyncio.run(main())
