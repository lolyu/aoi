# SONiC VPP IP-in-IP Tunnel — HLD & Gap Analysis

## Background

IP-in-IP (IPINIP) tunnelling encapsulates an IP packet inside another IP header, enabling transport over an overlay network without requiring additional headers (unlike VXLAN or GRE). In SONiC it is used for:

- **Decap-only tunnels** (e.g. VNET over IP forwarding, MUX tunnel for dual-ToR)
- **Encap + decap (bidirectional)** tunnels for traffic forwarding between VNET endpoints
- **SRv6 T.Encaps.L2** (which reuses `SAI_TUNNEL_TYPE_IPINIP` for its outer tunnel object)

The SONiC control plane programs IPINIP via two SAI object types:

| SAI Object | Purpose |
|---|---|
| `SAI_OBJECT_TYPE_TUNNEL` | Defines the encap/decap tunnel (src IP, TTL/DSCP mode, overlay/underlay RIF) |
| `SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY` | Matches inbound outer IP header (dst IP, optional src IP) and maps to a tunnel for decap |

---

## SONiC Control Plane Flow

`TunnelDecapOrch` in `orchagent` handles the `TUNNEL_DECAP_TABLE` from APP_DB and programs the following SAI objects in order.

### Tunnel object creation

```
sai_tunnel_api->create_tunnel(
    SAI_TUNNEL_ATTR_TYPE            = SAI_TUNNEL_TYPE_IPINIP,
    SAI_TUNNEL_ATTR_OVERLAY_INTERFACE = overlay_loopback_rif_oid,
    SAI_TUNNEL_ATTR_UNDERLAY_INTERFACE = underlay_rif_oid,
    SAI_TUNNEL_ATTR_ENCAP_SRC_IP    = <local VTEP IP>   (optional),
    SAI_TUNNEL_ATTR_DECAP_TTL_MODE  = UNIFORM | PIPE,
    SAI_TUNNEL_ATTR_DECAP_DSCP_MODE = UNIFORM | PIPE,
    SAI_TUNNEL_ATTR_DECAP_ECN_MODE  = COPY_FROM_OUTER | STANDARD,
)
→ ASIC_STATE:SAI_OBJECT_TYPE_TUNNEL:oid:0x…
```

### Tunnel termination entry creation

```
sai_tunnel_api->create_tunnel_term_table_entry(
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_VR_ID         = gVirtualRouterId,
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TYPE          = P2P | P2MP | MP2MP,
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_TUNNEL_TYPE   = SAI_TUNNEL_TYPE_IPINIP,
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_DST_IP        = <local decap IP>,
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_SRC_IP        = <remote IP>  (P2P/MP2P only),
    SAI_TUNNEL_TERM_TABLE_ENTRY_ATTR_ACTION_TUNNEL_ID = tunnel_oid,
)
→ ASIC_STATE:SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY:oid:0x…
```

### Encap route (encap direction)

For VNET encap, a next hop with `SAI_NEXT_HOP_TYPE_TUNNEL_ENCAP` pointing at the tunnel object is created, and a route entry points to that next hop:

```
SAI_OBJECT_TYPE_NEXT_HOP (TYPE=TUNNEL_ENCAP, TUNNEL_ID=tunnel_oid, IP=<remote endpoint>)
SAI_OBJECT_TYPE_ROUTE_ENTRY → next_hop_oid
```

---

## VPP Dataplane Counterpart

VPP's `ipip` plugin implements IP{v4,v6}-over-IP{v4,v6} tunnelling (RFC 2473). A tunnel is represented as a virtual interface (`ipip0`, `ipip1`, …).

### VPP API

Defined in `vpp/src/vnet/ipip/ipip.api`:

```c
/* Create an IP-in-IP tunnel */
define ipip_add_tunnel {
    vl_api_ipip_tunnel_t tunnel;  // src, dst, table_id, flags (COPY_DSCP etc.), mode
};
define ipip_add_tunnel_reply {
    i32 retval;
    vl_api_interface_index_t sw_if_index;  // ← the new tunnel interface
};

/* Delete by sw_if_index */
autoreply define ipip_del_tunnel { vl_api_interface_index_t sw_if_index; };
```

### VPP tunnel modes

VPP ipip supports exactly **two** internal modes (`ipip_mode_t`):

| VPP mode | CLI keyword | `tunnel_dst` | Decap match |
|---|---|---|---|
| `IPIP_MODE_P2P` | _(default)_ | specific remote IP | outer src == tunnel_dst **AND** outer dst == tunnel_src |
| `IPIP_MODE_P2MP` | `p2mp` | must be `0.0.0.0` / `::` | outer dst == tunnel_src only (any source accepted) |

