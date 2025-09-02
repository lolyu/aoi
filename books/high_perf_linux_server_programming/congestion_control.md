# TCP congestion control
* four topics:
    * slow start
    * congestion control
    * fast retransmit
    * fast recovery

## TCP congestion control vs sliding window
* TCP congestion control is to prevent the sender from overwhelming the network
* sliding window is to prevent the sender from overwhelming the sender

* terms:
    * RWND: receiver window - value from receiver to the sender to indicate how much buffer is available
    * CWND: congestion window - value maintained by the sender to gauge how much traffic the network can handle without congestion
    * SWND: sender window - actual sender window size
        * **`SWND = min(RWND, CWND)`**


## congestion control
* slow start:
    * for every `ACK` received, the `CWND` increases by one `MSS`
        * initialization: when a TCP connection is established, `CWND` is set to a small value
        * exponential growth: during the slow start, `CWND` doubles for every RTT

* check initial `CWND` for TCP connection:

```
$ ss -lnit
State                             Recv-Q                            Send-Q                                                       Local Address:Port                                                        Peer Address:Port                            Process
LISTEN                            0                                 10                                                                 0.0.0.0:5000                                                             0.0.0.0:*
	 cubic cwnd:10
LISTEN                            0                                 3                                                                127.0.0.1:2601                                                             0.0.0.0:*
	 cubic cwnd:10
LISTEN                            0                                 2                                                                127.0.0.1:2620                                                             0.0.0.0:*
	 cubic cwnd:10
```

* congestion control


<img width="976" height="561" alt="image" src="https://github.com/user-attachments/assets/fde0bb4a-5659-4c4c-81d8-20b0e70d4507" />



## references
* https://datatracker.ietf.org/doc/html/rfc5681
