#include <errno.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <stdarg.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <set>
#include <string>

const unsigned int PORTNUM = 8900;
const unsigned int BUFFLEN = 1024;

char ERROR_STR[100];
char BUFF[BUFFLEN];

void throw_error(const char *fmt, ...);
inline int find_max_fd(int server_fd, std::set<int> client_fds);
bool set_socket_blocking(int fd, bool blocking);

int main()
{
    int fd;
    int ret;
    int enable = 1;
    int max_fd;
    struct sockaddr_storage peer_addr;
    struct sockaddr_in sock_addr;
    socklen_t peer_addr_len = sizeof(sockaddr_storage);
    std::set<int> client_fds;
    fd_set rfds;
    struct timeval tv;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        throw_error("Error: Open socket(%d)", errno);
    }

    set_socket_blocking(fd, true);

    ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (ret == -1) {
        throw_error("Error: Set socket(%d)", errno);
    }

    sock_addr = {AF_INET, htons(PORTNUM), INADDR_ANY};
    ret = bind(fd, reinterpret_cast<sockaddr *>(&sock_addr), sizeof(sockaddr_in));
    if (ret == -1) {
        throw_error("Error: Bind socket(%d)", errno);
    }

    ret = listen(fd, SOMAXCONN);
    if (ret == -1) {
        throw_error("Error: Listen socket(%d)", errno);
    }

    tv.tv_sec = 2;
    tv.tv_usec = 0;

    while (true) {
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        for (auto client_fd : client_fds) {
            FD_SET(client_fd, &rfds);
        }
        max_fd = find_max_fd(fd, client_fds);
        ret = select(max_fd + 1, &rfds, nullptr, nullptr, nullptr);
        if (ret == -1) {
            throw_error("Error: Select(%d)", errno);
        }

        for (auto it = client_fds.begin(); it != client_fds.end();) {
            if (FD_ISSET(*it, &rfds)) {
                ret = recv(*it, BUFF, BUFFLEN, 0);
                std::cout << "Receive " << ret << " bytes from clinet" << std::endl;
                if (ret == 0) {
                    close(*it);
                    it = client_fds.erase(it);
                } else if (ret < 0) {
                    throw_error("Error: Recv(%d)", errno);
                } else {
                    std::cout << BUFF << std::endl;
                    send(*it, BUFF, ret, 0);
                    ++it;
                }
            } else {
                ++it;
            }
        }

        if (FD_ISSET(fd, &rfds)) {
            std::cout << "Accept new connection" << std::endl;
            int new_client = accept(fd, reinterpret_cast<sockaddr *>(&peer_addr), &peer_addr_len);
            if (new_client == -1) {
                throw_error("Error: Accept(%d)", errno);
            }
            set_socket_blocking(new_client, true);
            client_fds.insert(new_client);
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

int find_max_fd(int server_fd, std::set<int> client_fds)
{
    int max_fd = server_fd;
    for (auto client_fd : client_fds) {
        max_fd = std::max(max_fd, client_fd);
    }
    return max_fd;
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
