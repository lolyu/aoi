# SONiC VPP Introduction
* agenda:
    * VPP basics
    * How does SONiC manipulates VPP?
    * Debug Tips

## VPP Basics
```mermaid
graph TB
    subgraph before["SONiC VS (Before)"]
        direction TB
        A1[CLI / gNMI] --> B1[CONFIG_DB]
        B1 --> C1[orchagent]
        C1 -->|SAI API| D1["libsaivs.so<br/>(saivs::Sai)"]
        D1 -->|"store in memory<br/>NO data plane call"| E1["SAI Object DB<br/>(m_objectHash)"]
        E1 -->|"no programming"| F1["Linux Kernel<br/>(tap interfaces)"]
        F1 -->|"kernel FIB<br/>forwarding"| G1["eth0...ethN<br/>(PTF ports)"]

        style E1 fill:#fff3cd,stroke:#ffc107
        style F1 fill:#d4edda,stroke:#28a745
    end

    subgraph after["SONiC VPP (After)"]
```

| #  | Node                 | What It Does                                                                                                                                                                   |
| --- | -------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 1  | dpdk-input           | NIC poll — DPDK PMD reads the BGP packet from physical port bobm30.                                                                                                            |
| 2  | bond-input           | LAG mapping — Physical bobm30 → logical BondEthernet103 (PortChannel member).                                                                                                  |
| 3  | ethernet-input       | L2 parse — Strips Ethernet header, ethertype 0x0800 → IPv4.                                                                                                                    |
| 4  | ip4-input            | IP validation — Validates IPv4 header. dst=10.0.0.60 (our IP), proto=TCP.                                                                                                      |
| 5  | acl-plugin-in-ip4-fa | Ingress ACL — Checks against CoPP/ACL rules. BGP (TCP/179) is permitted.                                                                                                       |
| 6  | ip4-lookup           | FIB lookup — Looks up 10.0.0.60 in FIB 0. DPO index 48 resolves to local (it's our own address).                                                                               |
| 7  | ip4-receive          | Local delivery decision — Destination is us. VPP checks protocol — TCP doesn't have a built-in VPP handler, so it's not consumed in VPP. Passes to punt.                       |
| 8  | ip4-punt             | Punt decision — VPP can't process TCP/BGP natively. Packet is "punted" — redirected from the fast data plane to the slow control plane (Linux kernel).                         |
| 9  | ip4-punt-redirect    | Punt redirect — Looks up the punt redirect rule (index 68). This was configured by SONiC to redirect punted packets via a specific path instead of the default punt socket.    |
| 10 | ip4-dvr-dpo          | DVR (Distributed Virtual Router) — Routes the punted packet through sw_if_index 71. DVR allows L3 packets to be delivered across a bridge domain to the correct tap interface. |
| 11 | ip4-dvr-reinject     | Reinject to tap path — Repackages the packet for delivery to the Linux tap interface. Adds the necessary L2 header for the tap.                                                |
| 12 | tap4130-output       | Tap output queue — Enqueues the packet to tap4130, which is a virtio tap interface connected to the Linux kernel network namespace.                                            |
| 13 | tap4130-tx           | Tap transmit — Frame written to the tap fd. Linux kernel receives it → TCP stack → FRR bgpd reads the BGP message on its socket.                                               |


Traditional API:   Client → serialize → network → deserialize → Server
VPP Binary API:    Client → write struct to shared memory → Server reads it


```text
┌───────────────────────────────────────────────┐          ┌──────────────────────────────────────────────────────────┐
│  ORCHAGENT / SONiC Client Applications        │          │  SYNCD-VPP Container (runs both syncd AND VPP process)  │
│  (same as standard SONiC)                     │       ┌──┤                                                          │
└───────────────────────────────────────────────┘       │  └──────────────────────────────────────────────────────────┘
         │ sai_api_initialize()                         │           │
         ▼                                              │        Syncd ──────────────────────────────── syncd/Syncd.cpp
      lib/Sai ──────────────── manages contexts         │           │
         │                                              │           ├── RedisSelectableChannel (reads ASIC_DB)
         ▼                                              │           │
      Context (per switch) ──── lib/Context.cpp         │           ├── receives & deserializes commands
         │                                              │           │
         ├── creates RedisRemoteSaiInterface            │           └── loads saivpp.so (platform/vpp/saivpp)
         └── wraps with Meta (validation)               │                    │
                   │                                    │                    ▼
                   │ validates attributes               │           saivpp::Sai ──────────────────── saivpp/src/Sai.h
                   ▼                                    │                    │                       (namespace saivpp)
      RedisRemoteSaiInterface ── RedisChannel           │                    ├── wraps with Meta (validation)
                   │                                    │                    └── VirtualSwitchSaiInterface
                   │ serializes SAI calls               │                              │
                   ▼                                    │                              └── SwitchVpp (subclass)
      ═══════════════════════════════                   │                                        │
        Redis ASIC_DB (IPC boundary) ───────────────────┘                                        │ translates SAI → VPP API
      ═══════════════════════════════                                                            ▼
                                                                              vppxlate/SaiVppXlate.c ── C translation layer
                                                                                        │
                                                                                        │ VPP Binary API (shared memory)
                                                                                        ▼
                                                                 ══════════════════════════════════════════════════
                                                                   VPP Process (same container, separate process)
                                                                 ══════════════════════════════════════════════════
                                                                                  │
                                                                                  ├── Master Thread (handles config)
                                                                                  ├── Worker Threads (packet processing)
                                                                                  └── Linux-CP Plugin (creates taps)
                                                                                  │
                                                                                  ▼
                                                                         DPDK / Network Interfaces
                                                                                  │
                                                                                  └── Tap interfaces exposed to Linux
                                                                                        (EthernetX seen by SONiC)
```
```
                  SAI API Call (e.g. sai_route_api->create_route_entry)
                                        │
                                        ▼
                  ┌─────────────────────────────────────────────────┐
                  │          SwitchVpp::create(object_type, ...)    │
                  │          Dispatch by SAI_OBJECT_TYPE            │
                  └─────────────────────┬───────────────────────────┘
                                        │
        ┌───────────┬───────────┬───────┴───────┬───────────┬───────────┐
        ▼           ▼           ▼               ▼           ▼           ▼
  ROUTE_ENTRY  NEIGHBOR    ROUTER_IF        HOSTIF     VLAN_MEMBER  FDB_ENTRY ...
        │         │            │               │           │           │
        ▼         ▼            ▼               ▼           ▼           ▼
   addIpRoute  addIpNbr  createRouterif  createHostif createVlanMbr FdbEntryadd
        │         │            │               │           │           │
        │         │            │               │           │           │
════════╪═════════╪════════════╪═══════════════╪═══════════╪═══════════╪══════
        │  vppxlate/SaiVppXlate.c  (C translation layer)  │           │
════════╪═════════╪════════════╪═══════════════╪═══════════╪═══════════╪══════
        │         │            │               │           │           │
        ▼         ▼            ▼               ▼           ▼           ▼
ip_route_    ip_nbr_     set_interface   configure_  set_sw_if_   l2fib_
add_del()   add_del()    _vrf()         lcp_intf()  l2_bridge()  add_del()
                         create_sub_if()
                         if_ip_addr_add()
        │         │            │               │           │           │
════════╪═════════╪════════════╪═══════════════╪═══════════╪═══════════╪══════
        │          VPP Binary API (shared memory messages)             │
════════╪═════════╪════════════╪═══════════════╪═══════════╪═══════════╪══════
        │         │            │               │           │           │
        ▼         ▼            ▼               ▼           ▼           ▼
ip_route_  ip_neighbor_ sw_interface_  lcp_itf_pair_ sw_interface_ l2fib_
add_del    add_del      set_table      add_del       set_l2_bridge add_del
                        create_subif
                        sw_if_add_del
                        _address
```

```

# make init
# make SONIC_BUILD_JOBS=16 configure PLATFORM=vpp
# make -f Makefile.work INCLUDE_FIPS=n SONIC_BUILD_JOBS=16 NOBULLSEYE=1 NOBUSTER=1 NOSTRETCH=1 NOJESSIE=1 SLAVE_DIR=sonic-slave-bookworm KEEP_SLAVE_ON=yes target/docker-syncd-vpp-dbg.gz

```