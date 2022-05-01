#include <errno.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <stdarg.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <set>
#include <string>
#include <vector>

const unsigned int PORTNUM = 8900;
const unsigned int BUFFLEN = 1024;

char ERROR_STR[100];
char BUFF[BUFFLEN];

void throw_error(const char *fmt, ...);
bool set_socket_blocking(int fd, bool blocking);
void epoll_register_fd(int epoll_fd, int fd);
void epoll_delete_fd(int epoll_fd, int fd);

int main()
{
    int epoll_fd;
    int server_fd;
    int client_fd;
    int poll_fd;
    int ret;
    int enable = 1;
    int i;
    struct sockaddr_storage peer_addr;
    struct sockaddr_in sock_addr;
    socklen_t peer_addr_len = sizeof(sockaddr_storage);
    std::vector<struct epoll_event> events;
    std::set<int> all_fds;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        throw_error("Error: Open socket(%d)", errno);
    }

    set_socket_blocking(server_fd, true);

    ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (ret == -1) {
        throw_error("Error: Set socket(%d)", errno);
    }

    sock_addr = {AF_INET, htons(PORTNUM), INADDR_ANY};
    ret = bind(server_fd, reinterpret_cast<sockaddr *>(&sock_addr), sizeof(sockaddr_in));
    if (ret == -1) {
        throw_error("Error: Bind socket(%d)", errno);
    }

    ret = listen(server_fd, SOMAXCONN);
    if (ret == -1) {
        throw_error("Error: Listen socket(%d)", errno);
    }

    epoll_fd = epoll_create1(0);
    epoll_register_fd(epoll_fd, server_fd);
    all_fds.insert(server_fd);
    EEXIST;
    while (all_fds.size() > 0) {
        std::vector<struct epoll_event> events(all_fds.size());
        ret = epoll_wait(epoll_fd, events.data(), events.size(), -1);
        if (ret == -1) {
            throw_error("Error: Epoll(%d)", errno);
        }
        for (i = 0; i < ret; ++i) {
            poll_fd = events[i].data.fd;
            if (poll_fd == server_fd) {
                client_fd = accept(server_fd, reinterpret_cast<sockaddr *>(&peer_addr), &peer_addr_len);
                if (client_fd == -1) {
                    throw_error("Error: Accept(%d)", errno);
                }
                set_socket_blocking(client_fd, true);
                epoll_register_fd(epoll_fd, client_fd);
                all_fds.insert(server_fd);
            } else {
                ret = recv(poll_fd, BUFF, BUFFLEN, 0);
                std::cout << "Receive " << ret << " bytes from clinet" << std::endl;
                if (ret <= 0) {
                    all_fds.erase(poll_fd);
                    // a closed descriptor will be removed from epoll waiting list automatically
                    // epoll_delete_fd(epoll_fd, poll_fd);
                    close(poll_fd);
                } else {
                    std::cout << BUFF << std::endl;
                    send(poll_fd, BUFF, ret, 0);
                }
            }
        }
    }

    return 0;
}

void throw_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(ERROR_STR, sizeof(ERROR_STR), fmt, ap);
    va_end(ap);
    std::cerr << ERROR_STR << std::endl;
    throw std::runtime_error(ERROR_STR);
}

bool set_socket_blocking(int fd, bool blocking)
{
    if (fd < 0) {
        return false;
    }
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return false;
    }
    flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
    return fcntl(fd, F_SETFL, flags) == 0;
}

void epoll_register_fd(int epoll_fd, int fd)
{
    if (fd < 0) {
        throw_error("Error: invalid fd(%d)", fd);
    }
    struct epoll_event ev = {
        .events = EPOLLIN,
        .data = {
            .fd = fd,
        },
    };
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw_error("Error: Failed to add fd(%d) to epoll(%d)", fd, errno);
    }
}

void epoll_delete_fd(int epoll_fd, int fd)
{
    if (fd < 0) {
        throw_error("Error: invalid fd(%d)", fd);
    }

    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        throw_error("Error: Failed to delete fd(%d) from epoll(%d)", fd, errno);
    }
}
