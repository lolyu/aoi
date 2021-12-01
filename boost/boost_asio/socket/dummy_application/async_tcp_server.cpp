#include <boost/asio.hpp>
#include <thread>
#include <atomic>
#include <memory>
#include <iostream>

class Service
{
public:
    Service(std::shared_ptr<boost::asio::ip::tcp::socket> sock) : m_sock(sock) {}

    void startHandling()
    {
        boost::asio::async_read_until(*m_sock.get(), m_request, '\n', [this](const boost::system::error_code &ec, std::size_t bytes_transferred)
                                      { onRequestReceived(ec, bytes_transferred); });
    }

private:
    void onRequestReceived(const boost::system::error_code &ec, std::size_t bytes_transferred)
    {
        if (ec.value() != 0)
        {
            std::cout << "Error occurred! Error code = " << ec.value() << ". Error message = " << ec.message() << std::endl;
            onFinish();
            return;
        }
        m_response = processRequst(m_request);
        boost::asio::async_write(*m_sock.get(), boost::asio::buffer(m_response), [this](const boost::system::error_code &ec, std::size_t bytes_transferred)
                                 { onResponseSent(ec, bytes_transferred); });
    }

    void onResponseSent(const boost::system::error_code &ec, std::size_t bytes_transferred)
    {
        if (ec.value() != 0)
        {
            std::cout << "Error occurred! Error code = " << ec.value() << ". Error message = " << ec.message() << std::endl;
        }
        onFinish();
        return;
    }

    void onFinish()
    {
        delete this;
    }

    std::string processRequst(boost::asio::streambuf &request)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::istream input(&request);
        std::string request_message;
        std::getline(input, request_message);
        std::cout << "Recieved request: " << request_message << std::endl;
        std::string response = "Response\n";
        return response;
    }

    std::shared_ptr<boost::asio::ip::tcp::socket> m_sock;
    std::string m_response;
    boost::asio::streambuf m_request;
};

class Acceptor
{
public:
    Acceptor(boost::asio::io_service &ios, unsigned short port_num) : m_ios(ios), m_acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port_num)), m_isStopped(false) {}

    void start()
    {
        m_acceptor.listen();
        initAccept();
    }

    void stop()
    {
        m_isStopped.store(true);
    }

private:
    void initAccept()
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(m_ios));
        m_acceptor.async_accept(*sock.get(), [this, sock](const boost::system::error_code &ec)
                                { onAccept(ec, sock); });
    }

    void onAccept(const boost::system::error_code &ec, std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        if (ec.value() == 0)
        {
            (new Service(sock))->startHandling();
        }
        else
        {
            std::cout << "Error occurred! Error code = " << ec.value() << ". Error message = " << ec.message() << std::endl;
        }

        if (!m_isStopped.load())
        {
            initAccept();
        }
        else
        {
            m_acceptor.close();
        }
    }

    boost::asio::io_service &m_ios;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::atomic<bool> m_isStopped;
};

class Server
{
public:
    Server()
    {
        m_work.reset(new boost::asio::io_service::work(m_ios));
    }

    void start(unsigned short port_num, unsigned int thread_pool_size)
    {
        assert(thread_pool_size > 0);
        m_acc.reset(new Acceptor(m_ios, port_num));
        m_acc->start();

        for (int i = 0; i < thread_pool_size; ++i)
        {
            std::unique_ptr<std::thread> th(new std::thread([this]()
                                                            { m_ios.run(); }));
            m_thread_pool.push_back(std::move(th));
        }
    }

    void stop()
    {
        m_acc->stop();
        // boost::asio::io_service::stop will discard any pending asynchronous operations
        m_ios.stop();
        for (auto &th : m_thread_pool)
        {
            th->join();
        }
    }

private:
    boost::asio::io_service m_ios;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::unique_ptr<Acceptor> m_acc;
    std::vector<std::unique_ptr<std::thread>> m_thread_pool;
};

const unsigned int DEFAULT_THREAD_POOL_SIZE = 2;

int main()
{
    unsigned short port_num = 9000;
    unsigned int thread_pool_size = std::thread::hardware_concurrency() * 2;
    if (thread_pool_size == 0)
    {
        thread_pool_size = DEFAULT_THREAD_POOL_SIZE;
    }
    try
    {
        Server serv;
        serv.start(port_num, thread_pool_size);
        std::this_thread::sleep_for(std::chrono::seconds(10));
        serv.stop();
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << e.what() << '\n';
        return e.code().value();
    }
    return 0;
}
