# TCP states

![image](https://github.com/user-attachments/assets/63da01e4-b870-49ca-b14b-91cf993d0267)


## three way handshake

![image](https://github.com/user-attachments/assets/6b88da35-44e3-4945-be3c-fd9b074cbf8a)

1. the server changes status to `LISTEN`
2. the client sends a syn packet with `SYN=1` and `seq=x`, and changes status to `SYN-SENT`
3. the server responds to the syn packet:
    * if the server accepts the connection:
        * sends back a confirm response with `SYN=1, ACK=1` and `seq=y, ack=x+1`, changes status to `SYN-RCVD` 
    * if not:
        * sends back a `RST` packet
4. client receives the server response, sends back a confirm response with `ACK=1` and `seq=x+1, ack=y+1`

## four way handshake
```
      TCP A                                                TCP B

  1.  ESTABLISHED                                          ESTABLISHED

  2.  (Close)
      FIN-WAIT-1  --> <SEQ=100><ACK=300><CTL=FIN,ACK>  --> CLOSE-WAIT

  3.  FIN-WAIT-2  <-- <SEQ=300><ACK=101><CTL=ACK>      <-- CLOSE-WAIT

  4.                                                       (Close)
      TIME-WAIT   <-- <SEQ=300><ACK=101><CTL=FIN,ACK>  <-- LAST-ACK

  5.  TIME-WAIT   --> <SEQ=101><ACK=301><CTL=ACK>      --> CLOSED

  6.  (2 MSL)
      CLOSED                                                      
```

### what is the purpose of `TIME_WAIT`?
* only the local side that wants to close the connection transits into `TIME_WAIT`, which will lasts `2MSL` till entering into `CLOSE`.
* two purposes of `TIME_WAIT`:
    1. **ensure the remote end has closed the connection**
        * if the last `ACK` is lost, the remote end stays in `LAST-ACK` and retries sending `FIN`; if the local side is in `CLOSE`, it will respond with a `RST` packet instead of `ACK`.
    2. **prevent delayed segments from previouse connection being accepted by a later connection with same quadruplet**

![image](https://github.com/user-attachments/assets/d90e6e73-cda7-4f76-bc33-0771e97173b6)


#### `net.ipv4.tcp_tw_reuse`
* The `TIME-WAIT` state prevents delayed segments to be accepted in an unrelated connection. However, in certain conditions, it is possible to assume a new connection’s segment cannot be misinterpreted with an old connection’s segment.
* `tcp_tw_reuse` allows the reuse of a client port immediately after the connection has been closed, even though the connection is still in `TIME_WAIT` state. A precondition for it to take effect is that TCP timestamps are enabled, i.e. `net.ipv4.tcp_timestamps`  = 1

## references
* https://www.rfc-editor.org/rfc/rfc793.txt
* https://vincent.bernat.ch/en/blog/2014-tcp-time-wait-state-linux
