# VPP FDB Learning — `test_fdb` Dev Notes

**Goal**: Make `tests/fdb/test_fdb.py` pass on the VPP KVM t0 testbed (`vms-kvm-vpp-t0`, DUT: 10.250.0.93, PTF: 10.250.0.108).

---

## Root Cause 1 — FDB aging loop scans wrong bridge domain

**File**: `vslib/vpp/SwitchVpp.cpp` (`processFdbEntriesForAging()`)

The original code iterated `m_objectHash[SAI_OBJECT_TYPE_VLAN]` to find bridge domains to poll. After a syncd restart, `m_objectHash` may not be populated yet, so VLAN 1000 was never found and FDB polling did nothing.

**Fix**: Query VPP directly for all active bridge domain IDs using `vpp_bridge_domain_dump_all()` (bd_id=`~0u` dumps all), bypassing `m_objectHash` entirely.

```cpp
std::vector<uint32_t> bd_ids;
vpp_bridge_domain_dump_all([](uint32_t bd_id, void *ctx) {
    static_cast<std::vector<uint32_t>*>(ctx)->push_back(bd_id);
}, &bd_ids);
```

New API added:
- `vpp_bd_id_cb_fn` typedef + `vpp_bridge_domain_dump_all()` in `SaiVppXlate.c`/`.h`

---

## Root Cause 2 — FDB aging fires before tap interfaces are ready

**File**: `vslib/vpp/SwitchVpp.cpp` (`processFdbEntriesForAging()`)

The FDB aging thread fires every 1 second. On syncd restart, it fires before `processOids()` has finished creating hostifs and populating `m_port_id_to_tapname`. This caused `generateFdbLearnedEvent()` to fail silently for all ports, yet `m_vpp_fdb_entries = current_entries` still executed — swallowing 265 pre-existing VPP L2FIB entries as "known", so no LEARNED events ever fired for them.

**Fix**: Added a tap-readiness guard at the top of `processFdbEntriesForAging()`. Skip the poll until at least one port has a registered tap name:

```cpp
bool taps_ready = false;
for (auto &kv : m_objectHash.at(SAI_OBJECT_TYPE_PORT)) {
    sai_object_id_t pid;
    sai_deserialize_object_id(kv.first, pid);
    std::string dummy;
    if (getTapNameFromPortId(pid, dummy)) { taps_ready = true; break; }
}
if (!taps_ready) return;
```

---

## Root Cause 3 — FDB learning notification not implemented

**File**: `vslib/vpp/SwitchVppFdb.cpp` (`generateFdbLearnedEvent()`)

VPP L2FIB learned MACs but never notified SONiC's orchagent. Implemented the full notification path:

1. `vpp_get_swif_name(sw_if_index)` → `"bobmX"`
2. `hwif_to_tap_name("bobmX")` → `"EthernetY"` (via `sonic_vpp_ifmap.ini`)
3. Iterate `SAI_OBJECT_TYPE_PORT` to find port OID by tap name
4. `findBridgeVlanForPortVlan()` → bridge_port OID + bv_id
5. `create_internal(SAI_OBJECT_TYPE_FDB_ENTRY, ...)` + `send_fdb_event_notification(SAI_FDB_EVENT_LEARNED)`

Also implemented:
- `generateFdbAgedEvent()` — fires `SAI_FDB_EVENT_AGED` when a MAC disappears from VPP L2FIB
- `processFdbEntriesForAging()` — polls VPP L2FIB every second, compares against `m_fdb_info_set`, fires aged/learned events for deltas

**Key data structures**:
- `m_fdb_info_set` — set of currently known FDB entries (in sync with SONiC)
- `m_hwif_hostif_map` — populated once from `sonic_vpp_ifmap.ini`: `bobmX` → `EthernetY`

---

## Root Cause 4 — `setup_po2vlan` creating PortChannel1 on VPP testbed

**Files**: `tests/common/testbed.py`, `tests/common/helpers/portchannel_to_vlan.py`

`TestbedInfo._normalize_topo_names()` strips the `-vpp` suffix from topology names (`t0-vpp` → `t0`) so that VPP testbeds reuse the same topology files as non-VPP. This meant the `setup_po2vlan` skip check (`"t0-vpp" in tbinfo["topo"]["name"]`) never matched — the fixture ran and created PortChannel1, moving Ethernet4/8/12/16 out of Vlan1000.

**Fix in `testbed.py`**: Set `tb["topo"]["is_vpp"] = True` before stripping the suffix:
```python
def _normalize_topo_names(self):
    for tb_name, tb in list(self.testbed_topo.items()):
        topo_name = tb["topo"]["name"]
        if topo_name.endswith("-vpp"):
            tb["topo"]["is_vpp"] = True
            tb["topo"]["name"] = topo_name[:-4]
        else:
            tb["topo"].setdefault("is_vpp", False)
```

