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

