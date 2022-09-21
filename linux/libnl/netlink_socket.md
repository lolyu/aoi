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

### socket callback configuration
```cpp
#include <netlink/socket.h>

struct nl_cb *nl_socket_get_cb(const struct nl_sock *sk);
void nl_socket_set_cb(struct nl_sock *sk, struct nl_cb *cb);
int nl_socket_modify_cb(struct nl_sock *sk, enum nl_cb_type type, enum nl_cb_kind kind,
                        nl_recvmsg_msg_cb_t func, void *arg);
```

#### multicast example with callback
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

## references
* 
