# TCP

## TCP vs UDP
* TCP is **stream-oriented** protocol while UDP is **message oriented**

* TCP treats data it sends/receives as a continuous ordered flow of bytes.
    * how many times application calls `send` != TCP packet count != how many times the application calls `recv`
* how does TCP create the stream?
    * buffering
    * segmentation
    * ressembly

* UDP is message oriented.
    * one application writes, one datagram

<img width="1183" height="700" alt="image" src="https://github.com/user-attachments/assets/eed1b507-87b3-47ec-96a0-9b7014e88c1b" />

## TCP options
* TCP options are used to negotiate advanced TCP features

* TCP options from a TCP SYN:
    * MSS: MTU - 40 (20 IP header + 20 TCP header)
    * selective ACK
    * window scaling

<img width="1043" height="133" alt="image" src="https://github.com/user-attachments/assets/bba27187-3ab6-45ac-a3d9-272ede7584f6" />


## TCP `TIME_WAIT` status
* the finish initiator needs to wait for 2MSL (maximum segment life) before fully close, two purpose:
    * ensure the other side is fully closed (if the last `ACK` to the other side is lost, the other side will send `FIN,ACK`, so the local side is still able to process)
    * ensure the packets from a closed TCP session will not be received by the new TCP session (same 4-tuple)
* refer to [tcp states](https://github.com/lolyu/aoi/blob/master/books/understanding_linux_networking/05_tcp_connection/tcp_states.md)


## TCP `CLOSE_WAIT` status
* TCP `CLOSE_WAIT` means the remote side has closed its sie, but the local end is still waiting for its own application to shutdown.
* if a TCP connection is stuck in `CLOSE_WAIT` for a long time, it is a sign of an application-level problem.
    * for HTTP server, if the HTTP request client has closed the TCP connection, but the server is still busy processing the HTTP request, the TCP connection on the server will be stuck in `CLOSE_WAIT` for a while.
 
## TCP out-of-band (OOB)

## TCP SYN timeout
* initial timeout is 1 second with exponential backoff (2, 4, 6...)
* linux kernel parameter:
    * `tcp_syn_retries`
```
$ sysctl net.ipv4.tcp_syn_retries       
net.ipv4.tcp_syn_retries = 6
```

## TCP retranmission timeout (RTO)
* the TCP RTO controls how long the sender waits for an `ACK` before resending a data packet.
* the linux RTO is calculated based on RTT (round-trip time)
* but the RTO also has the following boundaries:
    * initial RTO: 1 second
    * minimum RTO: 200 ms
    * maximum RTO: 120 seconds
* how to check RTO statistics:
```
$ ss -ti                                                                127 ↵
State   Recv-Q   Send-Q       Local Address:Port           Peer Address:Port    Process
ESTAB   0        0               10.52.6.14:54146           20.66.7.133:https
	 cubic wscale:8,7 rto:202 rtt:1.14/0.701 ato:40 mss:1428 pmtu:1500 rcvmss:1406 advmss:1448 cwnd:10 bytes_sent:2118 bytes_acked:2119 bytes_received:4639 segs_out:11 segs_in:8 data_segs_out:5 data_segs_in:6 send 100Mbps lastsnd:23360 lastrcv:23288 lastack:23288 pacing_rate 200Mbps delivery_rate 47Mbps delivered:6 app_limited busy:2ms rcv_rtt:2 rcv_space:14480 rcv_ssthresh:75298 minrtt:0.556
ESTAB   0        0               10.52.6.14:47184          52.239.228.4:https
	 cubic wscale:8,7 rto:202 rtt:1.492/1.195 ato:40 mss:1428 pmtu:1500 rcvmss:1324 advmss:1448 cwnd:10 bytes_sent:3234 bytes_acked:3235 bytes_received:6423 segs_out:10 segs_in:10 data_segs_out:5 data_segs_in:8 send 76.6Mbps lastsnd:23382 lastrcv:23374 lastack:23374 pacing_rate 153Mbps delivery_rate 40.5Mbps delivered:6 app_limited busy:7ms rcv_rtt:5 rcv_space:14480 rcv_ssthresh:78214 minrtt:0.565
ESTAB   0        0               10.52.6.14:40178       169.254.169.254:http
	 cubic wscale:8,7 rto:203 rtt:2.075/3.157 ato:40 mss:1448 pmtu:1500 rcvmss:1448 advmss:1448 cwnd:10 bytes_sent:160 bytes_acked:161 bytes_received:2969 segs_out:6 segs_in:6 data_segs_out:1 data_segs_in:3 send 55.8Mbps lastsnd:84436 lastrcv:84424 lastack:23820 pacing_rate 112Mbps delivery_rate 940kbps delivered:2 app_limited busy:12ms rcv_rtt:12 rcv_space:14480 rcv_ssthresh:70026 minrtt:0.612
ESTAB   0        208             10.52.6.14:ssh            10.191.32.18:60739
	 cubic wscale:6,7 rto:482 rtt:207.951/50.132 ato:40 mss:1298 pmtu:1500 rcvmss:1348 advmss:1448 cwnd:10 bytes_sent:11494 bytes_acked:11286 bytes_received:5585 segs_out:107 segs_in:149 data_segs_out:92 data_segs_in:58 send 499kbps lastsnd:3 lastrcv:4 lastack:4 pacing_rate 999kbps delivery_rate 625kbps delivered:90 app_limited busy:9987ms unacked:3 rcv_space:14600 rcv_ssthresh:72207 minrtt:160.864
```
* linux kernel parameters:
    * `tcp_retries1`
        * a limit number of retransmissions before the kernel alerts the upper network protocols
    * `tcp_retries2`
        * a limit number of retransmissions before the kernel terminates the connection

```
$ sysctl net.ipv4.tcp_retries1                                                                             
net.ipv4.tcp_retries1 = 3
$ sysctl net.ipv4.tcp_retries2
net.ipv4.tcp_retries2 = 15
```
    
