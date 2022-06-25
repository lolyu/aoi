import asyncio


async def eternity():
    await asyncio.sleep(36000)
    print("Done sleeping")


async def main():
    try:
        await asyncio.wait_for(eternity(), timeout=1.0)
    except asyncio.TimeoutError:
        print("Timeout")


if __name__ == "__main__":
    asyncio.run(main())
