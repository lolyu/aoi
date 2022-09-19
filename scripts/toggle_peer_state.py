import argparse

# python toggle.py -p Ethernet4 -p Ethernet8 -p Ethernet12 -p Ethernet16 -p Ethernet20 -p Ethernet24 -p Ethernet28 -p Ethernet32 -p Ethernet36 -p Ethernet40 -p Ethernet44 -p Ethernet48
# 

from sonic_py_common import daemon_base
from swsscommon import swsscommon


def toggle_port_peer_mux_status(app_db, ports, state):
    peer_mux_table = swsscommon.Table(app_db, "HW_FORWARDING_STATE_PEER")
    updated = swsscommon.FieldValuePairs([("state", state)])
    for port in ports:
        peer_mux_table.set(port, updated)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="toggle peer mux state"
    )
    parser.add_argument(
        "-s",
        "--state",
        required=False,
        default="standby",
        choices=["active", "standby"],
        help="peer mux state"
    )
    parser.add_argument(
        "-p",
        "--port",
        required=False,
        default=[],
        action="append",
        dest="ports",
        help="the port to toggle"
    )

    args = parser.parse_args()
    app_db = daemon_base.db_connect("APPL_DB")

    ports = args.ports
    state = args.state

    if not ports:
        parser.error("No ports provided.")

    toggle_port_peer_mux_status(app_db, ports, state)
