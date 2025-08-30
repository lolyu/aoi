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
