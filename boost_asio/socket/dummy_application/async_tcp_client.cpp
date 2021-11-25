#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <map>

typedef void (*Callback)(unsigned int request_id, const std::string &reponse, const boost::system::error_code &ec);

struct Session
{
    Session(boost::asio::io_service &ios,
            const std::string &raw_ip_address,
            unsigned short port_num, const std::string &request,
            unsigned int id,
            Callback callback) : m_sock(ios),
                                 m_ep(boost::asio::ip::address::from_string(raw_ip_address), port_num),
                                 m_request(request),
                                 m_id(id),
                                 m_callback(callback),
                                 m_was_cancelled(false) {}

    boost::asio::ip::tcp::socket m_sock;
    boost::asio::ip::tcp::endpoint m_ep;
    std::string m_request;
    boost::asio::streambuf m_response_buf;
    std::string m_response;
    boost::system::error_code m_ec;
    unsigned int m_id;

    Callback m_callback;
    bool m_was_cancelled;
    std::mutex m_cancel_guard;
};

class AsyncTCPClient : public boost::noncopyable
{
public:
    AsyncTCPClient()
    {
        m_work.reset(new boost::asio::io_service::work(m_ios));
        m_thread.reset(new std::thread([this]()
                                       { m_ios.run(); }));
    }

private:
    boost::asio::io_service m_ios;
    std::map<int, std::shared_ptr<struct Session>> m_active_sessions;
    std::mutex m_active_sessions_guard;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::unique_ptr<std::thread> m_thread;
};

int main()
{
    return 0;
}