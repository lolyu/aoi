# decap case debug

```
Summary:
IPv4inIPv4: Failed
IPv6inIPv4: Passed
IPv4inIPv6: Failed
IPv6inIPv6: Passe
```

```
/root/env-python3/bin/ptf --test-dir ptftests/py3 IP_decap_test.DecapPacketTest --platform-dir ptftests --qlen=1000 --platform remote -t 'outer_ipv4=True;outer_ipv6=True;inner_ipv4=True;inner_ipv6=True;lo_ips=["10.1.0.32"];lo_ipv6s=["fc00:1::32"];ttl_mode="pipe";dscp_mode="pipe";asic_type="vpp";ignore_ttl=False;max_internal_hops=0;fib_info_files=["/root/fib_info_dut0.txt"];single_fib_for_duts="multiple-fib";ptf_test_port_map="/root/ptf_test_port_map.json";topo="t0";qos_remap_enabled=False' --relax --debug info --log-file /tmp/decap.2026-05-13-06:48:19.log
```
```
(Pdb) pkt.show()
###[ Ethernet ]###
  dst       = 22:59:d0:d5:19:a9
  src       = fe:54:00:98:10:16
  type      = IPv4
###[ IP ]###
     version   = 4
     ihl       = None
     tos       = 0x58
     len       = None
     id        = 1
     flags     =
     frag      = 0
     ttl       = 8
     proto     = ipencap
     chksum    = None
     src       = 1.1.1.1
     dst       = 10.1.0.32
     \options   \
###[ IP ]###
        version   = 4
        ihl       = None
        tos       = 0x18
        len       = None
        id        = 1
        flags     =
        frag      = 0
        ttl       = 40
        proto     = tcp
        chksum    = None
        src       = 1.1.1.1
        dst       = 240.0.0.0
        \options   \
###[ TCP ]###
           sport     = 1234
           dport     = http
           seq       = 0
           ack       = 0
           dataofs   = None
           reserved  = 0
           flags     = S
           window    = 8192
           chksum    = None
           urgptr    = 0
           options   = []
###[ Raw ]###
              load      = b'\x00\x01\x02\x03\x04\x05\x06\x07\x08\t\n\x0b\x0c\r\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f !"#$%&\'()*+,-./0123456789:;'
```

