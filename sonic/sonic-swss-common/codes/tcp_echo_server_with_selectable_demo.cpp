#include <iostream>
#include <memory>
#include <swss/select.h>
#include <swss/selectable.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <array>
#include <map>

#define ERROR_THROW(msg) ({                                                                        \
    std::string error = std::string() + msg + "[" + std::to_string((errno)) + "]" + ": " + std::strerror((errno));   \
    throw std::runtime_error(error); })
#define MAX_QUEUED_REQUESTS 100
#define BUFSIZE 4096
#define SELECT_TIMEOUT 1000

class Connection : public swss::Selectable
{
public:
    Connection(std::shared_ptr<swss::Select> selector, int pri = 0) : Selectable(pri), selector(selector) {}

    virtual void doTask() = 0;

    virtual ~Connection() = default;

protected:
    std::shared_ptr<swss::Select> selector;
};

class Client : public Connection
{
public:
    Client(std::shared_ptr<swss::Select> selector, int clientfd, struct sockaddr_in clientaddr);

    int getFd();

    uint64_t readData();

    int getLastReadResult();

    void doTask();

    ~Client();

private:
    int clientfd;
    struct sockaddr_in clientaddr;
    std::array<char, BUFSIZE + 1> buffer;
    int lastReadResult;
};

class Acceptor : public Connection
{
public:
    static std::map<int, std::shared_ptr<Client>> clients;
    inline static void addClient(std::shared_ptr<Client> client)
    {
        clients[(*client).getFd()] = client;
    }

    inline static bool hasClient(int clientfd)
    {
        return clients.count(clientfd) == 1;
    }

    inline static std::shared_ptr<Client> getClient(int clientfd)
    {
        return clients[clientfd];
    }

    inline static void removeClient(int clientfd)
    {
        clients.erase(clientfd);
    }

public:
    Acceptor(std::shared_ptr<swss::Select> selector, int port = 9000);

    int getFd();

    uint64_t readData();

    void doTask();

    ~Acceptor();

private:
    int serverfd;
    struct sockaddr_in serveraddr;
    int lastAcceptedClientFd = -1;
};

std::map<int, std::shared_ptr<Client>> Acceptor::clients;

int main()
{
    int ret;
    std::shared_ptr<swss::Select> selector = std::make_shared<swss::Select>();
    std::shared_ptr<Acceptor> acceptor = std::make_shared<Acceptor>(selector);
    selector->addSelectable(acceptor.get());

    for (;;)
    {
        swss::Selectable *sel;
        ret = selector->select(&sel, SELECT_TIMEOUT);

        if (ret == swss::Select::ERROR)
        {
            ERROR_THROW("select");
            continue;
        }
        else if (ret == swss::Select::TIMEOUT)
        {
            continue;
        }

        auto conn = reinterpret_cast<Connection *>(sel);
        conn->doTask();
    }
    return 0;
}

Acceptor::Acceptor(std::shared_ptr<swss::Select> selector, int port) : Connection(selector)
{
    int ret;
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd == -1)
    {
        ERROR_THROW("socket");
    }

    int enable = 1;
    ret = setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    if (ret < 0)
    {
        ERROR_THROW("setsockopt");
    }

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);
    ret = bind(serverfd, reinterpret_cast<sockaddr *>(&serveraddr), sizeof(serveraddr));
    if (ret < 0)
    {
        ERROR_THROW("bind");
    }

    ret = listen(serverfd, MAX_QUEUED_REQUESTS);
    if (ret < 0)
    {
        ERROR_THROW("listen");
    }
}

int Acceptor::getFd()
{
    return serverfd;
}

uint64_t Acceptor::readData()
{
    return 0;
}

void Acceptor::doTask()
{
    std::cout << "Accept new connection" << std::endl;
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int clientfd = accept4(serverfd, reinterpret_cast<sockaddr *>(&clientaddr), &len, SOCK_NONBLOCK);
    if (clientfd == -1)
    {
        ERROR_THROW("accept4");
    }

    std::shared_ptr<Client> client = std::make_shared<Client>(selector, clientfd, clientaddr);
    selector->addSelectable(client.get());
    addClient(client);
}

Acceptor::~Acceptor()
{
    close(serverfd);
}

Client::Client(
    std::shared_ptr<swss::Select> selector,
    int clientfd, struct sockaddr_in clientaddr) : Connection(selector), clientfd(clientfd), clientaddr(clientaddr)
{
}

int Client::getFd()
{
    return clientfd;
}

uint64_t Client::readData()
{
    std::cout << "Read from connection" << std::endl;
    size_t msg_size = read(clientfd, buffer.data(), BUFSIZE);
    lastReadResult = msg_size;
    return msg_size;
}

void Client::doTask()
{
    int msg_size;
    if ((msg_size = getLastReadResult()) > 0)
    {
        std::cout << "From client: " << std::string(buffer.data()) << std::endl;
        write(clientfd, buffer.data(), msg_size);
    }
    else
    {
        selector->removeSelectable(this);
        Acceptor::removeClient(getFd());
    }
}

int Client::getLastReadResult()
{
    return lastReadResult;
}

Client::~Client()
{
    close(clientfd);
}
