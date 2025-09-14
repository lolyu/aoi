# socket API

## socket
```cpp
#include <sys/types.h>
#include <sys/socket.h>
int socket(int domain, int type, int protocol)
```

## bind
```cpp
#include <sys/types.h>
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr* my_addr, socklen_t addrlen);
```

## listen
```cpp
#include <sys/socket.h>
int listen(int sockfd, int backlog);
```

## accept
```cpp
#include <sys/types.h>
#include <sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```
* for the TCP connection in the accept queue, even the client side closes the connection, the connection is still able to be **accepted**.

## connect
```cpp
#include <sys/types.h>
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr* serv_addr, socklen_t addrlen);
```

## close
```cpp
#include <unistd.h>
int close(int fd);
```
* `close` doesn't close the connection immediately, it simply reduce the socket fd reference counter.

## shutdown
```cpp
#include <sys/socket.h>
int shutdown(int sockfd, int howto)
```
* `howto`:
    * `SHUT_RD`: shutdown the read of `sockfd`, all data in the socket receive buffer is flushed
    * `SHUT_WR`: shutdown the write of `sockfd`, all data in the socket send buffer is sent, and the TCP connection is half-closed.
    * `SHUT_RDWR`: shutdown both read && write of `sockfd`

##  recv && send
```cpp
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recv(int sockfd, void *buf, size_t len, int flags)
```

* Flags:

|flag|note|`send`|`recv`|
|-|-|-|-|
|`MSG_CONFIRM`|tell the link layer that this packet is a reply from the other side to prevent the link layer to regularly reprobe the neighbor. Valid only `SOCK_DGRAM` and `SOCK_RAW`|Y|N|
|`MSG_DONTROUTE`|send data without look up the routing table, used to send packet to a local connected network.|Y|N|
|`MSG_DONTWAIT`|make a single socket operation non-blocking(`O_NONBLOCK` is socket level).|Y|Y|
|`MSG_MORE`|<br>The caller has more data to send. This flag is used with TCP sockets to obtain the same effect as the TCP_CORK socket option. Kernel will group multiple send calls into a single/larger packet.</br><br>When using `MSG_MORE`, kernel will holds the data into the buffer, it won't send the packet till:</br><br>1. final `send()` call without `MSG_MORE` flag</br><br>2. socket buffer becomes full.</br>|Y|N|
|`MSG_WAITALL`|This flag requests that the operation block until the full request is satisfied|N|Y|
|`MSG_PEEK`|This flag causes the receive operation to return data from the beginning of the receive queue without removing that data from the queue. Thus, a subsequent receive call will return the same data.|N|Y|
|`MSG_OOB`|send/receive OOB|Y|Y|
|`MSG_NOSIGNAL`||Y|N|

## references
* https://man7.org/linux/man-pages/man2/send.2.html
* https://man7.org/linux/man-pages/man2/recv.2.html
