#include <boost/asio.hpp>
#include <thread>
#include <atomic>
#include <memory>
#include <iostream>

class Service
{
public:
    Service() = default;

    void startHandlingClient(std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        std::thread th([this, sock]()
                       { handleClient(sock); });
        th.detach();
    }

private:
    void handleClient(std::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        try
        {
            boost::asio::streambuf request;
            boost::asio::read_until(*sock.get(), request, '\n');
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::string response = "Response\n";
            boost::asio::write(*sock.get(), boost::asio::buffer(response));
        }
        catch (const boost::system::system_error &e)
        {
            std::cerr << "Error occured! Error code = " << e.code() << ". Error message = " << e.what() << "\n";
        }
        delete this;
    }
};

class Acceptor
{
public:
    Acceptor(boost::asio::io_service &ios, unsigned short port_num) : m_ios(ios), m_acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port_num))
    {
        m_acceptor.listen();
    }

    void accept()
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> sock_ptr(new boost::asio::ip::tcp::socket(m_ios));
        m_acceptor.accept(*sock_ptr.get());
        (new Service)->startHandlingClient(sock_ptr);
    }

private:
    boost::asio::io_service &m_ios;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

class Server
{
public:
    Server() : m_stop(false){};

    void start(unsigned short port_num)
    {
        m_port_num = port_num;
        m_thread.reset(new std::thread([this, port_num]()
                                       { run(port_num); }));
    }

    void stop()
    {
        m_stop.store(true);
        send_dummy_request();
        m_thread->join();
    }

private:
    void run(unsigned short port_num)
    {
        Acceptor acc(m_ios, port_num);
        while (!m_stop.load())
        {
            acc.accept();
        }
    }

    void send_dummy_request()
    {
        try
        {
            boost::asio::ip::tcp::socket dummy_sock(m_ios);
            boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), m_port_num);
            dummy_sock.open(ep.protocol());
            dummy_sock.connect(ep);
            dummy_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        }
        catch (const boost::system::error_code &ec)
        {
            // do nothing
        }
    }

    std::unique_ptr<std::thread> m_thread;
    std::atomic<bool> m_stop;
    boost::asio::io_service m_ios;
    unsigned short m_port_num;
};

int main()
{
    unsigned short port_num = 9000;
    try
    {
        Server serv;
        serv.start(port_num);
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
