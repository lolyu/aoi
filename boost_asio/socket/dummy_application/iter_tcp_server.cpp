#include <boost/asio.hpp>
#include <thread>
#include <atomic>
#include <memory>
#include <iostream>

class Service
{
public:
    void handleClient(boost::asio::ip::tcp::socket &sock)
    {
        try
        {
            boost::asio::streambuf request;
            boost::asio::read_until(sock, request, '\n');
            std::istream is(&request);
            std::string message;
            std::getline(is, message);
            std::cout << "Receive message: " << message << std::endl;
            for (int i = 0; i < 10; ++i)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
            std::string response = "Response\n";
            boost::asio::write(sock, boost::asio::buffer(response));
        }
        catch (const boost::system::system_error &e)
        {
            std::cerr << "Error occured! Error code = " << e.code() << ". Message = " << e.what() << std::endl;
        }
    }
};

class Acceptor
{
public:
    Acceptor(boost::asio::io_service &ios, unsigned short port_num) : m_ios(ios), m_acceptor(m_ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port_num))
    {
        m_acceptor.listen();
    }

    void accept()
    {
        boost::asio::ip::tcp::socket sock(m_ios);
        m_acceptor.accept(sock);
        Service().handleClient(sock);
    }

private:
    boost::asio::io_service &m_ios;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

class Server
{
public:
    Server() : m_stop(false) {}

    void start(unsigned short port_num)
    {
        m_thread.reset(new std::thread([this, port_num]()
                                       { run(port_num); }));
    }

    void stop()
    {
        m_stop.store(false);
        m_thread->join();
    }

private:
    void run(unsigned short port_num)
    {
        Acceptor acc(m_ios, port_num);
        while (!m_stop.load())
        {
            // when the server is blocked in waiting for new connection, the stop()
            // method will never make the server thread exit
            acc.accept();
        }
    }
    std::unique_ptr<std::thread> m_thread;
    std::atomic<bool> m_stop;
    boost::asio::io_service m_ios;
};

int main()
{
    unsigned short port_num = 9000;
    try
    {
        Server s;
        s.start(port_num);
        std::this_thread::sleep_for(std::chrono::seconds(30));
        s.stop();
    }
    catch (const boost::system::system_error &e)
    {
        std::cerr << "Error occured! Error code = " << e.code() << ". Message = " << e.what() << std::endl;
        return e.code().value();
    }
    return 0;
}