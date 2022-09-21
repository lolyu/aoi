# netlink socket

## struct nl_sock
* socket allocation/free
```cpp
#include <netlink/socket.h>

struct nl_sock *nl_socket_alloc(void)
void nl_socket_free(struct nl_sock *sk)
```

* socket sequence increment/disable
```cpp
#include <netlink/socket.h>

unsigned int nl_socket_use_seq(struct nl_sock *sk);
void nl_socket_disable_seq_check(struct nl_sock *sk);
```

### port number
* the local port number uniquely identifies the socket and is used to address it
* port number has 32 bits
    * 22 bits as process ID
    * 10 bits random number allowing up to 1024 sockets per process
```cpp
#include <netlink/socket.h>

uint32_t nl_socket_get_local_port(const struct nl_sock *sk);
void nl_socket_set_local_port(struct nl_sock *sk, uint32_t port);
```

### peer port
```cpp
#include <netlink/socket.h>

uint32_t nl_socket_get_local_port(const struct nl_sock *sk);
void nl_socket_set_local_port(struct nl_sock *sk, uint32_t port);
```
### file descriptor
```cpp
#include <netlink/socket.h>

int nl_socket_get_fd(const struct nl_sock *sk);
int nl_socket_set_nonblocking(const struct nl_sock *sk);
```
* if a socket is used to only receive notifications, it is usually best to put the socket in non-blocking mode and periodically poll for new notifications.

## socket callback configuration
```cpp
#include <netlink/socket.h>

struct nl_cb *nl_socket_get_cb(const struct nl_sock *sk);
void nl_socket_set_cb(struct nl_sock *sk, struct nl_cb *cb);
int nl_socket_modify_cb(struct nl_sock *sk, enum nl_cb_type type, enum nl_cb_kind kind,
                        nl_recvmsg_msg_cb_t func, void *arg);
```
* callback kinds:
    * `NL_CB_DEFAULT`: default handler
    * `NL_CB_VERBOSE`: verbose handler, will print error message to stderr for invalid message received
    * `NL_CB_DEBUG`: decode and dump each message sent or received to the console
    * `NL_CB_CUSTOM`: user provided handler
* callback types:
    * `NL_CB_VALID`
    * `NL_CB_FINISH`
    * `NL_CB_OVERRUN`
    * `NL_CB_SKIPPED`
    * `NL_CB_ACK`
    * `NL_CB_MSG_IN`
    * `NL_CB_MSG_OUT`
    * `NL_CB_INVALID`
    * `NL_CB_SEQ_CHECK`
    * `NL_CB_SEND_ACK`
    * `NL_CB_DUMP_INTR`

### multicast example with callback
```cpp
#include <netlink/netlink.h>
#include <netlink/socket.h>
#include <netlink/msg.h>

#include <iostream>

// g++ netlink_intro.cpp $(pkg-config --cflags --libs libnl-3.0 libnl-genl-3.0)

int my_func(struct nl_msg *msg, void *arg)
{
    std::cout << "Got notification!" << std::endl;
    return 0;
}

int main()
{
    struct nl_sock *sock = nl_socket_alloc();

    nl_socket_disable_seq_check(sock);
    nl_socket_modify_cb(sock, NL_CB_VALID, NL_CB_CUSTOM, my_func, nullptr);

    nl_connect(sock, NETLINK_ROUTE);
    nl_socket_add_memberships(sock, RTNLGRP_LINK, 0);

    while (1)
    {
        nl_recvmsgs_default(sock);
    }

    nl_socket_free(sock);
    return 0;
}
```

## sending && receiving message

## references
* 
