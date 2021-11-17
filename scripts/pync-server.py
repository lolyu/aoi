"""netcat server."""
import argparse
import fcntl
import os
import select
import socket
import sys

from six.moves import queue


message_queues = queue.Queue()


def server_listen(port):
    """Listen and accept a connection."""
    global server, conn
    addr = (socket.gethostbyname(socket.gethostname()), port)
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(addr)
    server.listen(1)
    conn, addr = server.accept()
    conn.setblocking(False)


def set_nonblocking(fd):
    """Set fd non-blocking."""
    flag = fcntl.fcntl(fd, fcntl.F_GETFL)
    fcntl.fcntl(fd, fcntl.F_SETFL, flag | os.O_NONBLOCK)


def main_loop(port):
    """Main message loop."""
    global conn
    server_listen(port)
    inputs = [conn, sys.stdin]
    outputs = []
    while inputs:
        set_nonblocking(sys.stdin.fileno())
        set_nonblocking(sys.stdout.fileno())
        readable, writable, exceptional = select.select(inputs, outputs, [])
        if not outputs:
            outputs = [conn] if sys.stdin in readable else [sys.stdout]
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
        description="Python netcat-like utility server"
        )
    parser.add_argument("-l", "--listen", nargs=1, help="listen on port.")
    args = parser.parse_args()
    main_loop(int(args.listen[0]))