> **Constraint (from `ipip.c`):** `if (tmode == TUNNEL_MODE_MP && !ip46_address_is_zero(dst)) return VNET_API_ERROR_INVALID_DST_ADDRESS` — MP mode **requires** dst to be the zero address.

### Equivalent VPP CLI per SAI term table entry type

```
# create ipip tunnel src <addr> dst <addr> [instance <n>] [outer-table-id <ID>] [p2mp]
```

#### P2P — specific remote src + specific local dst

SAI: `DST_IP=10.1.1.1` (my IP), `SRC_IP=10.2.2.2` (remote peer)

```
# Create P2P tunnel: local src=10.1.1.1, remote dst=10.2.2.2
create ipip tunnel src 10.1.1.1 dst 10.2.2.2 outer-table-id 0
set interface state ipip0 up

# Encap: route traffic destined for the remote overlay via the tunnel
ip route add 192.168.10.0/24 via ipip0

# Decap: automatic — VPP decaps packets with outer dst=10.1.1.1 AND outer src=10.2.2.2
```

#### P2MP — specific local dst, any remote src

SAI: `DST_IP=10.1.1.1` (my IP), no `SRC_IP`

```
# Create P2MP tunnel: local src=10.1.1.1, any remote (dst=0.0.0.0 implicit)
create ipip tunnel src 10.1.1.1 p2mp outer-table-id 0
set interface state ipip0 up

# Decap: automatic — VPP decaps any packet with outer dst=10.1.1.1, regardless of source
```

#### MP2P — any local dst, specific remote src

SAI: `SRC_IP=10.2.2.2` (remote peer), no `DST_IP` restriction

VPP has **no direct equivalent** for MP2P (accept-from-one-source, to-any-local-IP). The closest approximation is to create individual P2P tunnels for each local IP that should receive from that remote:

```
# For each local IP that accepts from 10.2.2.2, create a P2P tunnel
create ipip tunnel src 10.1.1.1 dst 10.2.2.2 outer-table-id 0
create ipip tunnel src 10.1.1.2 dst 10.2.2.2 outer-table-id 0
# ...
set interface state ipip0 up
set interface state ipip1 up
```

Alternatively, use a P2MP tunnel per local IP (see P2MP above), accepting that any source can reach it, and rely on upper-layer filtering.

#### MP2MP — src prefix + dst prefix (both masked)

SAI: `SRC_IP=10.2.0.0`, `SRC_IP_MASK=255.255.0.0`, `DST_IP=10.1.0.0`, `DST_IP_MASK=255.255.0.0`

VPP ipip has **no subnet/prefix matching** for decap. The workaround uses:
1. A **dedicated underlay VRF** (e.g. table-id 100) that holds one P2MP tunnel per local IP.
2. A **`classify` table** on the ingress physical interface to match outer src/dst prefix and steer matched packets into that VRF via `action set-ip4-fib-id`.

##### Step 1 — Create one P2MP tunnel per local IP in the destination prefix (in underlay VRF 100)

```
# Each local IP that should accept decap gets its own P2MP tunnel in table 100
create ipip tunnel src 10.1.0.1 p2mp outer-table-id 100
create ipip tunnel src 10.1.0.2 p2mp outer-table-id 100
# ... repeat for every local IP in 10.1.0.0/16 that requires decap
set interface state ipip0 up
set interface state ipip1 up
```

The `ipip` plugin decaps automatically in VRF 100: any packet with outer dst == tunnel src is stripped and the inner packet enters the normal IP4 forwarding path.

##### Step 2 — Create a classify table matching outer src=10.2.0.0/16 + dst=10.1.0.0/16

The classifier is applied at the IPv4 layer (`ip4-table`), so coordinates are byte offsets within the outer IPv4 header:

| IPv4 header field | byte offset | mask bytes |
|---|---|---|
| Source IP (src prefix /16) | 12–13 | `ff ff`, 14–15 → `00 00` |
| Destination IP (dst prefix /16) | 16–17 | `ff ff`, 18–31 → `00 00` |

`skip=0, match=2` → covers 32 bytes (2 × 16-byte vectors) from the start of the outer IPv4 header.

