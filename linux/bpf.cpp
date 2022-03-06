#include <arpa/inet.h>
#include <linux/filter.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <malloc.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstring>
#include <iostream>
#include <memory>

// struct sockaddr_ll {
//     unsigned short sll_family;   /* Always AF_PACKET */
//     unsigned short sll_protocol; /* Physical-layer protocol */
//     int            sll_ifindex;  /* Interface number */
//     unsigned short sll_hatype;   /* ARP hardware type */
//     unsigned char  sll_pkttype;  /* Packet type */
//     unsigned char  sll_halen;    /* Length of address */
//     unsigned char  sll_addr[8];  /* Physical-layer address */
// };

const char INTF[] = "eth0";

struct sock_filter ICMP_FILTER_CODE[] = {
    {.code = 0x28, .jt = 0, .jf = 0, .k = 0x0000000c},
    {.code = 0x15, .jt = 0, .jf = 10, .k = 0x00000800},
    {.code = 0x20, .jt = 0, .jf = 0, .k = 0x0000001a},
    {.code = 0x15, .jt = 0, .jf = 8, .k = 0x00000000},
    {.code = 0x30, .jt = 0, .jf = 0, .k = 0x00000017},
    {.code = 0x15, .jt = 0, .jf = 6, .k = 0x00000001},
    {.code = 0x28, .jt = 0, .jf = 0, .k = 0x00000014},
    {.code = 0x45, .jt = 4, .jf = 0, .k = 0x00001fff},
    {.code = 0xb1, .jt = 0, .jf = 0, .k = 0x0000000e},
    {.code = 0x50, .jt = 0, .jf = 0, .k = 0x0000000e},
    {.code = 0x15, .jt = 0, .jf = 1, .k = 0x00000000},
    {.code = 0x6, .jt = 0, .jf = 0, .k = 0x00040000},
    {.code = 0x6, .jt = 0, .jf = 0, .k = 0x00000000},
};

int main()
{
    auto sock = socket(AF_PACKET, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_ICMP);
    if (sock < 0) {
        throw std::exception();
    }
    struct sockaddr_ll addr;
    std::memset(static_cast<void *>(&addr), 0, sizeof(addr));
    addr.sll_ifindex = if_nametoindex(INTF);
    addr.sll_family = AF_PACKET;
    addr.sll_protocol = htons(ETH_P_ALL);
    if (bind(sock, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        throw std::exception();
    }
    struct sock_fprog bpf = {
        .len = sizeof(ICMP_FILTER_CODE) / sizeof(sock_filter),
        .filter = ICMP_FILTER_CODE};
    if (setsockopt(sock, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf)) < 0) {
        throw std::exception();
    }
    return 0;
}