# use netstat to debug server/client TCP connection

## list tcp/udp connection with pid
```
# netstat -antp
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
# netstat -antup
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
tcp        0      0 0.0.0.0:5355            0.0.0.0:*               LISTEN      762/systemd-resolve
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      988/sshd: /usr/sbin-
tcp        0    360 10.52.6.14:22           10.191.32.4:49843       ESTABLISHED 499282/sshd: lolv [
tcp        0      0 10.52.6.14:55956        20.60.168.1:443         ESTABLISHED 1117/mdsd
tcp        0      0 10.52.6.14:33724        168.63.129.16:80        TIME_WAIT   -
tcp6       0      0 :::5355                 :::*                    LISTEN      762/systemd-resolve
tcp6       0      0 :::22                   :::*                    LISTEN      988/sshd: /usr/sbin
udp        0      0 127.0.0.53:53           0.0.0.0:*                           762/systemd-resolve
udp        0      0 10.52.6.14:68           0.0.0.0:*                           757/systemd-network
udp        0      0 0.0.0.0:5355            0.0.0.0:*                           762/systemd-resolve
udp6       0      0 fe80::6245:bdff:fe0:546 :::*                                757/systemd-network
udp6       0      0 :::5355                 :::*                                762/systemd-resolve
```
* `Recv-Q` means the data in the TCP/IP receive buffer in bytes, but the application doesn't call `recv()` to copy it from the TCP/IP receive buffer to application buffer.
    * if the `Recv-Q` is high, it means the process is slow in responding
* `Send-Q` means the data is in the TCP/IP send buffer, but it is not sent or sent but not ACKed.
    * if the `Send-Q` is high, it means:
        * server network congestion
        * server performance issue
        * data packet flow control
