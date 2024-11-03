## ss
`ss` prints more detailed information than `netstat`

* common options:
    * `-n`: numeric
    * `-a`: all
    * `-l`: listening
    * `-e`: show socket detailed information
    * `-m`: show socket memory information
    * `-0`, `--packet`
    * `-t`, `--tcp`
    * `-u`, `--udp`
    * `-w`, `--raw`

## show ipv4 or ipv6 sockets
```
# ss -4
Netid State Recv-Q Send-Q  Local Address:Port  Peer Address:Port Process
tcp   ESTAB 0      0           127.0.0.1:redis    127.0.0.1:39288
tcp   ESTAB 0      0           127.0.0.1:41524    127.0.0.1:redis
tcp   ESTAB 0      0           127.0.0.1:40792    127.0.0.1:5570
tcp   ESTAB 0      0           127.0.0.1:redis    127.0.0.1:33710
tcp   ESTAB 0      0           127.0.0.1:39024    127.0.0.1:redis
tcp   ESTAB 0      0           127.0.0.1:redis    127.0.0.1:38866
tcp   ESTAB 0      0           127.0.0.1:39448    127.0.0.1:redis
tcp   ESTAB 0      0           127.0.0.1:redis    127.0.0.1:38566
tcp   ESTAB 0      0      10.206.144.131:ssh     10.1.84.57:39522
```
```
# ss -6
Netid                    State                     Recv-Q                     Send-Q                                                Local Address:Port                                                       Peer Address:Port                     Process
icmp6                    UNCONN                    0                          0                                                        *%Vlan1000:ipv6-icmp                                                             *:*
icmp6                    UNCONN                    219136                     0                                                                 *:ipv6-icmp                                                             *:*
icmp6                    UNCONN                    0                          0                                                                 *:ipv6-icmp                                                             *:*
tcp                      ESTAB                     0                          0                                              [::ffff:192.168.0.1]:41256                                             [::ffff:192.168.0.27]:50075
tcp                      ESTAB                     0                          0                                              [::ffff:192.168.0.1]:45980                                             [::ffff:192.168.0.37]:50075
tcp                      ESTAB                     0                          0                                              [::ffff:192.168.0.1]:42144                                             [::ffff:192.168.0.33]:50075
```

## filter by ip or port
```
# ss -ap src 0.0.0.0:22
Netid                       State                        Recv-Q                       Send-Q                                             Local Address:Port                                             Peer Address:Port                       Process
tcp                         LISTEN                       0                            128                                                      0.0.0.0:ssh                                                   0.0.0.0:*                           users:(("sshd",pid=1457,fd=4))
```
```
# ss -ap '( dport = :22 or sport = :22 )'
Netid                 State                  Recv-Q                 Send-Q                                  Local Address:Port                                 Peer Address:Port                 Process
tcp                   LISTEN                 0                      128                                           0.0.0.0:ssh                                       0.0.0.0:*                     users:(("sshd",pid=1457,fd=4))
tcp                   ESTAB                  0                      0                                      10.206.144.131:ssh                                    10.1.84.57:39522                 users:(("sshd",pid=3598781,fd=4),("sshd",pid=3598684,fd=4))
tcp                   LISTEN                 0                      128                                              [::]:ssh                                          [::]:*                     users:(("sshd",pid=1457,fd=3))
```
```
# ss -ap src :22 or dst :22
Netid                 State                  Recv-Q                 Send-Q                                  Local Address:Port                                 Peer Address:Port                 Process
tcp                   LISTEN                 0                      128                                           0.0.0.0:ssh                                       0.0.0.0:*                     users:(("sshd",pid=1457,fd=4))
tcp                   ESTAB                  0                      0                                      10.206.144.131:ssh                                    10.1.84.57:39522                 users:(("sshd",pid=3598781,fd=4),("sshd",pid=3598684,fd=4))
tcp                   LISTEN                 0                      128                                              [::]:ssh                                          [::]:*                     users:(("sshd",pid=1457,fd=3))
```

