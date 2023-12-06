"""Listens for messages from FPM."""
#
# Steps to use:
# 1. kill fpmsyncd in the bgp container
# 2. run the script with `python fpm_listener.py | ip monitor` in the bgp container
import ctypes
import socket
import struct


LOCALHOST = "127.0.0.1"
FPM_PORT = 2620


class FPMHeader(ctypes.Structure):
    """Represents the header of an FPM message."""
    HeaderFormat = "<BBH"
    _fields_ = [
        ("version", ctypes.c_uint8),
        ("type", ctypes.c_uint8),
        ("length", ctypes.c_uint16),
    ]


def setup_fpm_server_socket():
    """Sets up a connection to FPM."""
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
    sock.bind((LOCALHOST, FPM_PORT))
    sock.listen()
    return sock


def handle_fpm_connection(sock):
    """Handles a connection to FPM."""
    fpm_header = FPMHeader()
    header_size = struct.calcsize(fpm_header.HeaderFormat)
    data = sock.recv(header_size)
    fpm_header.version, fpm_header.type, fpm_header.length = struct.unpack(fpm_header.HeaderFormat, data)

    if fpm_header.version != 1:
        print("Unsupported FPM message version %s" % fpm_header.version)
    if fpm_header.type != 1:
        print("Unsupported FPM message type %s" % fpm_header.type)
    fpm_data_len = fpm_header.length - header_size
    print(sock.recv(fpm_data_len))


if __name__ == "__main__":
    fpm_server_socket = setup_fpm_server_socket()
    while True:
        fpm_conn_socket = fpm_server_socket.accept()[0]    
        handle_fpm_connection(fpm_conn_socket)
