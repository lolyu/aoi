# socket option

```cpp
#include <sys/socket.h>

int getsockopt(int sockfd, int level, int option_name, void *option_value, socklen_t *restrict option_len);
int setsockopt(int sockfd, int level, int option_name, const void *option_value, socklen_t *restrict option_len);
```
* `level` specifies the protocol level:
    * `SOL_SOCKET`
    * `IPPROTO_IP`
    * `IPPROTO_IPV6`
    * `IPPROTO_TCP`

## `SO_REUSEADDR`
* allow reuse of addresses of `TIME_WAIT` TCP socket.
* must be set prior to `bind`

## `SO_SNDBUF` and `SO_RCVBUF`
* `SO_SNDBUF`: socket send buffer size
* `SO_RCVBUF`: socket receive buffer size

## references
* 
