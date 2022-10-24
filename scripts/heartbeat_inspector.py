"""Inspector for dualtor heartbeat packets."""
import argparse
import logging
import pdb
import struct
import sys
import time
import tabulate

from scapy.all import AsyncSniffer
from scapy.all import ICMP
from scapy.all import IP
from scapy.all import sniff
from scapy.fields import ByteEnumField
from scapy.fields import IntField
from scapy.fields import LongField
from scapy.fields import ShortField
from scapy.packet import Packet
from scapy.packet import PacketListField
from scapy.packet import bind_layers
from scapy.packet import split_layers


"""
link prober heartbeat:

|Ethernet(14bytes)|IP(20bytes)|ICMP(8bytes)|payload|

payload:
|cookie(4bytes)|version(4bytes)|uuid(8bytes)|seq(8bytes)|tlvs|

Supported TLVs:

1. TlvCommand
|type(1bytes) = 0x1|length(2bytes) = 1|command(1bytes)|

2. TlvSentinel
|type(1bytes) = 0xff|length(2bytes) = 0|

"""


VERBOSITY_TO_LOGGING_LEVEL = {
    0: logging.ERROR,
    1: logging.WARNING,
    2: logging.INFO,
    3: logging.DEBUG
}

ICMP_PAYLOAD_COOKIE = "0x47656d69"
ICMP_PAYLOAD_VERSION = 0x0
port_index = None


def ICMP_HEARTBEAT_FILTER(p):
    return ICMPHeartbeat in p


class ICMPHeartbeat(ICMP):
    name = "ICMPHeartbeat"

    def guess_payload_class(self, payload):
        if payload.startswith(b"Gemi"):
            return HeartbeatPayload
        else:
            return ICMP.guess_payload_class(self, payload)


def guess_next_tlv(pkt, lst, cur, remain):
    if not remain:
        # no more TLV to process
        return None
    return TlvHeader


class HeartbeatPayload(Packet):
    name = "HeartbeatPayload"

    fields_desc = [
        IntField("cookie", None),
        IntField("version", None),
        LongField("uuid", 0),
        LongField("seq", 0),
        PacketListField(
            "TLVs",
            [],
            next_cls_cb=guess_next_tlv
        )
    ]

    def guess_payload_class(self, payload):
        return TlvHeader


class TlvHeader(Packet):
    name = "TlvHeader"

    TLV_COMMAND = 0x01
    TLV_DUMMY = 0xfe
    TLV_SENTINEL = 0xff

    fields_desc = [
        ByteEnumField("type", None, {TLV_COMMAND: "TLV_COMMAND", TLV_DUMMY: "TLV_DUMMY", TLV_SENTINEL: "TLV_SENTINEL"}),
        ShortField("length", None)
    ]

    def guess_payload_class(self, payload):
        if self.type == TlvHeader.TLV_COMMAND:
            return TlvCommand
        elif self.type == TlvHeader.TLV_SENTINEL:
            return TlvSentinel
        else:
            return Packet.guess_payload_class(self, payload)


class TlvCommand(Packet):
    name = "TlvCommand"

    COMMAND_NONE = 0x00
    COMMAND_SWITCH_ACTIVE = 0x01
    COMMAND_MUX_PROBE = 0x02

    fields_desc = [
        ByteEnumField(
            "command",
            None,
            {COMMAND_NONE: "COMMAND_NONE", COMMAND_SWITCH_ACTIVE: "COMMAND_SWITCH_ACTIVE", COMMAND_MUX_PROBE: "COMMAND_MUX_PROBE"}
        )
    ]


class TlvSentinel(Packet):
    name = "TlvSentinel"


def sniff_packet_from_port(port, capture_time=None, capture_count=None):
    kwargs = {"iface": port, "lfilter": ICMP_HEARTBEAT_FILTER}
    if capture_count is not None:
        kwargs["count"] = capture_count

    sniffer = AsyncSniffer(**kwargs)
    sniffer.start()

    if capture_time is not None:
        time.sleep(capture_time)
        sniffer.stop()
    if capture_count is not None:
        sniffer.join()

    return sniffer.results


def sniff_packet_from_file(file, count=None):
    packets = sniff(offline=file, lfilter=ICMP_HEARTBEAT_FILTER)
    if count is None:
        return packets
    else:
        return packets[:min(len(packets), count)]


