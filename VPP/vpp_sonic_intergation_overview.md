
# SONiC VPP Architecture Overview

## VPP SONiC Architecture

VPP SONiC uses a **software dataplane** approach where VPP process acts as the forwarding engine instead of hardware ASIC.

```
┌─────────────────────────────────────────────────────────────────────────────┐
│  ORCHAGENT / SONiC Client Applications (same as standard SONiC)             │
└─────────────────────────────────────────────────────────────────────────────┘
         │ sai_api_initialize()
         ▼
      lib/Sai ────────────────────────────────────────────── manages contexts
         │
         ▼
      Context (per switch) ──────────────────────────────── lib/Context.cpp
         │
         ├── creates RedisRemoteSaiInterface
         └── wraps with Meta (validation)
                   │
                   │ validates attributes
                   ▼
         RedisRemoteSaiInterface ──────────────────────────  RedisChannel
                   │
                   │ serializes SAI calls
                   ▼
         ══════════════════════════════════════════════════════════════════════
              Redis ASIC_DB (IPC boundary)
         ══════════════════════════════════════════════════════════════════════
                   │
                   ▼

┌─────────────────────────────────────────────────────────────────────────────┐
│  SYNCD-VPP Container (runs both syncd AND VPP process)                      │
└─────────────────────────────────────────────────────────────────────────────┘
         │
      Syncd ───────────────────────────────────────────────── syncd/Syncd.cpp
         │
         ├── RedisSelectableChannel (reads ASIC_DB)
         │
         ├── receives & deserializes commands
         │
         └── loads saivpp.so (platform/vpp/saivpp)
                   │
                   ▼
         saivpp::Sai ──────────────────────────────────────── saivpp/src/Sai.h
                   │                                            (namespace saivpp)
                   ├── wraps with Meta (validation)
                   └── VirtualSwitchSaiInterface
                             │
                             └── SwitchVpp (subclass)
                                       │
                                       │ translates SAI → VPP API
                                       ▼
                             vppxlate/SaiVppXlate.c ───────── C translation layer
                                       │
                                       │ VPP Binary API (shared memory)
                                       ▼
         ══════════════════════════════════════════════════════════════════════
              VPP Process (same container, separate process)
         ══════════════════════════════════════════════════════════════════════
                   │
                   ├── Master Thread (handles config requests)
                   ├── Worker Threads (packet processing)
                   └── Linux-CP Plugin (creates tap interfaces)
                   │
                   ▼
            DPDK / Network Interfaces
                   │
                   └── Tap interfaces exposed to Linux namespace
                         (EthernetX seen by SONiC)
```

**Key Differences from Standard SONiC:**

| Aspect | Standard SONiC | VPP SONiC |
|--------|---------------|-----------|
| **Hardware** | Real ASIC (Broadcom, Mellanox, etc.) | Software dataplane (VPP) |
| **Syncd SAI** | VendorSai → vendor .so → ASIC SDK | saivpp::Sai → VirtualSwitchSaiInterface → VPP API |
| **Dataplane** | Hardware forwarding | VPP software forwarding (DPDK) |
| **Location** | VPP N/A | VPP runs in syncd container |
| **Interface** | N/A | VPP Binary API (shared memory IPC) |
| **Front-panel** | Physical ports | Tap interfaces (veth/VF/passthrough) |
| **Meta validation** | Client-side only (orchagent) | Both client (orchagent) AND syncd |
| **Switch type** | Real hardware switch | `SAI_VS_SWITCH_TYPE_VPP` in sai.profile |

**VPP Translation Example** (Route Add):
1. orchagent: `create_route_entry()` → ASIC_DB
2. syncd: reads ASIC_DB → calls `saivpp::Sai::create()`
3. Meta: validates SAI attributes
4. SwitchVpp: `addIpRoute()` → converts to `vpp_ip_route_t`
5. SaiVppXlate.c: `ip_route_add_del()` → constructs `vl_api_ip_route_add_del_t`
6. VPP: processes binary API message → updates routing table
7. VPP: sends reply back through shared memory
8. syncd: writes response to ASIC_DB

**Notable:** VPP SONiC reuses most of vslib's `VirtualSwitchSaiInterface` code, with `SwitchVpp` as a specialized subclass that adds VPP-specific translation logic in `vppxlate/`.
