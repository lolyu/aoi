import argparse

from swsscommon import swsscommon
try:
    from swsssdk import port_util
except ImportError:
    from sonic_py_common import port_util


def parse_args():
    parser = argparse.ArgumentParser(description='set port tx state')
    parser.add_argument('-p', '--port', type=str, required=True, help='port')
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--enable", action="store_true", help="enable port tx")
    group.add_argument("--disable", action="store_true", help="disable port tx")
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()

    print(args)
    port = args.port
    enable = args.enable

    db = swsscommon.SonicV2Connector(host="127.0.0.1")
    port_oid_map = port_util.get_interface_oid_map(db)[0]
    if port not in port_oid_map:
        raise ValueError("Port {} not found".format(port))
    port_oid = port_oid_map[port]
    print("port oid: {}".format(port_oid))
    asic_db = swsscommon.DBConnector("ASIC_DB", 0, True)
    asic_state_table = swsscommon.ProducerTable(asic_db, "ASIC_STATE")
    if enable:
        fvs = swsscommon.FieldValuePairs([('SAI_PORT_ATTR_PKT_TX_ENABLE', 'true')])
    else:
        fvs = swsscommon.FieldValuePairs([('SAI_PORT_ATTR_PKT_TX_ENABLE', 'false')])
    asic_state_table.set("SAI_OBJECT_TYPE_PORT:oid:0x%s" % port_oid, fvs, "set")
