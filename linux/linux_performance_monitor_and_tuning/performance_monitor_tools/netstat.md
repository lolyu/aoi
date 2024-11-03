# netstat

* options:
    * `-a`
        * show both listening and non-listneing sockets
        * show all interfaces (up and not-up)
    * `-p`
        * show the pid the socket belongs
    * `-n`
        * show numeric address instead of resolving
    * `-v`
        * verbose
    * `-c`
        * continuously monitor the system

## show interfaces
```
# netstat -i -a
Kernel Interface table
Iface      MTU    RX-OK RX-ERR RX-DRP RX-OVR    TX-OK TX-ERR TX-DRP TX-OVR Flg
docker0   1500   499303      0      0 0        793474      0      0      0 BMRU
enP55029  1500 26207210      0     73 0      23132789      0      0      0 BMsRU
eth0      1500 13079834      0      0 0      14826789      0      0      0 BMRU
lo       65536  7146370      0      0 0       7146370      0      0      0 LRU
veth01d1  1500        0      0      0 0            65      0      0      0 BMRU
veth0ef8  1500     1972      0      0 0          2809      0      0      0 BMRU
veth1b2a  1500     4113      0      0 0          8888      0      0      0 BMRU
veth244a  1500     3531      0      0 0          8658      0      0      0 BMRU
veth2fc7  1500        0      0      0 0            64      0      0      0 BMRU
veth458c  1500     6112      0      0 0         10643      0      0      0 BMRU
veth4deb  1500       14      0      0 0           386      0      0      0 BMRU
veth54f4  1500      481      0      0 0          1037      0      0      0 BMRU
veth8c0b  1500     2060      0      0 0          4515      0      0      0 BMRU
vethd5b3  1500     6386      0      0 0         11294      0      0      0 BMRU
vethf028  1500    20205      0      0 0         33377      0      0      0 BMRU
virbr0    1500        0      0      0 0             0      0      0      0 BMU
virbr0-n  1500        0      0      0 0             0      0      0      0 BM
```
* the last column is the interface flag:
    * `B`: a broadcast address has been set.
    * `L`: this interface is a loopback device
    * `M`: all packets are received (promiscuous mode).
    * `N`: trailers are avoided.
    * `O`: ARP is turned off for this interface.
    * `P`: this is a point-to-point connection.
    * `R`: Interface is running.
    * `U`: Interface is up.

## show routes
```
─# netstat -r -n
Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
0.0.0.0         10.52.6.1       0.0.0.0         UG        0 0          0 eth0
10.52.6.0       0.0.0.0         255.255.254.0   U         0 0          0 eth0
168.63.129.16   10.52.6.1       255.255.255.255 UGH       0 0          0 eth0
169.254.169.254 10.52.6.1       255.255.255.255 UGH       0 0          0 eth0
172.17.0.0      0.0.0.0         255.255.0.0     U         0 0          0 docker0
192.168.122.0   0.0.0.0         255.255.255.0   U         0 0          0 virbr0
```

## show socket
### show raw socket
```
# netstat -wanp
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
raw6       0      0 :::58                   :::*                    7           778/systemd-network
```
### show tcp socket
* show all tcp sockets
```
# netstat -antp
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      1160/sshd: /usr/sbi
tcp        0      0 127.0.0.53:53           0.0.0.0:*               LISTEN      820/systemd-resolve
tcp        0      0 0.0.0.0:5355            0.0.0.0:*               LISTEN      820/systemd-resolve
tcp        0      0 127.0.0.1:38211         0.0.0.0:*               LISTEN      3817295/node
tcp        0      0 10.52.6.14:45150        168.63.129.16:80        TIME_WAIT   -
tcp        0      0 10.52.6.14:36852        40.112.242.80:443       ESTABLISHED 1364/mdsd
```
* show listening only
```
# netstat -lntp
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      1160/sshd: /usr/sbi
tcp        0      0 127.0.0.53:53           0.0.0.0:*               LISTEN      820/systemd-resolve
tcp        0      0 0.0.0.0:5355            0.0.0.0:*               LISTEN      820/systemd-resolve
tcp        0      0 127.0.0.1:38211         0.0.0.0:*               LISTEN      3817295/node
```
* `Recv-Q`:
    * `Established`: The count of bytes not copied by the user program connected to this socket.
    * `Listening`: Since Kernel 2.6.18 this column contains the current syn backlog.
* `Send-Q`:
    * `Established`: The count of bytes not acknowledged by the remote host.
    * `Listening`: Since Kernel 2.6.18 this column contains the maximum size of the syn backlog.

### show udp socket
```
# netstat -anup
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
udp        0      0 192.168.122.1:53        0.0.0.0:*                           1811/dnsmasq
udp        0      0 127.0.0.53:53           0.0.0.0:*                           820/systemd-resolve
udp        0      0 0.0.0.0:67              0.0.0.0:*                           1811/dnsmasq
udp        0      0 10.52.6.14:68           0.0.0.0:*                           778/systemd-network
udp        0      0 0.0.0.0:5355            0.0.0.0:*                           820/systemd-resolve
udp6       0      0 fe80::6245:bdff:fe0:546 :::*                                778/systemd-network
udp6       0      0 :::5355                 :::*                                820/systemd-resolve
```

## show statistics
* show all statistics
```
# netstat -s
```
* show tcp statistics
```
# netstat -st
```

