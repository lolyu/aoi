#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <cassert>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " addr port" << std::endl;
        exit(EXIT_FAILURE);
    }

    int res;
    const char *ip = argv[1];
    const int port = atoi(argv[2]);

    struct sockaddr_in address{0};
    address.sin_family = AF_INET;
    res = inet_pton(AF_INET, ip, &(address.sin_addr));
    address.sin_port = htons(port);
    assert(res > 0);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    res = bind(sock, reinterpret_cast<struct sockaddr *>(&address), sizeof(address));
    assert(res != -1);

    res = listen(sock, 5);
    assert(res != -1);

    sleep(10);
    struct sockaddr_in client_address{0};
    socklen_t client_address_len;
    int connfd = accept(sock, reinterpret_cast<struct sockaddr *>(&client_address), &client_address_len);
    if (connfd < 0)
    {
        std::cerr << "errno is: " << errno << std::endl;
    }
    else
    {
        char client_addr_str[INET_ADDRSTRLEN];
        short client_port = ntohs(client_address.sin_port);
        inet_ntop(AF_INET, &(client_address.sin_addr), client_addr_str, INET_ADDRSTRLEN);
        std::cout << "Client is connected, address: " << client_addr_str << ", port: " << client_port << "." << std::endl;
    }

    return 0;
}
