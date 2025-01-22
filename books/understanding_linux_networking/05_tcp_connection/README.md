# tcp connection

## questions?
### what could possible goes wrong in three-way handshake?
* here are a checklist of issues that happen during TCP three way handshake:
    * client tcp port range is not sufficient
        * **symptom**: the client shall suffer from high cpu
        * **why**: as the tcp ports are running out, the client needs more CPU to find a free tcp port, as there are more hash collisions and spin-lock waiting.
        * **how to solve**:
            * increase `net.ipv4.ip_local_port_range`
            * enable `tcp_tw_reuse` and `tcp_tw_recycle`
            * reuse connections - use "long" connection
    * first `SYN` packet is dropped by server
        * **symptom**: the client will try to retransimit the `SYN` with backoff timer (1s, 2s, 4s, 8s...)
        * **why**: Linux drops `SYN` packet in the following two situations:
            * the listen queue is full and `tcp_syncookie` is not enabled
            * the accept queue is full
        * the client retransimission is controlled by `net.ipv4.tcp_syn_retries`
    * third `ACK` from client is dropped by server
        * **symptom**: the client becomes `ESTABLISHED` and any packets from client will be dropped by server; the server will try to retransmit the `SYNACK` packets with backoff timer
        * **why**: the accept queue is full
        * the `SYNACK` retransmission is controlled by `net.ipv4.tcp.tcp_synack_retries`

### how to fix the listen/accept queue overflow?
* considerations:
    * enable syncookie
    * increase listen/accept queue length
    * improve server performance to `accept` connections timely
    * enable `tcp_abort_on_overflow` to reset connections if listen/accept queue is full.
    * reuse connection - use "long" connection

### what is "Cannot assign requested adddress" error?
* a tcp connection between server and client, its 4-tuple is (server ip, server port, client ip, client port), only client port is chosen dynamically.
* the error implies the client cannot request an available client port.
* how to fix?
    * increase `ip_local_port_range`
    * decrease `tcp_max_tw_bucket`

## references
