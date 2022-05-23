import asyncio
import itertools
import os
import random
import time


async def makeitem(size = 5):
    return os.urandom(size).hex()

async def randsleep(caller=None):
    i = random.randint(0, 5)
    if caller:
        print(f"{caller} sleeping for {i} seconds.")
    await asyncio.sleep(i)

async def produce(name, q):
    n = random.randint(0, 5)
    print(f"Producer {name} loops for {n} times.")
    for _ in itertools.repeat(None, n):
        await randsleep(caller=f"Producer {name}")
        i = await makeitem()
        t = time.perf_counter()
        await q.put((i, t))
        print(f"Producer {name} added <{i}> to queue.")

async def consume(name, q):
    while True:
        await randsleep(caller=f"Consumer {name}")
        i, t = await q.get()
        now = time.perf_counter()
        print(f"Consumer {name} got element <{i}> in {now - t:0.5f} seconds.")
        q.task_done()

async def main(prod_num, con_num):
    q = asyncio.Queue()
    producers = [asyncio.create_task(produce(n, q)) for n in range(prod_num)]
    consumers = [asyncio.create_task(consume(n, q)) for n in range(con_num)]
    # wait for all producers to finish
    await asyncio.gather(*producers)
    # wait for all consumers to process items in the queue
    await q.join()
    for c in consumers:
        c.cancel()


if __name__ == "__main__":
    start = time.perf_counter()
    asyncio.run(main(2, 5))
    print(f"program completed in {time.perf_counter() - start:0.5} seconds.")
