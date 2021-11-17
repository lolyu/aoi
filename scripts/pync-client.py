"""netcat client."""
import argparse
import fcntl
import itertools
import os
import select
import socket
import sys

from six.moves import queue


message_queues = queue.Queue()


def client_connect(address, port):
    """Connect to remote server."""
    global client
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((address, port))
    client.setblocking(False)


def set_nonblocking(fd):
    """Set fd non-blocking."""
    flag = fcntl.fcntl(fd, fcntl.F_GETFL)
    fcntl.fcntl(fd, fcntl.F_SETFL, flag | os.O_NONBLOCK)


def main_loop(addr, port):
    """Main loop."""
    global client
    client_connect(addr, port)
    inputs = [sys.stdin, client]
    outputs = []
    while inputs:
        set_nonblocking(sys.stdin.fileno())
        set_nonblocking(sys.stdout.fileno())
        readable, writable, exceptional = select.select(inputs, outputs, [])
        if not outputs:
            outputs = [client] if sys.stdin in readable else [sys.stdout]
        for r in readable:
            msg = os.read(r.fileno(), 4096)
            if msg:
                message_queues.put(msg)
            else:
                sys.exit(0)
        for w in writable:
            try:
                msg = message_queues.get_nowait()
            except queue.Empty:
                pass
            else:
                os.write(w.fileno(), msg)
        for e in exceptional:
            try:
                inputs.remove(e)
                outputs.remove(e)
            except ValueError:
                pass


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Python netcat-like utility client"
        )
    parser.add_argument("-a", "--addr", metavar="address",
                        type=str, help="destination address.")
    parser.add_argument("-p", "--port", metavar="port", type=int,
                        help="destination port")
    args = parser.parse_args()
    main_loop(args.addr, int(args.port))