```
Packet 16

00:46:59:532483: dpdk-input
  bobm22 rx queue 0
  buffer 0x25eacd: current data 0, length 134, buffer-pool 0, ref-count 1, trace handle 0x100000f
                   ext-hdr-valid
  PKT MBUF: port 22, nb_segs 1, pkt_len 134
    buf_len 2176, data_len 134, ol_flags 0x0, data_off 128, phys_addr 0xd53ab3c0
    packet_type 0x0 l2_len 0 l3_len 0 outer_l2_len 0 outer_l3_len 0
    rss 0x0 fdir.hi 0x0 fdir.lo 0x0
  IP4: fe:54:00:98:10:16 -> 22:59:d0:d5:19:a9
  IP_IN_IP: 1.1.1.1 -> 10.1.0.32
    tos 0x58, ttl 8, length 120, checksum 0xa607 dscp AF23 ecn NON_ECN
    fragment id 0x0001
00:46:59:532531: ethernet-input
  frame: flags 0x1, hw-if-index 23, sw-if-index 23
  IP4: fe:54:00:98:10:16 -> 22:59:d0:d5:19:a9
00:46:59:532590: l2-input
  l2-input: sw_if_index 23 dst 22:59:d0:d5:19:a9 src fe:54:00:98:10:16 [l2-input-feat-arc l2-input-vtr l2-learn l2-fwd l2-flood arp-term-l2bd l2-flood ]
00:46:59:532598: l2-input-feat-arc
  IN-FEAT-ARC: head 1 feature_bitmap 86b5 ethertype 800 sw_if_index 23, next_index 15
00:46:59:532600: acl-plugin-in-ip4-l2
  acl-plugin: lc_index: 25, sw_if_index 23, next index 1, action: 1, match: acl 1 rule 0 trace_bits 00000000
  pkt info 0000000000000000 0000000000000000 0000000000000000 2000010a01010101 0017030400000000 0200ffff00000019
   lc_index 25 l3 ip4 1.1.1.1 -> 10.1.0.32 l4 lsb_of_sw_if_index 23 proto 4 l4_is_input 1 l4_slow_path 1 l4_flags 0x03 port 0 -> 0 tcp flags (invalid) 00 rsvd 0
00:46:59:532663: l2-input-feat-arc-end
  IN-FEAT-ARC: head 0 feature_bitmap 6b5 ethertype 0 sw_if_index -1, next_index 8
00:46:59:532679: l2-input-vtr
  l2-input-vtr: sw_if_index 23 dst 22:59:d0:d5:19:a9 src fe:54:00:98:10:16 data 81 00 03 e8 08 00 45 58 00 78 00 01
00:46:59:532690: l2-learn
  l2-learn: sw_if_index 23 dst 22:59:d0:d5:19:a9 src fe:54:00:98:10:16 bd_index 1
00:46:59:532694: l2-fwd
  l2-fwd:   sw_if_index 23 dst 22:59:d0:d5:19:a9 src fe:54:00:98:10:16 bd_index 1 result [0x700000057, 87] static age-not bvi
00:46:59:532698: ip4-input
  IP_IN_IP: 1.1.1.1 -> 10.1.0.32
    tos 0x58, ttl 8, length 120, checksum 0xa607 dscp AF23 ecn NON_ECN
    fragment id 0x0001
00:46:59:532711: ip4-validate
  IP4-VALIDATE: sw_if_index 87 next 1 src 1.1.1.1 dst 10.1.0.32
00:46:59:532717: ip4-lookup
  fib 0 dpo-idx 39 flow hash: 0x00000000
  IP_IN_IP: 1.1.1.1 -> 10.1.0.32
    tos 0x58, ttl 8, length 120, checksum 0xa607 dscp AF23 ecn NON_ECN
    fragment id 0x0001
00:46:59:532730: ip4-receive
    fib:0 adj:39 flow:0x00000000
  IP_IN_IP: 1.1.1.1 -> 10.1.0.32
    tos 0x58, ttl 8, length 120, checksum 0xa607 dscp AF23 ecn NON_ECN
    fragment id 0x0001
00:46:59:532738: ipip4-input
  IPIP: tunnel 85 len 120 src 1.1.1.1 dst 10.1.0.32
00:46:59:532754: ip4-input
  TCP: 1.1.1.1 -> 240.0.0.0
    tos 0x18, ttl 40, length 100, checksum 0xa079 dscp unknown ecn NON_ECN
    fragment id 0x0001
  TCP: 1234 -> 80
    seq. 0x00000000 ack 0x00000000
    flags 0x02 SYN, tcp header: 20 bytes
    window 8192, checksum 0x2efb

00:46:59:532754: ip4-validate
  IP4-VALIDATE: sw_if_index 85 next 1 src 1.1.1.1 dst 240.0.0.0
00:46:59:532755: ip4-lookup
  fib 0 dpo-idx 0 flow hash: 0x00000000
  TCP: 1.1.1.1 -> 240.0.0.0
    tos 0x18, ttl 40, length 100, checksum 0xa079 dscp unknown ecn NON_ECN
    fragment id 0x0001
  TCP: 1234 -> 80
    seq. 0x00000000 ack 0x00000000
    flags 0x02 SYN, tcp header: 20 bytes
    window 8192, checksum 0x2efb

00:46:59:532756: ip4-drop
    fib:0 adj:0 flow:0x00000000
  TCP: 1.1.1.1 -> 240.0.0.0
    tos 0x18, ttl 40, length 100, checksum 0xa079 dscp unknown ecn NON_ECN
    fragment id 0x0001
  TCP: 1234 -> 80
    seq. 0x00000000 ack 0x00000000
    flags 0x02 SYN, tcp header: 20 bytes
    window 8192, checksum 0x2efb

00:46:59:532761: error-drop
  rx:ipip10
00:46:59:532765: drop
  ip4-local: unknown ip protocol
```