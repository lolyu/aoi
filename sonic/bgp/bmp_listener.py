import ctypes
import socket
import struct
import enum


LOCALHOST = "127.0.0.1"
BGPD_BMP_PORT = 8900


class BMPMessageType(enum.Enum):
    """Represents the type of a BMP message."""
    RouteMonitoring = 0
    StatisticsReport = 1
    PeerDownNotification = 2
    PeerUpNotification = 3
    Initiation = 4
    Termination = 5
    RouteMirroring = 6


class BMPHeader(ctypes.BigEndianStructure):
    """Represents the header of an BMP message."""
    _fields_ = [
        ("version", ctypes.c_uint8),
        ("length", ctypes.c_uint32),
        ("type", ctypes.c_uint8),
    ]
    _pack_ = 1

class BMPPeerHeader(ctypes.BigEndianStructure):
    """Represents the header of an BMP per-peer message."""
    _fields_ = [
        ("peer_type", ctypes.c_uint8),
        ("peer_flags", ctypes.c_uint8),
        ("peer_distinguisher", ctypes.c_uint64),
        ("peer_address", ctypes.c_uint8 * 16),
        ("peer_as", ctypes.c_uint32),
        ("peer_bgp_id", ctypes.c_uint32),
        ("timestamp_sec", ctypes.c_uint32),
        ("timestamp_msec", ctypes.c_uint32),
    ]
    _pack_ = 1


def setup_bmp_socket():
    """Sets up a connection to BMP."""
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
    sock.connect((LOCALHOST, BGPD_BMP_PORT))
    return sock


def handle_bgp_update(packet_data):
    """Handles a BGP update."""
    peer_header_data = packet_data[:ctypes.sizeof(BMPPeerHeader)]
    bmp_peer_header = BMPPeerHeader.from_buffer_copy(peer_header_data)
    print(
        bmp_peer_header.peer_type,
        bmp_peer_header.peer_flags,
        bmp_peer_header.peer_distinguisher,
        bmp_peer_header.peer_address,
        bmp_peer_header.peer_as,
        bmp_peer_header.peer_bgp_id,
        bmp_peer_header.timestamp_sec,
        bmp_peer_header.timestamp_msec
    )

    # print("Peer header:", peer_header.peer_type, peer_header.peer_flags, peer_header.peer_distinguisher, peer_header.peer_address, peer_header.peer_as, peer_header.peer_bgp_id)
    # packet_data = packet_data[BMPPeerHeader.size:]
    # print("Packet data:", packet_data)


data = b'\x00@\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\n\x00\x00=\x00\x00\xfcXd\x01\x00\x1fep\x17\x8d\x00\x00\\\x96\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\x00=\x02\x00\x00\x00%@\x01\x01\x00P\x02\x00\x16\x02\x05\x00\x00\xfeL\x00\x00\xfcX\x00\x00\xff\xfe\x00\x00\x1a\n\x00\x00\x1a\x0b@\x03\x04\n\x00\x00=\x00'

if __name__ == "__main__":
    bmp_sock = setup_bmp_socket()
    pkt_count = 1
    while True:        
        pkt_header_data = bmp_sock.recv(ctypes.sizeof(BMPHeader))
        bmp_header = BMPHeader.from_buffer_copy(pkt_header_data)
        print("%dth packet:" % pkt_count, bmp_header.version, bmp_header.length, bmp_header.type)
        bmp_data_len = bmp_header.length - ctypes.sizeof(BMPHeader)
        pkt_content = bmp_sock.recv(bmp_data_len)

        if bmp_header.type == BMPMessageType.RouteMonitoring.value:
            handle_bgp_update(pkt_content)
        pkt_count += 1
