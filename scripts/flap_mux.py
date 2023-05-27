import time

from swsscommon import swsscommon


db = swsscommon.DBConnector("APPL_DB", 0, True)
ps = swsscommon.ProducerStateTable(db, swsscommon.APP_MUX_CABLE_TABLE_NAME)
active_fvs = swsscommon.FieldValuePairs([('state', 'active')])
standby_fvs = swsscommon.FieldValuePairs([('state', 'standby')])

while True:
    print("Flapping Ethernet4 to active")
    ps.set("Ethernet4", active_fvs)
    time.sleep(5)
    print("Flapping Ethernet4 to standby")
    ps.set("Ethernet4", standby_fvs)
    time.sleep(5)
