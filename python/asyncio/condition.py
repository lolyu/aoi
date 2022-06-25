import asyncio


# async with statement: https://docs.python.org/3/reference/compound_stmts.html#async-with
# an asynchronous context manager is a context manager that is able to suspend execution at its enter and exit methods


async def waiter(index, condition):
    print(f"start waiter {index}")
    async with condition:
        await condition.wait()
        print(f"waiter {index} is awaken!")
    print(f"waiter {index} says goodbye!")


async def main():
    condition = asyncio.Condition()
    tasks = set(asyncio.create_task(waiter(i, condition)) for i in range(5))
    await asyncio.sleep(0)
    while tasks:
        async with condition:
            print("notify waiters")
            condition.notify()
        await asyncio.sleep(0)

        task = None
        for task in tasks:
            if task.done():
                break
        if task:
            tasks.remove(task)
    print("main done!")


if __name__ == "__main__":
    asyncio.run(main())
