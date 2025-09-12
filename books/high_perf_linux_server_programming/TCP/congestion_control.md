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
    * `RWND`: receiver window - value from receiver to the sender to indicate how much buffer is available
    * `CWND`: congestion window - value maintained by the sender to gauge how much traffic the network can handle without congestion
    * `SWND`: sender window - actual sender window size, which determines how many packets the sender is allowed to send before receiving an ACK.
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

* congestion avoidance
	* when `CWND` reaches `ssthresh` (slow start threshold), `CWND` will grow linearly to provide an acceptable approximation.

<img width="976" height="561" alt="image" src="https://github.com/user-attachments/assets/fde0bb4a-5659-4c4c-81d8-20b0e70d4507" />

* congestion detection:
	* retran timeout
 	* duplicated `ACK`
		* when a packet is lost, the receiver will continue to send `ACK`s for the last in-order packet it received, and the sender will assume the packet is lost after receiving three duplicate `ACK`s (4 in total).

* fast retransmit:
	* when a TCP cannot receives a packet, but a subsequent packet is received, the receiver will send a duplicate `ACK`.
	* the Fast Retransmit algorithm is triggered when a sender receives three duplicate ACKs for the same packet. This is a strong indication to the sender that the packet immediately following the acknowledged one has been lost. Instead of waiting for a potentially long retransmission timeout, the sender immediately retransmits the missing packet.

* **when a TCP sender detects segment loss with retran timeout:**
	* `ssthresh = max(FlightSize/2, 2*MSS)`
	* `CWND` is reset to slow start initial value.
 	* sender enters slow start phase
* **when a TCP sender detects a segment loss with fast retransmit:**
	* `ssthresh = max(FlightSize/2, 2*MSS)`
 	* `CWND = ssthresh`
	* the sender enters congestion avoidance phase, bypass slow start.

## references
* https://datatracker.ietf.org/doc/html/rfc5681
