# VPP trace packet

```
vpp# trace add dpdk-input 10
vpp# show trace
vpp# show trace
------------------- Start of thread 0 vpp_main -------------------
No packets in trace buffer
------------------- Start of thread 1 vpp_wk_0 -------------------
Packet 10

03:21:53:937423: dpdk-input
bobm28 rx queue 0
buffer 0x149cf3: current data 0, length 62, buffer-pool 0, ref-count 1, trace handle 0x1000009
ext-hdr-valid
PKT MBUF: port 28, nb_segs 1, pkt_len 62
buf_len 2176, data_len 62, ol_flags 0x0, data_off 128, phys_addr 0x6b873d40
packet_type 0x0 l2_len 0 l3_len 0 outer_l2_len 0 outer_l3_len 0
rss 0x0 fdir.hi 0x0 fdir.lo 0x0
IP4: 3e:38:be:ea:b0:1c -> 22:59:d0:d5:19:a9
IP_IN_IP: 10.10.10.10 -> 192.168.0.1
tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
fragment id 0x0001
03:21:53:937456: bond-input
src 3e:38:be:ea:b0:1c, dst 22:59:d0:d5:19:a9, bobm28 -> BondEthernet101
03:21:53:937466: ethernet-input
IP4: 3e:38:be:ea:b0:1c -> 22:59:d0:d5:19:a9
03:21:53:937471: ip4-input
IP_IN_IP: 10.10.10.10 -> 192.168.0.1
tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
fragment id 0x0001
03:21:53:937475: acl-plugin-in-ip4-fa
acl-plugin: lc_index: 0, sw_if_index 65, next index 1, action: 1, match: acl 1 rule 0 trace_bits 00000000
pkt info 0000000000000000 0000000000000000 0000000000000000 0100a8c00a0a0a0a 0041030400000000 0200ffff00000000
lc_index 0 l3 ip4 10.10.10.10 -> 192.168.0.1 l4 lsb_of_sw_if_index 65 proto 4 l4_is_input 1 l4_slow_path 1 l4_flags 0x03 port 0 -> 0 tcp flags (invalid) 00 rsvd 0
03:21:53:937483: ip4-lookup
fib 0 dpo-idx 54 flow hash: 0x00000000
IP_IN_IP: 10.10.10.10 -> 192.168.0.1
tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
fragment id 0x0001
03:21:53:937492: ip4-receive
fib:0 adj:54 flow:0x00000000
IP_IN_IP: 10.10.10.10 -> 192.168.0.1
tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
fragment id 0x0001
03:21:53:937500: ipip4-input
IPIP: tunnel 76 len 48 src 10.10.10.10 dst 192.168.0.1
03:21:53:937510: ip4-input
ICMP: 20.20.20.20 -> 20.20.20.20
tos 0x00, ttl 64, length 28, checksum 0x2a91 dscp CS0 ecn NON_ECN
fragment id 0x0001
ICMP echo_request checksum 0xf7ff id 0
03:21:53:937510: ip4-not-enabled
fib:0 adj:1 flow:0x00000000
ICMP: 20.20.20.20 -> 20.20.20.20
tos 0x00, ttl 64, length 28, checksum 0x2a91 dscp CS0 ecn NON_ECN
fragment id 0x0001
ICMP echo_request checksum 0xf7ff id 0
03:21:53:937513: error-drop
rx:ipip0
03:21:53:937514: drop
ip4-local: unknown ip protocol
```

## P2MP tunnel miss — no TEIB entry (tunnel -1)

`ipip4-input: tunnel -1` means the P2P hash lookup `{src=10.10.10.10, dst=192.168.0.1}` found
no match.  This happens when the P2MP tunnel exists but no TEIB entry has been added for the
peer `10.10.10.10`, so the per-peer P2P key is absent from the tunnel DB.

```
00:18:48:250203: dpdk-input
  bobm28 rx queue 0
  buffer 0x15b828: current data 0, length 62, buffer-pool 0, ref-count 1, trace handle 0x1000005
                   ext-hdr-valid
  PKT MBUF: port 28, nb_segs 1, pkt_len 62
    buf_len 2176, data_len 62, ol_flags 0x0, data_off 128, phys_addr 0x838e0a80
    packet_type 0x0 l2_len 0 l3_len 0 outer_l2_len 0 outer_l3_len 0
    rss 0x0 fdir.hi 0x0 fdir.lo 0x0
  IP4: 3e:38:be:ea:b0:1c -> 22:59:d0:d5:19:a9
  IP_IN_IP: 10.10.10.10 -> 192.168.0.1
    tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
    fragment id 0x0001
00:18:48:250208: bond-input
  src 3e:38:be:ea:b0:1c, dst 22:59:d0:d5:19:a9, bobm28 -> BondEthernet101
00:18:48:250208: ethernet-input
  IP4: 3e:38:be:ea:b0:1c -> 22:59:d0:d5:19:a9
00:18:48:250209: ip4-input
  IP_IN_IP: 10.10.10.10 -> 192.168.0.1
    tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
    fragment id 0x0001
00:18:48:250209: acl-plugin-in-ip4-fa
  acl-plugin: lc_index: 0, sw_if_index 65, next index 1, action: 1, match: acl 1 rule 0 trace_bits 00000000
  pkt info 0000000000000000 0000000000000000 0000000000000000 0100a8c00a0a0a0a 0041030400000000 0200ffff00000000
   lc_index 0 l3 ip4 10.10.10.10 -> 192.168.0.1 l4 lsb_of_sw_if_index 65 proto 4 l4_is_input 1 l4_slow_path 1 l4_flags 0x03 port 0 -> 0 tcp flags (invalid) 00 rsvd 0
00:18:48:250210: ip4-lookup
  fib 0 dpo-idx 54 flow hash: 0x00000000
  IP_IN_IP: 10.10.10.10 -> 192.168.0.1
    tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
    fragment id 0x0001
00:18:48:250210: ip4-receive
    fib:0 adj:54 flow:0x00000000
  IP_IN_IP: 10.10.10.10 -> 192.168.0.1
    tos 0x00, ttl 64, length 48, checksum 0xa60c dscp CS0 ecn NON_ECN
    fragment id 0x0001
00:18:48:250211: ipip4-input
  IPIP: tunnel -1 len 48 src 10.10.10.10 dst 192.168.0.1
00:18:48:250212: error-drop
  rx:BondEthernet101
00:18:48:250212: drop
  ipip4-input: no tunnel
```