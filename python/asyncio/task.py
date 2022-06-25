import asyncio
import sys

# an event loop runs a task at a time, while a task awaits for the completion of a Future, the event loop
# runs other Tasks, callbacks, or performs IO operations.
# asyncio.create_task()
# loop.create_task()
# asyncio.ensure_future()


class ChildCancelled(asyncio.CancelledError):
    pass


async def detect_cancel(task):
    cont = asyncio.get_event_loop().create_future()

    def on_done(_):
        if task.cancelled():
            cont.set_exception(ChildCancelled())
        elif task.exception() is not None:
            cont.set_exception(task.exception())
        else:
            cont.set_result(task.result())
    task.add_done_callback(on_done)
    await cont


async def cancel_me():
    print('cancel_me')
    try:
        await asyncio.sleep(3600)
    finally:
        print('cancel_me(): after sleep')


async def parent(task):
    await asyncio.sleep(.001)
    try:
        await detect_cancel(task)
    except ChildCancelled:
        print("parent(): child is cancelled now")
        raise
    except asyncio.CancelledError:
        print("parent(): I am cancelled")
        raise


async def main():
    loop = asyncio.get_event_loop()
    child_task = loop.create_task(cancel_me())
    parent_task = loop.create_task(parent(child_task))
    await asyncio.sleep(.1)  # give a chance to child to start running
    if sys.argv[1] == 'parent':
        # if parent is cancelling first, the cont will be cancelled before child_task
        # on_done will try to set the exception of the already-finished future object cont
        parent_task.cancel()
    else:
        child_task.cancel()
    await asyncio.sleep(.5)


if __name__ == "__main__":
    asyncio.run(main())
