import asyncio
import time


def start_callback(index: int, loop: asyncio.BaseEventLoop, interval: int):
    print(f"start {index}th callback at {time.monotonic()}")
    loop.call_later(interval, start_callback, index + 1, loop, interval)


async def main():
    loop = asyncio.get_running_loop()
    loop.call_later(0, start_callback, 0, loop, 5)
    while True:
        await asyncio.sleep(100)


if __name__ == "__main__":
    loop = asyncio.get_event_loop()
    loop.create_task(main())
    try:
        loop.run_forever()
    except Exception:
        pass
