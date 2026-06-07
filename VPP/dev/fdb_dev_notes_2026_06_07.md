# VPP FDB Development Notes — 2026-06-07

## Overview

This document captures the design decisions and implementation comparison for FDB event generation in `saivpp`.

---

## Poll Mode vs Push Mode

### Approach 1: Poll Mode (`vpp_l2fib_table_dump`)

The first implementation used a 1-second polling loop in `processFdbEntriesForAging()`:

```
Every 1s:
  1. Dump all VPP bridge domain IDs via BRIDGE_DOMAIN_DUMP
  2. For each bd_id: dump L2FIB via L2_FIB_TABLE_DUMP → current_entries{VppFdbKey → sw_if_index}
  3. Diff current_entries vs m_vpp_fdb_entries:
       AGED:    in m_vpp_fdb_entries but not in current_entries
       LEARNED: in current_entries but not in m_vpp_fdb_entries
       MOVE:    in both, sw_if_index changed
  4. m_vpp_fdb_entries = current_entries
```

The L2FIB dump entry carries `{mac, bd_id, sw_if_index}` directly — no auxiliary mapping needed.

**Pros:**
- Simple — no VPP event registration required
- Self-healing: a full dump on every cycle corrects any drift

**Cons:**
- 1s learn/age latency (average ~500ms)
- Constant VPP API calls regardless of FDB activity
- CPU overhead proportional to FDB table size

---

### Approach 2: Push Mode (`WANT_L2_MACS_EVENTS2`) — **Final Decision**

VPP exposes `WANT_L2_MACS_EVENTS2` — a push-based MAC event API analogous to Linux kernel netlink `RTM_NEWNEIGH` for the FDB.

#### VPP API

```c
// Register once:
want_l2_macs_events2 {
    max_macs_in_event = 10,  // units of 10 → max 100 MACs per event
    enable_disable = true,
    pid = <our pid>
}

// VPP pushes unsolicited notifications:
l2_macs_event {
    n_macs,
    mac[] = { sw_if_index, mac_addr, action=ADD/DELETE/MOVE }
}
```

Three event types exactly matching SAI:

| VPP action | SAI equivalent |
|---|---|
| `MAC_EVENT_ACTION_ADD` | `SAI_FDB_EVENT_LEARNED` |
| `MAC_EVENT_ACTION_DELETE` | `SAI_FDB_EVENT_AGED` |
| `MAC_EVENT_ACTION_MOVE` | `SAI_FDB_EVENT_MOVE` |

**Note:** `l2_macs_event` carries `sw_if_index` but NOT `bd_id`. VPP knows which bridge domain the interface belongs to, but doesn't include it in the event. An auxiliary map `m_swif_to_bdid` (sw_if_index → bd_id) is required.

#### Implementation

```
On startup:
  1. Seed m_swif_to_bdid from bridge topology (BRIDGE_DOMAIN_DUMP)
  2. vpp_l2fib_set_scan_delay(1) → VPP scans every 10ms
  3. vpp_want_l2_macs_events2(true, cb) → register for push events

Steady state:
  VPP pushes l2_macs_event
  → staticMacEventCb() enqueues to m_mac_event_queue (VPP receive thread)
  → m_fdbAgingWakeEvent.notify() wakes FDB aging thread immediately
  → processFdbEntriesForAging() drains queue under m_apimutex (~10ms latency)
  → generateFdbLearnedOrMoveEvent / generateFdbAgedEvent
```

#### Thread model

The queue acts as the thread boundary:

```
VPP API receive thread (holds VPP_LOCK, NOT m_apimutex)
  → vl_api_l2_macs_event_t_handler
    → staticMacEventCb → onVppMacEvent
      → lock(m_mac_event_queue_mutex)
      → m_mac_event_queue.push(ev)      ← PRODUCER
      → m_fdbAgingWakeEvent.notify()

FDB aging thread (holds m_apimutex)
  → woken by m_fdbAgingWakeEvent
  → processFdbEntriesForAging()
    → swap(m_mac_event_queue, local)    ← CONSUMER
    → generate* events
```

**Pros:**
- ~10ms end-to-end latency
- Zero CPU cost when FDB is stable
- Incremental — VPP only sends changed entries

**Cons:**
- More complex: thread queue, wake event, `m_swif_to_bdid` map
- `m_vpp_fdb_entries` still needed for de-duplication and flush coordination

---

## Decision: Push Mode

Push mode was chosen because:

1. **Latency**: ~10ms vs ~500ms average — matters for STP, EVPN, MCLAG convergence
2. **CPU efficiency**: No continuous L2FIB dumps
3. **VPP alignment**: `WANT_L2_MACS_EVENTS2` is VPP's intended API for this; the poll approach reimplements VPP's internal scan externally

---

## Test Results

`tests/fdb/test_fdb.py` on `vms-kvm-vpp-t0`:

| Run | Result |
|---|---|
| Single run (4 variants) | 4 passed, 0 errors |
| 10-iteration soak (40 total) | 40 passed, 0 errors |

0 ERR syslog entries in all runs.
