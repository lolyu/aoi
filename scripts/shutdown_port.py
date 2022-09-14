import argparse

from sonic_py_common import daemon_base
from swsscommon import swsscommon


def get_vlan_members(config_db, vlan):
    vlan_member_table = swsscommon.Table(config_db, "VLAN_MEMBER")
    table_entries = vlan_member_table.getKeys()
    table_separator = vlan_member_table.getTableNameSeparator()
    ports = []
    for entry in table_entries:
        vlan_name, port = entry.split(table_separator)
        if vlan.lower() == vlan_name.lower():
            ports.append(port)
    return ports


def config_ports_admin_status(config_db, ports, status):
    port_table = swsscommon.Table(config_db, "PORT")
    updated = swsscommon.FieldValuePairs([("admin_status", status)])
    for port in ports:
        port_table.set(port, updated)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="port startup/shutdown utility"
    )
    parser.add_argument(
        "-a",
        "--action",
        required=False,
        default="shutdown",
        choices=["shutdown", "startup"],
        help="action(shutdown/startup)"
    )
    parser.add_argument(
        "-p",
        "--port",
        required=False,
        default=[],
        action="append",
        dest="ports",
        help="the port to shutdown"
    )
    parser.add_argument(
        "-v",
        "--vlan",
        required=False,
        dest="vlan",
        help="vlan name, its port member will be shutdown"
    )

    args = parser.parse_args()
    config_db = daemon_base.db_connect("CONFIG_DB")

    ports = args.ports
    if args.vlan:
        ports.extend(get_vlan_members(config_db, args.vlan))

    if not ports:
        parser.error("No ports provided.")

    action = args.action
    port_status = "up" if action == "startup" else "down"
    config_ports_admin_status(config_db, ports, port_status)