**Fix in `portchannel_to_vlan.py`**: Use `is_vpp` flag instead of the stripped name:
```python
if tbinfo.get("topo", {}).get("is_vpp", False) or \
        any(topo in tbinfo["topo"]["name"] for topo in ["dualtor", "t0-backend", "t0-standalone"]):
    yield
    return
```

---

## Files Modified

| File | Change |
|---|---|
| `vslib/vpp/SwitchVpp.cpp` | `processFdbEntriesForAging()`: use `vpp_bridge_domain_dump_all()` + tap-readiness guard |
| `vslib/vpp/vppxlate/SaiVppXlate.c` | Add `vpp_bridge_domain_dump_all()` using bd_id=`~0u` |
| `vslib/vpp/vppxlate/SaiVppXlate.h` | Add `vpp_bd_id_cb_fn` typedef + `vpp_bridge_domain_dump_all` declaration |
| `vslib/vpp/SwitchVppFdb.cpp` | Implement `generateFdbLearnedEvent()`, `generateFdbAgedEvent()`, `processFdbEntriesForAging()` |
| `vslib/vpp/SwitchVppHostif.cpp` | `hwif_to_tap_name()` + `populate_if_mapping()` from `sonic_vpp_ifmap.ini` |
| `tests/common/testbed.py` | `_normalize_topo_names()` sets `is_vpp=True` before stripping suffix |
| `tests/common/helpers/portchannel_to_vlan.py` | `setup_po2vlan` skips for VPP testbeds via `is_vpp` flag |

---

## FDB Event DB Schema

When `generateFdbLearnedEvent()` fires, it writes to two places:

### 1. ASIC_DB (via `create_internal`)

Key: `ASIC_STATE:SAI_OBJECT_TYPE_FDB_ENTRY:{"bvid":"oid:0x26...","mac":"02:11:22:33:44:01","switch_id":"oid:0x21..."}`

Fields:
| Field | Value |
|---|---|
| `SAI_FDB_ENTRY_ATTR_TYPE` | `SAI_FDB_ENTRY_TYPE_DYNAMIC` |
| `SAI_FDB_ENTRY_ATTR_BRIDGE_PORT_ID` | `oid:0x3a...` (bridge port OID) |

The key encodes the **tuple (bv_id, mac, switch_id)**:
- `bv_id` — SAI VLAN OID (e.g. `oid:0x2600000000001a` for Vlan1000)
- `mac` — learned MAC address
- `switch_id` — SAI switch OID

### 2. SONiC Notification channel (via `send_fdb_event_notification`)

Sends `sai_fdb_event_notification_data_t` to the notification consumer, which orchagent translates into:

**APP_DB** `FDB_TABLE:Vlan1000:02:11:22:33:44:01`:
| Field | Value |
|---|---|
| `port` | `Ethernet0` |
| `type` | `dynamic` |

### Object ID resolution chain

```
VPP L2FIB entry:  mac + bd_id=1000 + sw_if_index=5
        ↓
  sw_if_index → hwif_name ("bobm4") via vpp_get_swif_name()
        ↓
  hwif_name → tap_name ("Ethernet4") via m_hwif_hostif_map (sonic_vpp_ifmap.ini)
        ↓
  tap_name → port_id (SAI OID) via m_port_id_to_tapname
        ↓
  findBridgeVlanForPortVlan(port_id, vlan_id=1000)
        ↓
  → bv_id (SAI VLAN OID)  +  bridge_port_id (SAI bridge port OID)
        ↓
  sai_fdb_entry_t { bv_id, mac, switch_id }  +  attrs { TYPE=DYNAMIC, BRIDGE_PORT_ID }
```

---

## Test Result

All 8 `test_fdb` cases pass (301s, testbed `vms-kvm-vpp-t0`):
```
fdb/test_fdb.py::test_fdb[ethernet]                      PASSED
fdb/test_fdb.py::test_fdb[arp_request]                   PASSED
fdb/test_fdb.py::test_fdb[arp_reply]                     PASSED
fdb/test_fdb.py::test_fdb[cleanup]                       PASSED
fdb/test_fdb.py::test_self_mac_not_learnt[ethernet]      PASSED
fdb/test_fdb.py::test_self_mac_not_learnt[arp_request]   PASSED
fdb/test_fdb.py::test_self_mac_not_learnt[arp_reply]     PASSED
fdb/test_fdb.py::test_self_mac_not_learnt[cleanup]       PASSED
================ 8 passed, 186 warnings in 301.95s =================
```

