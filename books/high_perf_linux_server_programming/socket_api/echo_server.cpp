#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <cassert>
#include <string>
#include <tuple>

const size_t BUFFSIZE = 4096;
using IPv4SockAddr = std::tuple<std::string, short>;
enum IPv4SockAddrFields
{
    Ipv4Addr,
    Port
};

IPv4SockAddr get_sockaddr(const struct sockaddr_in &addr)
{
    static char ip_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip_addr, INET_ADDRSTRLEN);
    return {std::string(ip_addr), ntohs(addr.sin_port)};
}

void check_sock_op(int ret, const char *msg)
{
    if (ret < 0)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

void handle_client(int clientfd)
{
    static char buffer[BUFFSIZE];
    size_t bytes_received;

    struct msghdr msg;
    struct iovec iov[1];

    while (true)
    {
        memset(&msg, 0, sizeof(msg));
        memset(iov, 0, sizeof(iov));
        memset(buffer, 0, BUFFSIZE);

        iov->iov_base = buffer;
        iov->iov_len = BUFFSIZE;
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;

        bytes_received = recvmsg(clientfd, &msg, 0);
        check_sock_op(bytes_received, "recvmsg");
        if (bytes_received == 0)
        {
            std::cout << "client disconnected." << std::endl;
            break;
        }

        std::cout <<"client received " << bytes_received << " bytes: " << buffer << std::endl;

        iov->iov_len = bytes_received;
        check_sock_op(sendmsg(clientfd, &msg, 0), "sendmsg");
    }
}

int main(int argc, char *argv[])
{
    int ret;
    int sock;
    int clientfd;
    char *addr;
    int port;
    socklen_t client_addr_len;
    struct sockaddr_in server_addr{0};
    struct sockaddr_in client_addr{0};

    if (argc != 3)
    {
        std::cerr << argv[0] << " ip_address port" << std::endl;
        exit(EXIT_FAILURE);
    }

    addr = argv[1];
    port = atoi(argv[2]);
    ret = inet_pton(AF_INET, addr, &(server_addr.sin_addr));
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;
    if (ret != 1)
    {
        std::cerr << argv[0] << ": invalid ip address!" << std::endl;
        exit(EXIT_FAILURE);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    check_sock_op(sock, "socket");

    ret = bind(sock, reinterpret_cast<struct sockaddr *>(&server_addr), sizeof(server_addr));
    check_sock_op(ret, "bind");

    auto server_ipv4_addr = get_sockaddr(server_addr);
    std::cout << "Connected to address <" << std::get<Ipv4Addr>(server_ipv4_addr) << ", " << std::get<Port>(server_ipv4_addr) << ">." << std::endl;

    ret = listen(sock, 5);
    check_sock_op(ret, "listen");

    clientfd = accept(sock, reinterpret_cast<struct sockaddr *>(&client_addr), &client_addr_len);
    check_sock_op(ret, "accept");

    auto client_ipv4_addr = get_sockaddr(client_addr);
    std::cout << "Client address <" << std::get<Ipv4Addr>(client_ipv4_addr) << ", " << std::get<Port>(client_ipv4_addr) << ">." << std::endl;

    handle_client(clientfd);

    close(sock);
    return 0;
}
