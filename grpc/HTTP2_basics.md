# HTTP/2 basics

* characteristics
    * binary-based instead of text-based
    * mutiplexed
 
* gRPC could work over HTTP/2, Cronet, and in-process.
* HTTP/2 terms
    * HTTP/2, all communications are performed over a single TCP connection that can carry any number of bidirectional flows of bytes
    * stream: a bidirectional flow of bytes within an established connection, a stream can carry one or more messages
    * message: a complete sequence of frames that map to a logical request or response message
    * frame: smallest unit to send/receive over a stream
* relationships of the above terms:
    * All communication is performed over a single TCP connection that can carry any number of bidirectional streams.
    * Each stream has a unique identifier and optional priority information that is used to carry bidirectional messages.
    * Each message is a logical HTTP message, such as a request, or response, which consists of one or more frames.
    * The frame is the smallest unit of communication that carries a specific type of data—e.g., HTTP headers, message payload, and so on. Frames from different streams may be interleaved and then reassembled via the embedded stream identifier in the header of each frame.

* HTTP/2 summary:
    * In short, HTTP/2 breaks down the HTTP protocol communication into an exchange of binary-encoded frames, which are then mapped to messages that belong to a particular stream, all of which are multiplexed within a single TCP connection.
* in gRPC:
    * gRPC channel represents a HTTP/2 TCP connection
    * each RPC call is a stream
    * a gRPC message is a HTTP/2 message, which might span multiple HTTP/2 frames
 

## multiplexed
![image](https://user-images.githubusercontent.com/35479537/176356114-9092a0bb-9d8d-4591-bd10-d63f92677e68.png)
* HTTP/1
    * one TCP connection one HTTP request
 
 ![image](https://user-images.githubusercontent.com/35479537/176356192-e8a23966-7a6f-41d0-bcdc-428839bbd20a.png)



## references
* 
