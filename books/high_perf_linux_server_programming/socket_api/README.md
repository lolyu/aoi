# socket API

## common headers

* `#include <sys/socket.h>`: This is the main header. It defines the core socket functions and data structures like `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, and the `sockaddr` structure.
* `#include <netinet/in.h>`: Defines the internet address family structures like `sockaddr_in` (for IPv4) and `sockaddr_in6` (for IPv6), along with constants like `AF_INET` and `AF_INET6`.
* `#include <arpa/inet.h>`: Provides functions for converting internet addresses between text and binary formats, such as `inet_pton()` and `inet_ntop()`.
* `#include <unistd.h>`: Contains definitions for standard symbolic constants and types, including the essential `close()`, `read()`, and `write()` functions.
* `#include <netdb.h>`: Defines functions for network database operations, most importantly `getaddrinfo()` for hostname and service name resolution.


## references
