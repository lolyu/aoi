# socket address

```cpp
#include <bits/socket.h>

typedef unsigned short int sa_family_t;

struct sockaddr
{
    sa_family_t sa_prefix;        /* Common data: address family and length.  */
    char sa_data[14];            /* Address data.  */
};

struct sockaddr_storage
{
    sa_family_t sa_prefix;	        /* Address family, etc.  */
    char __ss_padding[128 - sizeof (unsigned short int) - sizeof (unsigned long int)];
    unsigned long int __ss_align;	/* Force desired alignment.  */
};
```

* why `sockaddr_storage` is needed?
    * Different network protocols use different structures to store address information, and these structures are different sizes:
        * unix: Uses `struct sockaddr_un`
        * IPv4: Uses `struct sockaddr_in`
        * IPv6: Uses `struct sockaddr_in6` (which is larger than `sockaddr_in`)
    * If you write a server and hardcode `struct sockaddr`_in to accept a connection, it won't work with an IPv6 client because the structure is too small to hold the IPv6 address.
    * struct sockaddr_storage solves this by providing a "one-size-fits-all" data type guaranteed by the system to be large enough and properly aligned for the largest supported address structure.



```cpp
#include <sys/socket.h>
#include <type_traits>
#include <iostream>

int main()
{
    struct sockaddr_storage ss;

    std::cout << sizeof(ss) << std::endl;                                   // 128
    std::cout << alignof(struct sockaddr_storage) << std::endl;             // 8
    std::cout << offsetof(sockaddr_storage, ss_family) << std::endl;        // 0
    std::cout << offsetof(sockaddr_storage, __ss_padding) << std::endl;     // 2
    std::cout << offsetof(sockaddr_storage, __ss_align) << std::endl;       // 120

    return 0;
}
```

```
// Internet domain sockets
#include <netinet/in.h>

struct sockaddr_in {
   sa_family_t     sin_family;     /* AF_INET */
   in_port_t       sin_port;       /* Port number */
   struct in_addr  sin_addr;       /* IPv4 address */
};

struct sockaddr_in6 {
   sa_family_t     sin6_family;    /* AF_INET6 */
   in_port_t       sin6_port;      /* Port number */
   uint32_t        sin6_flowinfo;  /* IPv6 flow info */
   struct in6_addr sin6_addr;      /* IPv6 address */
   uint32_t        sin6_scope_id;  /* Set of interfaces for a scope */
};

struct in_addr {
   in_addr_t s_addr;
};

struct in6_addr {
   uint8_t   s6_addr[16];
};

typedef uint32_t in_addr_t;
typedef uint16_t in_port_t;

// UNIX domain sockets
#include <sys/un.h>

struct sockaddr_un {
   sa_family_t     sun_family;     /* Address family */
   char            sun_path[108];     /* Socket pathname */
};
```

## address conversion

```cpp
#include <iostream>
#include <arpa/inet.h>
#include <string>

bool is_valid(in_addr_t addr)
{
    // INADDR_NONE is 0xffffffff
    return addr != INADDR_NONE;
}

int main()
{
    // convert ipv4 address string to network byte order
    // return INADDR_NONE if the string is not a valid addr
    in_addr_t addr0 = inet_addr("192.168.0.1");
    std::cout << addr0 << std::endl;
    std::cout << is_valid(addr0) << std::endl;

    // convert ipv4 address string to network byte order
    // return 1 if success, 0 otherwise
    in_addr addr1;
    auto res = inet_aton("192.168.0.1", &addr1);
    if (res)
    {
        std::cout << addr1.s_addr << std::endl;
        std::cout << is_valid(addr1.s_addr) << std::endl;
    }

    // convert network byte order address to string
    char *addr = inet_ntoa(addr1);
    std::cout << addr << std::endl;
    addr = inet_ntoa({INADDR_NONE});
    std::cout << addr << std::endl;
    addr = inet_ntoa({INADDR_ANY});
    std::cout << addr << std::endl;

    return 0;
}
```

## `inet_pton` and `inet_ntop`
```cpp
#include <arpa/inet.h>

int inet_pton(int af, const char *src, void *dst);
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
```

```cpp
#include <iostream>
#include <arpa/inet.h>

char ipv4_addr[INET_ADDRSTRLEN];
char ipv6_addr[INET6_ADDRSTRLEN];

int main()
{
    struct sockaddr_in addr;
    auto res = inet_pton(AF_INET, "192.168.0.1", &(addr.sin_addr));
    if (res == 1)
    {
        std::cout << "Converted string address!" << std::endl;
    }
    else if (res == 0)
    {
        std::cout << "Failed to convert string address!" << std::endl;
    }
    else
    {
        std::cout << "Function inet_pton failed" << std::endl;
    }

    auto addr_str = inet_ntop(AF_INET, &(addr.sin_addr), ipv4_addr, INET_ADDRSTRLEN);
    std::cout << addr_str << std::endl;

    struct sockaddr_in6 addr_ipv6;
    res = inet_pton(AF_INET6, "2001:0db8:85a3:0000:0000:8a2e:0370:7334", &(addr_ipv6.sin6_addr));
    if (res == 1)
    {
        std::cout << "Converted string address!" << std::endl;
    }
    else if (res == 0)
    {
        std::cout << "Failed to convert string address!" << std::endl;
    }
    else
    {
        std::cout << "Function inet_pton failed" << std::endl;
    }

    auto addr_ipv6_str = inet_ntop(AF_INET6, &(addr_ipv6.sin6_addr), ipv6_addr, INET6_ADDRSTRLEN);
    std::cout << addr_ipv6_str << std::endl;

    return 0;
};
```