def inspect_packets(packets):

    def validate_chksum(packet):
        ip_chksum = packet[IP].chksum
        icmp_chksum = packet[ICMPHeartbeat].chksum
        del packet[IP].chksum
        del packet[ICMPHeartbeat].chksum
        packet = packet.__class__(bytes(packet))

        logging.v3("Correct IP checksum %s, ICMP checksum %s", packet[IP].chksum, packet[ICMPHeartbeat].chksum)
        if ip_chksum != packet[IP].chksum:
            return "Incorrect IP checksum", False
        if icmp_chksum != packet[ICMPHeartbeat].chksum:
            return "Incorrect ICMP checksum", False
        return "", True

    def validate_icmp_id(packet):
        logging.v3("ICMP id field: %s", packet[ICMPHeartbeat].id)
        if port_index is not None and packet[ICMPHeartbeat].id != port_index:
            return "Incorrect ICMP id field %s, should be %s" % (packet[ICMPHeartbeat].id, port_index), False
        return "", True

    def validate_icmp_payload_cookie(packet):
        logging.v3("ICMP payload cookie field %s", packet[HeartbeatPayload].cookie)
        if hex(packet[HeartbeatPayload].cookie) != ICMP_PAYLOAD_COOKIE:
            return "Incorrect ICMP payload cookie field %s, should be %s" % (packet[HeartbeatPayload].cookie, ICMP_PAYLOAD_COOKIE), False
        return "", True

    def validate_icmp_payload_version(packet):
        logging.v3("ICMP payload version field %s", packet[HeartbeatPayload].version)
        if packet[HeartbeatPayload].version != ICMP_PAYLOAD_VERSION:
            return "Incorrect ICMP payload version field %s, should be %s" % (packet[HeartbeatPayload].version, ICMP_PAYLOAD_VERSION), False
        return "", True

    def validate_icmp_payload_tlv(packet):
        if packet[HeartbeatPayload].payload:
            return "Not all payload in the ICMP heartbeat could be parsed into TLVs", False

        tlv_list = packet[HeartbeatPayload].TLVs
        if not tlv_list:
            return "No TLV defined in the heartbeat payload, should have at least one TlvSentinel", False
        for i, tlv_header in enumerate(tlv_list):
            logging.v3("%sth TLV: %r", i + 1, tlv_header)

            if tlv_header.type == TlvHeader.TLV_SENTINEL:
                if tlv_header.length != 0:
                    return "TLV sentinel has unzero(%s) length" % packet[TlvHeader].length, False
                if len(tlv_header.payload) != 0:
                    return "TLV sentinel has payload as %r" % packet[TlvHeader].payload, False
                break
            elif tlv_header.type == TlvHeader.TLV_COMMAND:
                if TlvCommand not in tlv_header:
                    return "No TLV command in the packet", False
                if packet[TlvCommand].command not in (TlvCommand.COMMAND_SWITCH_ACTIVE, TlvCommand.COMMAND_MUX_PROBE):
                    return "Unsupported COMMAND type %s" % packet[TlvCommand].command, False
            else:
                return "Unsupported TLV type %s" % (packet[TlvHeader].type), False
        else:
            return "No TlvSentinel defined in the payload", False

        for j in range(i, len(tlv_list)):
            logging.v3("%sth TLV: %r", j + 1, tlv_list[j])

        if i != len(tlv_list) - 1:
            return "TlvSentinel is not the last TLV listed in the payload", False

        return "", True

    def report_check_result(check_item, err, res):
        if res:
            logging.v2("Validate %s: PASS", check_item)
        else:
            err_counters[check_item] += 1
            logging.v2("Validate %s: FAILED(%s)", check_item, err)

    global port_index
    checks = {
        "checksum": validate_chksum,
        "icmp id": validate_icmp_id,
        "icmp payload cookie": validate_icmp_payload_cookie,
        "icmp payload version": validate_icmp_payload_version,
        "icmp payload tlv": validate_icmp_payload_tlv
    }
    err_counters = {
        "checksum": 0,
        "icmp id": 0,
        "icmp payload cookie": 0,
        "icmp payload version": 0,
        "icmp payload tlv": 0
    }
    incorrect_packets = 0

    for index, packet in enumerate(packets):
        logging.v2("")
        logging.v2("Inspect %sth packet: %r" % (index + 1, packet))

        check_result = True
        for check_item, check_func in checks.items():
            err, res = check_func(packet)
            report_check_result(check_item, err, res)
            check_result &= res

        if not check_result:
            incorrect_packets += 1
            logging.v1("Packet %r failed to pass the checks", packet)

    logging.v0("")
    logging.v0("Packet check summary:")
    logging.v0("heartbeat count %s, incorrect heartbeat count %s", len(packets), incorrect_packets)
    logging.v0("\n" + tabulate.tabulate(list(err_counters.items()), headers=["check item", "failed packet count"]))


def parse_args():
    parser = argparse.ArgumentParser(description="link prober heartbeats inspector.")
    parser.add_argument("-p", "--port", help="the port to capture packets from")
    parser.add_argument("-f", "--file", help="pcap file to read packets from")

    parser.add_argument("-c", "--count", type=int, required=False, help="the number of packets to process")
    parser.add_argument("-t", "--time", type=int, required=False, help="time to capture packets from the specified port, in seconds")
    parser.add_argument("-v", "--verbose", action="count", default=0, help="increase verbosity")
    return parser, parser.parse_args()


def config_logging(verbose):
    root = logging.getLogger()
    root.setLevel(logging.DEBUG)

    stdout_handler = logging.StreamHandler(sys.stdout)
    stdout_handler.setLevel(VERBOSITY_TO_LOGGING_LEVEL.get(verbose, logging.DEBUG))
    formatter = logging.Formatter("%(message)s")
    stdout_handler.setFormatter(formatter)
    root.addHandler(stdout_handler)

    # v0: show inspect summary
    # v1: show incorrect packets
    # v2: show all packets
    # v3: show check details
    logging.v0, logging.v1, logging.v2, logging.v3 = logging.error, logging.warn, logging.info, logging.debug


def config_layers(port):
    global port_index
    port_index = int(port.strip("Ethernet"))
    split_layers(IP, ICMP, {'frag': 0, 'proto': 1})
    bind_layers(IP, ICMPHeartbeat, {'frag': 0, 'proto': 1})


if __name__ == "__main__":
    parser, args = parse_args()
    config_logging(args.verbose)
    config_layers(args.port)

    if args.port is None:
        parser.error("Please specify port")

    if args.file is not None:
        packets = sniff_packet_from_file(args.file, args.count)
    else:
        if args.count is None and args.time is None:
            parser.error("Must specify capture time(-t) or capture packet count(-c)")
        if args.count is not None and args.time is not None:
            # prefer capture time here
            args.count = None
        packets = sniff_packet_from_port(args.port, args.time, args.count)

    inspect_packets(packets)