## list all packet sockets
```
# ss -0 -ap
Netid                  State                   Recv-Q                  Send-Q                                   Local Address:Port                                               Peer Address:Port                  Process
p_raw                  UNCONN                  0                       0                                                 ipv6:Vlan1000                                                       *                       users:(("ndppd",pid=743887,fd=4))
p_raw                  UNCONN                  0                       0                                              [34825]:Ethernet116                                                    *                       users:(("teamd",pid=743033,fd=19))
p_raw                  UNCONN                  0                       0                                              [34825]:Ethernet112                                                    *                       users:(("teamd",pid=742944,fd=19))
p_raw                  UNCONN                  0                       0                                              [34825]:Ethernet120                                                    *                       users:(("teamd",pid=743071,fd=19))
p_raw                  UNCONN                  0                       0                                              [34825]:Ethernet124                                                    *                       users:(("teamd",pid=743094,fd=19))
p_raw                  UNCONN                  0                       0                                                    *:*                                                              *                       users:(("dhcp6relay",pid=745030,fd=11))
p_raw                  UNCONN                  0                       0                                                    *:*                                                              *                       users:(("dhcpmon",pid=745117,fd=17))
p_raw                  UNCONN                  0                       0                                                    *:*                                                              *                       users:(("dhcpmon",pid=745117,fd=18))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet12                                                     *                       users:(("linkmgrd",pid=745150,fd=56))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet16                                                     *                       users:(("linkmgrd",pid=745150,fd=57))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet20                                                     *                       users:(("linkmgrd",pid=745150,fd=58))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet24                                                     *                       users:(("linkmgrd",pid=745150,fd=59))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet28                                                     *                       users:(("linkmgrd",pid=745150,fd=60))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet32                                                     *                       users:(("linkmgrd",pid=745150,fd=61))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet36                                                     *                       users:(("linkmgrd",pid=745150,fd=62))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet4                                                      *                       users:(("linkmgrd",pid=745150,fd=63))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet44                                                     *                       users:(("linkmgrd",pid=745150,fd=64))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet48                                                     *                       users:(("linkmgrd",pid=745150,fd=65))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet40                                                     *                       users:(("linkmgrd",pid=745150,fd=66))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet52                                                     *                       users:(("linkmgrd",pid=745150,fd=67))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet56                                                     *                       users:(("linkmgrd",pid=745150,fd=68))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet60                                                     *                       users:(("linkmgrd",pid=745150,fd=69))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet64                                                     *                       users:(("linkmgrd",pid=745150,fd=70))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet68                                                     *                       users:(("linkmgrd",pid=745150,fd=71))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet72                                                     *                       users:(("linkmgrd",pid=745150,fd=72))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet76                                                     *                       users:(("linkmgrd",pid=745150,fd=73))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet8                                                      *                       users:(("linkmgrd",pid=745150,fd=74))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet80                                                     *                       users:(("linkmgrd",pid=745150,fd=75))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet84                                                     *                       users:(("linkmgrd",pid=745150,fd=76))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet88                                                     *                       users:(("linkmgrd",pid=745150,fd=77))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet92                                                     *                       users:(("linkmgrd",pid=745150,fd=78))
p_raw                  UNCONN                  0                       0                                                    *:Ethernet96                                                     *                       users:(("linkmgrd",pid=745150,fd=79))
```

## list local processes connected to a unix domain socket
```
t# ss -x src /var/run/redis/redis.sock
Netid State Recv-Q Send-Q             Local Address:Port    Peer Address:Port   Process
u_str ESTAB 0      0      /var/run/redis/redis.sock 4401298            * 4399230
u_str ESTAB 0      0      /var/run/redis/redis.sock 4397990            * 4396748
u_str ESTAB 0      0      /var/run/redis/redis.sock 4386780            * 4385220
u_str ESTAB 0      0      /var/run/redis/redis.sock 4385203            * 4388137
u_str ESTAB 0      0      /var/run/redis/redis.sock 4381926            * 4381925
u_str ESTAB 0      0      /var/run/redis/redis.sock 4380499            * 4380498
u_str ESTAB 0      0      /var/run/redis/redis.sock 4377043            * 4377042
u_str ESTAB 0      0      /var/run/redis/redis.sock 4386728            * 4388202
u_str ESTAB 0      0      /var/run/redis/redis.sock 4380665            * 4380664
```

