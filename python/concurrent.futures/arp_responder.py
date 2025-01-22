from concurrent import futures
import struct
import socket
import selectors
import ctypes
import binascii
import json
import argparse
import os.path
from collections import defaultdict
import logging
import scapy.all as scapy
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)


def bpf_stmt(code, jt, jf, k):
    """Format struct `sock_filter`."""
    return struct.pack("HBBI", code, jt, jf, k)


def build_bpfilter(filter):
    """Build BPF filter buffer."""
    return ctypes.create_string_buffer(b"".join(bpf_stmt(*_) for _ in filter))


# As defined in asm/socket.h
SO_ATTACH_FILTER = 26

# BPF filter "arp"
arp_bpf_filter = [
    [0x28, 0, 0, 0x0000000c],
    [0x15, 0, 1, 0x00000806],
    [0x6, 0, 0, 0x00040000],
    [0x6, 0, 0, 0x00000000]
]

import functools
from collections import deque
from scapy.all import Ether, ARP, Dot1Q


class ARPResponderWorker(object):

    RCV_SIZE = 4096
    ARP_OP_REQUEST = 1

    def __init__(self, ip_sets, thread_pool_size):
        self.poller = selectors.EpollSelector()
        self.thread_pool = futures.ThreadPoolExecutor(thread_pool_size)
        self.futures = deque()
        self.ip_sets = ip_sets
        self.sockets, self.fileno_to_intf_mapping = ARPResponderWorker._create_sockets(self.ip_sets.keys())
        self.alive = True

    @staticmethod
    def _create_sockets(interfaces):
        sockets = []
        fileno_to_intf = {}
        for interface in interfaces:
            sock = socket.socket(family=socket.AF_PACKET,
                                type=socket.SOCK_RAW, proto=0)
            sock.setblocking(False)
            bpf_filter = build_bpfilter(arp_bpf_filter)
            fprog = struct.pack("HL", len(arp_bpf_filter),
                                ctypes.addressof(bpf_filter))
            sock.setsockopt(socket.SOL_SOCKET, SO_ATTACH_FILTER, fprog)
            sock.bind((interface, socket.SOCK_RAW))
            sockets.append(sock)
            fileno_to_intf[sock.fileno()] = interface

        return sockets, fileno_to_intf

    def enqueue_request(self, sock):
        fut = self.thread_pool.submit(self.handle_request, sock)
        fut.sock = sock
        self.futures.append(fut)
        # fut.add_done_callback(self.finish_request)

    def reply_to_arp_request(self, sock, packet):

        @functools.lru_cache(maxsize=None)
        def _generate_arp_reply(local_mac, remote_mac, local_ip, remote_ip, vlan_id):
            l2 = scapy.Ether(dst=remote_mac, src=local_mac, type=(0x8100 if vlan_id else 0x0806))
            l3 = scapy.ARP(op=2, hwsrc=local_mac, psrc=local_ip, hwdst=remote_mac, pdst=remote_ip)
            if vlan_id:
                l2 /= scapy.Dot1Q(vlan=vlan_id, type=0x0806)
            return bytes(l2 / l3)

        op_type = packet[ARP].op
        if op_type != ARPResponderWorker.ARP_OP_REQUEST:
            return

        remote_mac, remote_ip, request_ip = packet[ARP].hwsrc, packet[ARP].psrc, packet[ARP].pdst
        interface = self.fileno_to_intf_mapping[sock.fileno()]
        if interface not in self.ip_sets or request_ip not in self.ip_sets[interface]:
            return

        if 'vlan' in self.ip_sets[interface]:
            vlan_list = self.ip_sets[interface]['vlan']
        else:
            vlan_list = [None]

        for vlan_id in vlan_list:
            arp_reply = _generate_arp_reply(self.ip_sets[interface][request_ip], remote_mac, request_ip, remote_ip, vlan_id)
            sock.send(arp_reply)

    def handle_request(self, sock):
        data = sock.recv(ARPResponderWorker.RCV_SIZE)
        self.reply_to_arp_request(sock, Ether(data))

    def finish_request(self, fut):
        if fut.cancelled():
            fut.sock.close()
            return
    
        try:
            fut.result()
            self.poller.register(fut.sock, selectors.EVENT_READ, self.on_raw_socket_readable)
        except Exception:
            fut.sock.close()

    def on_raw_socket_readable(self, sock):
        self.enqueue_request(sock)

    def run(self):
        for sock in self.sockets:
            sock.setblocking(False)
            self.poller.register(sock, selectors.EVENT_READ, self.on_raw_socket_readable)

        while self.alive:
            events = self.poller.select(1.0)
            for key, _ in events:
                callback = key.data
                callback(key.fileobj)

            # cleanup finished requests
            result = futures.wait(self.futures, timeout=0, return_when=futures.FIRST_COMPLETED)
            for fut in result.done:
                self.futures.remove(fut)


def parse_args():
    parser = argparse.ArgumentParser(description='ARP autoresponder')
    parser.add_argument('--conf', '-c', type=str, dest='conf',
                        default='/tmp/from_t1.json', help='path to json file with configuration')
    parser.add_argument('--extended', '-e', action='store_true',
                        dest='extended', default=False, help='enable extended mode')
    args = parser.parse_args()
    return args


def get_config(path):
    if not os.path.exists(path):
        print(("Can't find file %s" % path))
        return

    with open(path) as fp:
        data = json.load(fp)

    # generate ip_sets. every ip address will have it's own uniq mac address
    ip_sets = {}
    for iface, ip_dict in list(data.items()):
        vlan = None
        iface = str(iface)
        if iface.find('@') != -1:
            iface, vlan = iface.split('@')
            vlan_tag = format(int(vlan), 'x')
            vlan_tag = vlan_tag.zfill(4)
        if iface not in ip_sets:
            ip_sets[iface] = defaultdict(list)
        if args.extended:
            for ip, mac in list(ip_dict.items()):
                ip_sets[iface][str(ip)] = binascii.unhexlify(str(mac))
        else:
            for ip in ip_dict:
                ip_sets[iface][str(ip)] = scapy.get_if_hwaddr(iface)
        if vlan is not None:
            ip_sets[iface]['vlan'].append(binascii.unhexlify(vlan_tag))
    return ip_sets


if __name__ == '__main__':
    args = parse_args()

    ip_sets = get_config(args.conf)
    responder = ARPResponderWorker(ip_sets, 8)
    responder.run()
