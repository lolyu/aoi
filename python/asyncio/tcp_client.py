import asyncio


async def tcp_echo_client(message):
    reader, writer = await asyncio.open_connection("127.0.0.1", 8900)
    print(f"Send: {message}")
    writer.write(message.encode())
    # await writer.drain()

    print("Wait for data")
    data = await reader.read(100)
    print(f"Received: {data}")

    writer.close()
    await writer.wait_closed()


if __name__ == "__main__":
    asyncio.run(tcp_echo_client("ALOHA"))