```
# 32-byte mask (hex, no spaces):
#   bytes  0-11: 000000000000000000000000  (don't care: VER/IHL/LEN/ID/FLAGS/TTL/PROTO/CKSUM)
#   bytes 12-13: ffff                       (src /16 mask)
#   bytes 14-15: 0000                       (src host bits, don't care)
#   bytes 16-17: ffff                       (dst /16 mask)
#   bytes 18-31: 000000000000000000000000000000  (dst host bits + rest, don't care)

classify table miss-next 0 \
  mask hex 000000000000000000000000ffff0000ffff0000000000000000000000000000 \
  buckets 8 skip 0 match 2
# → VPP prints: new table index <N>  (assume 0)
```

`miss-next 0` = non-matching traffic continues with normal ip4-lookup (no collateral damage to other traffic).

##### Step 3 — Add a classify session steering matched packets into VRF 100

```
# 32-byte match value (hex):
#   bytes  0-11: 000000000000000000000000  (don't care — ANDed with mask)
#   bytes 12-15: 0a020000                  (src 10.2.0.0, masked to /16)
#   bytes 16-19: 0a010000                  (dst 10.1.0.0, masked to /16)
#   bytes 20-31: 000000000000000000000000  (don't care)

classify session hit-next 0 table-index 0 \
  match hex 0000000000000000000000000a0200000a010000000000000000000000000000 \
  action set-ip4-fib-id 100
# Matching packets are re-looked-up in VRF 100 where the P2MP tunnels live
```

##### Step 4 — Attach the classify table to the ingress underlay interface

```
set interface input acl intfc GigabitEthernet0/0/0 ip4-table 0
```

##### Verify

```
show classify tables verbose
show classify session table 0
show inacl interface GigabitEthernet0/0/0
show ipip tunnel
```

> **Summary**: VPP natively handles P2P and P2MP directly. MP2P and MP2MP require workarounds (tunnel enumeration or classifier-based punt) and are not straightforward to implement in saivpp.

---

## Current saivpp Implementation — Gap Analysis

### What exists

| Feature | Status |
|---|---|
| `SAI_TUNNEL_TYPE_VXLAN` encap + decap | **Implemented** — `TunnelManager` / `vpp_vxlan_tunnel_add_del()` |
| `SAI_OBJECT_TYPE_SRV6_SIDLIST` / `SAI_OBJECT_TYPE_MY_SID_ENTRY` | **Implemented** — `TunnelManagerSRv6` |
| `SAI_TUNNEL_TYPE_IPINIP` encap | **NOT implemented** |
| `SAI_TUNNEL_TYPE_IPINIP` decap (term table entry) | **NOT implemented** |

### Root cause

In `TunnelManager::tunnel_encap_nexthop_action()` ([TunnelManager.cpp](sonic-buildimage/platform/vpp/saivpp/src/TunnelManager.cpp)):

```cpp
if (attr.value.s32 != SAI_TUNNEL_TYPE_VXLAN) {
    SWSS_LOG_ERROR("Unsupported tunnel encap type %d ...");
    return SAI_STATUS_NOT_IMPLEMENTED;   // ← IPINIP hit here
}
```

In `SwitchStateBase::create()` ([SwitchStateBase.cpp](sonic-buildimage/platform/vpp/saivpp/src/SwitchStateBase.cpp)), `SAI_OBJECT_TYPE_TUNNEL` and `SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY` have no special-case handler — they fall through to `create_internal()` which only writes to the Redis object store, with no VPP dataplane programming.

There are also **no `vpp_ipip_*` functions** in `SaiVppXlate.c`/`.h` — the VPP `IPIP_ADD_TUNNEL` binary API is not called anywhere.

### Detailed gap table

