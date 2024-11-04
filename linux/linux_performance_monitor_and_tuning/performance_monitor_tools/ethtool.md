# ethtool

## show NIC configuration
```
# ethtool eth0
Settings for eth0:
        Supported ports: [ TP ]
        Supported link modes:   10baseT/Half 10baseT/Full
                                100baseT/Half 100baseT/Full
                                1000baseT/Half 1000baseT/Full
        Supported pause frame use: No
        Supports auto-negotiation: Yes
        Supported FEC modes: Not reported
        Advertised link modes:  10baseT/Half 10baseT/Full
                                100baseT/Half 100baseT/Full
                                1000baseT/Half 1000baseT/Full
        Advertised pause frame use: No
        Advertised auto-negotiation: Yes
        Advertised FEC modes: Not reported
        Link partner advertised link modes:  1000baseT/Full
        Link partner advertised pause frame use: No
        Link partner advertised auto-negotiation: Yes
        Link partner advertised FEC modes: Not reported
        Speed: 1000Mb/s
        Duplex: Full
        Auto-negotiation: on
        Port: Twisted Pair
        PHYAD: 1
        Transceiver: internal
        MDI-X: off
        Supports Wake-on: g
        Wake-on: d
        Current message level: 0x000000ff (255)
                               drv probe link timer ifdown ifup rx_err tx_err
```
## show NIC driver information
* `-i`, `--driver`
```
# ethtool -i eth0
driver: tg3
version: 5.10.0-30-2-amd64
firmware-version: sb
expansion-rom-version:
bus-info: 0000:02:00.0
supports-statistics: yes
supports-test: yes
supports-eeprom-access: no
supports-register-dump: yes
supports-priv-flags: no
```

## show statistics
```
# ethtool -S eth0
NIC statistics:
     rx_octets: 314843626
     rx_fragments: 0
     rx_ucast_packets: 237992
     rx_mcast_packets: 258922
     rx_bcast_packets: 2544401
     rx_fcs_errors: 0
     rx_align_errors: 0
     rx_xon_pause_rcvd: 0
     rx_xoff_pause_rcvd: 0
     rx_mac_ctrl_rcvd: 0
     rx_xoff_entered: 0
     rx_frame_too_long_errors: 0
     rx_jabbers: 0
     rx_undersize_packets: 0
     rx_in_length_errors: 0
     rx_out_length_errors: 0
```
## show rx/tx ring buffer parameters
```
# ethtool -g eth0
Ring parameters for eth0:
Pre-set maximums:
RX:             511
RX Mini:        n/a
RX Jumbo:       n/a
TX:             511
Current hardware settings:
RX:             200
RX Mini:        n/a
RX Jumbo:       n/a
TX:             511
```
