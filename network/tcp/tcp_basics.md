# tcp basics

## connection termination
* four way handshake

![image](https://user-images.githubusercontent.com/35479537/235894699-dcb99567-aa28-4097-85e5-5d79cdffa0e3.png)

* after the client sent the final `ACK`, it waits for a timeout(2 `MSS`(maximum segment lifetime)) before finally closing the connection, during which time the local port is unavailable for new connections

### the purpose of the final `TIME_WAIT`
* the active closer does wait for 2 `MSS`, this is to ensure that there is no `FIN` received from the passive closer anymore.
* If the passive closer(`LAST_ACK`) doesn't receive the last `ACK`(there is a timeout), it will resend `FIN`, and the active closer should resent the last `ACK`


## reliable delivery
* TCP achieves reliable delivery using a technique known as `positive acknowledgement with re-transmission`. This requires the receiver to respond with an acknowledgement message as it receives the data. The sender keeps a record of each packet it sends and maintains a timer from when the packet was sent. The sender re-transmits a packet if the timer expires before receiving the acknowledgement. The timer is needed in case a packet gets lost or corrupted.

### retransmission

### timeout-based retransmission
* retransimission timeout


### dupack-based retransmission
* duplicate cumulative acknowlegdements
* also called fast retransmit
* If a single segment (say segment number 100) in a stream is lost, then the receiver cannot acknowledge packets above that segment number (100) because it uses cumulative ACKs. Hence the receiver acknowledges packet 99 again on the receipt of another data packet (101). This duplicate acknowledgement is used as a signal for packet loss.
* when the sender receives three duplicate ACKs, it retransmits the last unacknowledged packet

## sliding window
### protocol operation
* terms:
    * Wt: transmitter side window size
    * Wr: receiver side window size
    * Nt: first packet not yet transmitted
    * Na: highest ackownledgement received
    * Nr: first packet not yet received
        * all packets < Nr have been received
        * when the receiver receives a packet, it transmits an ACK with Nr
    * Ns: one more than the sequence number of the highest sequence number received
        * no packets >= Ns have been received
        * some packets between (Nr, Ns) have been received
* Na <= Nr <= Ns <= Nt <= Na + Wt
    * Na <= Nr:
        * there must be some ACKs on-the-fly, sent by the receiver but not received by the transmitter yet
    * Nr <= Ns
        * the span of fully received packets cannot extend beyond the end of the partially received packets.
    * Ns <= Nt
        * there must be some packets on-the-fly, sent by the transmitter but not received by the receiver yet
    * Nt <= Na + Wt
        * the span of fully received packets cannot extend beyond the end of the partially received packets.
* transmitter operation:
    * transmitter is only allowed to send packets with sequence number Nt < Na + Wt
* receiver operation
    * when a packet with sequence number X is received:
        * if falls within the receive window: [Nr, Nr + Wr)
            * if yes:
                * if X == Nr:
                    * increase Nr by one, or more if further consecutive packets were previously received and stored
                * if X > Nr:
                    * the packet is stored until all preceding packets have been received
                * if X >= Ns:
                    * Ns = X + 1
            * if no:
                * NOOP
        * transmit an ACK with Nr
* **NOTE**: there is no point having the receive window Wr larger than the transmit windwo Wt, because there is no need to worry about receiving a packet not transmitted


## congestion control
* TCP detects congestion when it fails to receive an acknowledgement for a packet within the estimated timeout.


### AIMD
* AIMD: additive increase/multiplicative decrease
* let `w(t)` be the congestion window size indicating the amount of data in flight during time slot `t`, `a`(`a` > 0) be the additive increase parameter, and `b`(0 < `b` < 1) be the multiplicative decrease factor
    * `w(t+1) = w(t) + a` if congestion is not detected
    * `w(t+1) = w(t) * b` if congestion is detected


### slow start
* slow starts begins initially with a congestion window size(CWND) of 1 MSS, the value for the CWND can be increased by 1 MSS with each acknowledgement(ACK) received, effectively doubling the window size each RTT.
* if CWND reaches slow start threshold(ssthresh, used to determine whether the slow start or congestion avoidance algorithm is used), TCP changes to congestion avoidance algorithm: increase 1 MSS for each RTT
* When a TCP sender detects segment loss using the retransmission timer and the given segment has not yet been resent by way of the retransmission timer, the value of ssthresh must be set to:
    * TCP Tahoe: when a loss occurs, retransmit is sent, half of the current CWND is saved as sshthresh and slow start begins again from its initial CWND
    * TCP Reno: when a loss occurs, a fast retransmit is sent, half of the current CWND is saved as ssthresh and as new CWND, thus skipping slow start and going directly to the congestion avoidance algorithm, the overall algorithm is called fast recovery


![image](https://user-images.githubusercontent.com/35479537/236131835-79ffda6d-4529-4f02-9dba-63faec3a6072.png)
