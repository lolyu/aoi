import argparse
import ipaddress
import logging
import sys
import subprocess
import time

from io import BytesIO

from scapy.all import *
from scapy.contrib import bgp
from swsssdk import ConfigDBConnector


def is_ip_address(address):
    try:
        ipaddress.ip_address(address)
    except Exception:
        return False
    return True


def dump_scapy_packets_show_output(packets):
    """Dump packet show output to string."""
    _stdout, sys.stdout = sys.stdout, BytesIO()
    try:
        packets.show()
        return sys.stdout.getvalue()
    finally:
        sys.stdout = _stdout


def run_command(cmd, check=True):
    """Run a command."""
    logging.debug("COMMAND: %s", cmd)
    process = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        shell=True,
    )
    out, err = process.communicate()
    logging.debug("COMMAND STDOUT:\n%s\n", out)
    logging.debug("COMMAND STDERR:\n%s\n", err)
    if check:
        if process.returncode != 0:
            raise RuntimeError


class BGPCeaseChecker(object):

    NEIGHBOR_STATUS = {
        
    }

    def __init__(self, neighbors, interfaces, pcap_file="/tmp/bgp.pcap"):
        self.config_db = ConfigDBConnector(use_unix_socket_path=True, namespace=None)
        self.config_db.connect()

        neighbors = [neighbor.lower() for neighbor in neighbors if not is_ip_address(neighbor)]
        self.neighbors = [neighbor.lower() for neighbor in neighbors if is_ip_address(neighbor)]
        self.all_neighbors = []
        bgp_sessions = self.config_db.get_table('BGP_NEIGHBOR')
        for addr, session in bgp_sessions.items():
            self.all_neighbors.append(addr)
            if session.has_key('name') and session['name'].lower() in neighbors:
                self.neighbors.append(addr)
        assert self.neighbors
        assert self.all_neighbors

        self.interfaces = interfaces
        self.sniffer = AsyncSniffer(
            iface=self.interfaces,
            filter="tcp port 179",
            store=True
        )
        self.pcap_file = pcap_file

        self._change_bgp_session_status("up")
        time.sleep(10)

    def _change_bgp_session_status(self, status):
        # for address in self.all_neighbors:
        #     self.config_db.mod_entry('bgp_neighbor', address, {'admin_status': status})
        if status == "up":
            run_command("config bgp startup all")
        elif status == "down":
            run_command("config bgp shutdown all")

    def check_packets(self, captured_packets):
        neighbor_packets = {neighbor: [] for neighbor in self.neighbors}

        logging.debug("Packets captured:\n\n%s\n", dump_scapy_packets_show_output(captured_packets))

        try:
            for packet in captured_packets:
                if not (TCP in packet and 179 in (packet[TCP].sport, packet[TCP].dport)):
                    continue

                if IP in packet:
                    if packet[IP].src in neighbor_packets:
                        neighbor_packets[packet[IP].src].append(packet)

                    if packet[IP].dst in neighbor_packets:
                        neighbor_packets[packet[IP].dst].append(packet)

                if IPv6 in packet:
                    if packet[IPv6].src in neighbor_packets:
                        neighbor_packets[packet[IPv6].src].append(packet)

                    if packet[IPv6].dst in neighbor_packets:
                        neighbor_packets[packet[IPv6].dst].append(packet)

            for neighbor, packets in neighbor_packets.items():
                logging.info("-------- check neighbor %s --------", neighbor)
                packets = PacketList(packets)
                logging.debug("Packets captured for neighbor %s:\n\n%s\n", neighbor, dump_scapy_packets_show_output(packets))

                if not packets:
                    logging.error("No BGP CEASE notification from %s", neighbor)
                    return False

                for packet in packets:
                    if (((IP in packet and packet[IP].dst == neighbor) or (IPv6 in packet and packet[IPv6].dst == neighbor)) and
                        bgp.BGPNotification in packet and
                        packet[bgp.BGPNotification].error_code == 6 and
                        packet[bgp.BGPNotification].error_subcode == 2):
                        logging.info("Got BGP cease notification to neighbor %s", neighbor)
                        break
                else:
                    logging.error("No BGP CEASE notification to %s", neighbor)
                    return False

            return True
        except Exception:
            logging.exception("Got exception in the checker")
            return False

    def start(self):

        logging.info("start BGP cease check")
        loop = 0
        while True:
            logging.info("======================= Loop %d =======================", loop)
            logging.info("start sniffer")
            self.sniffer.start()
            time.sleep(5)

            logging.info("shutdown all BGP neighbors")
            self._change_bgp_session_status("down")
            time.sleep(60)

            logging.info("stop sniffer")
            packets = self.sniffer.stop()
            wrpcap(self.pcap_file, packets, append=False)

            logging.info("start checking packets")
            if not self.check_packets(packets):
                break

            time.sleep(5)
            logging.info("startup all BGP neighbors")
            self._change_bgp_session_status("up")
            time.sleep(60)
            logging.info("")

            loop += 1


def config_logging():
    log_format = "%(asctime)s %(funcName)-20.20s %(levelname)-5.5s #%(lineno)-.4d| %(message)s"
    root = logging.getLogger()
    root.setLevel(logging.DEBUG)
    handler = logging.StreamHandler(sys.stdout)
    handler.setLevel(logging.DEBUG)
    formatter = logging.Formatter(log_format)
    handler.setFormatter(formatter)
    root.addHandler(handler)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="BGP cease packet checker"
    )

    parser.add_argument(
        "-i",
        "--interface",
        required=True,
        default=[],
        action="append",
        dest="interfaces",
        help="the interfaces to capture BGP packets"
    )

    parser.add_argument(
        "-n",
        "--neighbor",
        required=True,
        default=[],
        action="append",
        dest="neighbors",
        help="the BGP neighbor to startup/shutdown"
    )

    parser.add_argument(
        "-f",
        "--file",
        required=False,
        default="/tmp/bgp.pcap",
        dest="pcap_file",
        help="the BGP pcap file"
    )

    args = parser.parse_args()
    interfaces = args.interfaces
    neighbors = args.neighbors
    pcap_file = args.pcap_file

    config_logging()

    checker = BGPCeaseChecker(neighbors, interfaces)
    checker.start()
