#include <iostream>
#include <cassert>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

bool stop = false;

void handle_term(int signal)
{
    stop = true;
}

int main(int argc, char *argv[])
{

    signal(SIGTERM, handle_term);

    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " ip port backlog" << std::endl;
        exit(EXIT_FAILURE);
    }

    int ret;
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);
    std::cout << "IP: " << ip << ", Port: " << port << ", Backlog: " << backlog << std::endl;

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock > 0);

    struct sockaddr_in address{0};
    address.sin_family = AF_INET;
    ret = inet_pton(AF_INET, ip, &(address.sin_addr));
    assert(ret > 0);
    address.sin_port = htons(port);

    ret = bind(sock, reinterpret_cast<struct sockaddr*>(&address), sizeof(address));
    assert(ret != -1);

    ret = listen(sock, backlog);
    assert(ret != -1);

    while (!stop)
    {
        sleep(1);
    }

    close(sock);
    return 0;
}
