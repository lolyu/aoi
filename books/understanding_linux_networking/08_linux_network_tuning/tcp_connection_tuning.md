# TCP connection tuning

## TCP port range
* `net.ipv4.ip_local_port_range`
* `net.ipv4.tcp_tw_reuse`

## don't use `bind` in client
* the TCP `bind` ports cannot be reused by other TCP connections even if the 4-tuple is different.

## listen/accept queue overflow
* `net.ipv4.tcp_syncookies`
    * if enabled, Linux will not allocate any resources for the connection until it receives the final `ACK` packet from the client, which completes the handshake.
    * this is to prevent listen queue overflow

## reduce retries
* `net.ipv4.tcp_syn_retries`
* `net.ipv4.tcp_synack_retries`

## connection timeout in client code

## TCP fast open
* `net.ipv4.tcp_fastopen`
    * enable send/accept data in the opening TCP SYN packet.

## file limit
* `ulimit` `nofile`
* `fs.file-max`
* `fs.nr_open`

## use long connections instead of short connections

## TIME_WAIT
* `net.ipv4.tcp_tw_reuse`
* net.ipv4.tcp_max_tw_buckets
  
## references
* 
