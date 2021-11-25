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
    // emulateLongComputationOp is a chain of multiple asynchronous operations, we check the cancel
    // status between each call to asynchronous operations(obtain the mutex m_cancel_guard)
    std::mutex m_cancel_guard;
};

class AsyncTCPClient : public boost::noncopyable
{
public:
    AsyncTCPClient(unsigned short num_of_threads)
    {
        // when `io_service::run` is called without a work object, it will return immediately
        // if it is called with a work object, the event loop will keep running
        m_work.reset(new boost::asio::io_service::work(m_ios));

        // https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/io_context/run/overload1.html
        // multiple threads could call the run() method to set up a pool of threads from which the io_service will execute handlers
        // all threads that are waiting in the pool are equaivalent and the io_service may choose any one of them to invoke a handler
        // NOTE: those threads are used to run handlers
        for (unsigned short i = 0; i < num_of_threads; ++i)
        {
            std::unique_ptr<std::thread> th(new std::thread([this]()
                                                            { m_ios.run(); }));
            m_threads.push_back(std::move(th));
        }
    }

    void emulateLongComputationOp(
        unsigned int duration_sec,
        const std::string &raw_ip_address,
        unsigned short port_num,
        Callback callback,
        unsigned int request_id)
    {
        std::string request = "EMULATE_LONG_CALC_OP " + std::to_string(duration_sec) + "\n";
        std::shared_ptr<struct Session> session(new Session(m_ios, raw_ip_address, port_num, request, request_id, callback));
        session->m_sock.open(session->m_ep.protocol());
        std::unique_lock<std::mutex> lock(m_active_sessions_guard);
        m_active_sessions[request_id] = session;
        lock.unlock();

        session->m_sock.async_connect(session->m_ep, [this, session](const boost::system::error_code &ec)
                                      {
                                          if (ec.value() != 0)
                                          {
                                              session->m_ec = ec;
                                              onRequestComplete(session);
                                              return;
                                          }

                                          std::unique_lock<std::mutex> cancel_lock(session->m_cancel_guard);
                                          if (session->m_was_cancelled)
                                          {
                                              onRequestComplete(session);
                                              return;
                                          }

                                          boost::asio::async_write(session->m_sock,
                                                                   boost::asio::buffer(session->m_request),
                                                                   [this, session](const boost::system::error_code &ec, std::size_t bytes_transferred)
                                                                   {
                                                                       if (ec.value() != 0)
                                                                       {
                                                                           session->m_ec = ec;
                                                                           onRequestComplete(session);
                                                                           return;
                                                                       }

                                                                       std::unique_lock<std::mutex> cancel_lock(session->m_cancel_guard);
                                                                       if (session->m_was_cancelled)
                                                                       {
                                                                           onRequestComplete(session);
                                                                           return;
                                                                       }

                                                                       boost::asio::async_read_until(session->m_sock,
                                                                                                     session->m_response_buf,
                                                                                                     '\n',
                                                                                                     [this, session](const boost::system::error_code &ec, std::size_t bytes_transferred)
                                                                                                     {
                                                                                                         if (ec.value() != 0)
                                                                                                         {
                                                                                                             session->m_ec = ec;
                                                                                                         }
                                                                                                         else
                                                                                                         {
                                                                                                             std::istream is(&session->m_response_buf);
                                                                                                             std::getline(is, session->m_response);
                                                                                                         }
                                                                                                         onRequestComplete(session);
                                                                                                     });
                                                                   });
                                      });
    }

    void cancelRequest(unsigned int request_id)
    {
        std::unique_lock<std::mutex> lock(m_active_sessions_guard);
        auto it = m_active_sessions.find(request_id);
        if (it != m_active_sessions.end())
        {
            std::unique_lock<std::mutex> cancel_lock(it->second->m_cancel_guard);
            it->second->m_was_cancelled = true;
            it->second->m_sock.cancel();
        }
    }

    void close()
    {
        // destroy the asio::io_service::work object to allow the I/O event loop thread to exit after all
        // asynchronous operations are completed
        m_work.reset(NULL);
        for (auto &th : m_threads)
        {
            th->join();
        }
    }

private:
    void onRequestComplete(std::shared_ptr<struct Session> session)
    {
        std::cout << "Comlete session: " << session->m_id << std::endl;
        boost::system::error_code ignored_ec;
        session->m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        session->m_sock.close();
        std::unique_lock<std::mutex> lock(m_active_sessions_guard);
        auto it = m_active_sessions.find(session->m_id);
        if (it != m_active_sessions.end())
        {
            m_active_sessions.erase(it);
        }
        lock.unlock();

        boost::system::error_code ec = (session->m_ec.value() == 0 && session->m_was_cancelled) ? boost::asio::error::operation_aborted : session->m_ec;
        session->m_callback(session->m_id, session->m_response, ec);
    }

private:
    boost::asio::io_service m_ios;
    std::map<int, std::shared_ptr<struct Session>> m_active_sessions;
    std::mutex m_active_sessions_guard;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::vector<std::unique_ptr<std::thread>> m_threads;
};

void handler(unsigned int request_id, const std::string &response, const boost::system::error_code &ec)
{
    if (ec.value() == 0)
    {
        std::cout << "Request #" << request_id << " has completed, response: " << response << std::endl;
    }
    else if (ec.value() == boost::asio::error::operation_aborted)
    {
        std::cout << "Request #" << request_id << " has been cancelled by the user." << std::endl;
    }
    else
    {
        std::cout << "Request #" << request_id << " failed! Error code = " << ec.value() << ", error message = " << ec.message() << std::endl;
    }
}

int main()
{
    try
    {
        AsyncTCPClient client(4);
        client.emulateLongComputationOp(10, "127.0.0.1", 9000, handler, 1);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        client.emulateLongComputationOp(11, "127.0.0.1", 9000, handler, 2);
        client.close();
    }
    catch (boost::system::system_error &e)
    {
        std::cout << "Error code = " << e.code().value() << ", error message = " << e.what() << std::endl;
        return e.code().value();
    }
    return 0;
}