| Gap | Where to fix | Details |
|---|---|---|
| **`vpp_ipip_add_del_tunnel()`** | `SaiVppXlate.c` / `.h` | New function wrapping VPP `IPIP_ADD_TUNNEL` / `IPIP_DEL_TUNNEL` binary API. Inputs: src IP, dst IP, table_id, flags (copy DSCP/TTL). Output: `sw_if_index`. |
| **`SAI_OBJECT_TYPE_TUNNEL` create/remove for IPINIP** | `SwitchStateBase.cpp` `create()` dispatch | Add `SAI_OBJECT_TYPE_TUNNEL` case, check `SAI_TUNNEL_ATTR_TYPE == SAI_TUNNEL_TYPE_IPINIP`, call `m_tunnel_mgr.create_ipinip_tunnel()` |
| **`SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY` programming** | New handler in `TunnelManager` | For decap: read `DST_IP` (and optionally `SRC_IP`), call `vpp_ipip_add_del_tunnel()` in P2MP mode (dst-only match) or P2P mode. Set `sw_if_index` → add IP route via tunnel interface. |
| **Encap next hop** | `TunnelManager::tunnel_encap_nexthop_action()` | Add `SAI_TUNNEL_TYPE_IPINIP` branch: call `vpp_ipip_add_del_tunnel()`, set interface IP address (encap src), add FIB route via `ipip` interface. Store `nexthop_oid → sw_if_index` in a new map (analogous to `m_tunnel_encap_nexthop_map`). |
| **TTL / DSCP mode** | `vpp_ipip_add_del_tunnel()` | Map `SAI_TUNNEL_TTL_MODE_UNIFORM_MODEL` → `TUNNEL_ENCAP_DECAP_FLAG_ENCAP_COPY_TTL`, `SAI_TUNNEL_DSCP_MODE_UNIFORM_MODEL` → `TUNNEL_ENCAP_DECAP_FLAG_ENCAP_COPY_DSCP` in `vl_api_tunnel_encap_decap_flags_t`. |
| **VRF (table_id)** | `TunnelManager` | Read `SAI_TUNNEL_ATTR_UNDERLAY_INTERFACE` → get VRF → resolve to VPP `table_id` via `vpp_get_ip_vrf()`. |

---

## Proposed Implementation Sketch

### 1. `SaiVppXlate.h` — new type and function declarations

```c
typedef struct vpp_ipip_tunnel_ {
    vpp_ip_addr_t src;
    vpp_ip_addr_t dst;
    uint32_t      table_id;
    uint8_t       flags;     /* vl_api_tunnel_encap_decap_flags_t bits */
    uint32_t      instance;  /* ~0 = auto */
} vpp_ipip_tunnel_t;

extern int vpp_ipip_add_del_tunnel(vpp_ipip_tunnel_t *tunnel, bool is_add,
                                   uint32_t *sw_if_index);
```

### 2. `SaiVppXlate.c` — implementation

Follow the same M/S/WR pattern as `vpp_vxlan_tunnel_add_del()`:

```c
int vpp_ipip_add_del_tunnel(vpp_ipip_tunnel_t *tunnel, bool is_add, uint32_t *sw_if_index)
{
    vl_api_ipip_add_tunnel_t *mp;
    int ret;
    VPP_LOCK();
    __plugin_msg_base = ipip_msg_id_base;
    if (is_add) {
        M(IPIP_ADD_TUNNEL, mp);
        /* fill mp->tunnel.{src, dst, table_id, flags, instance} */
        mp->tunnel.instance = htonl(~0);
        /* ... */
        S(mp);
        WR(ret);   /* reply carries sw_if_index */
    } else {
        vl_api_ipip_del_tunnel_t *dm;
        M(IPIP_DEL_TUNNEL, dm);
        dm->sw_if_index = htonl(*sw_if_index);
        S(dm);
        WR(ret);
    }
    VPP_UNLOCK();
    return ret;
}
```

### 3. `TunnelManager` — new IPINIP methods

```cpp
sai_status_t create_ipinip_tunnel(
    _In_ const std::string &serializedObjectId,
    _In_ sai_object_id_t switch_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list);

sai_status_t remove_ipinip_tunnel(_In_ const std::string &serializedObjectId);

sai_status_t create_ipinip_term_entry(
    _In_ const std::string &serializedObjectId,
    _In_ sai_object_id_t switch_id,
    _In_ uint32_t attr_count,
    _In_ const sai_attribute_t *attr_list);

sai_status_t remove_ipinip_term_entry(_In_ const std::string &serializedObjectId);
```

### 4. `SwitchStateBase.cpp` dispatch additions

```cpp
if (object_type == SAI_OBJECT_TYPE_TUNNEL) {
    // check SAI_TUNNEL_ATTR_TYPE; dispatch IPINIP to TunnelManager
    return m_tunnel_mgr.create_ipinip_tunnel(...);
}
if (object_type == SAI_OBJECT_TYPE_TUNNEL_TERM_TABLE_ENTRY) {
    return m_tunnel_mgr.create_ipinip_term_entry(...);
}
```

---

## References

- VPP IPIP API: `vpp/src/vnet/ipip/ipip.api`
- SONiC `TunnelDecapOrch`: `sonic-swss/orchagent/tunneldecaporch.cpp`
- SAI tunnel attributes: `SAI/inc/saitunnel.h`
- Existing VxLAN implementation: `saivpp/src/TunnelManager.cpp`
