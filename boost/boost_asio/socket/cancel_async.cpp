#include <boost/predef.h>

// #ifdef BOOST_OS_WINDOWS
// #define _WIN32_WINNT 0x0501

// #if _WIN32_WINNT <= 0x0502
// #define BOOST_ASIO_DISABLE_IOCP
// #define BOOST_ASIO_ENABLE_CANCELIO
// #endif
// #endif

#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using namespace boost;

int main()
{
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 9000;
    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address_v4::from_string(raw_ip_address), port_num);
        asio::io_service ios;

        std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()));
        sock->async_connect(ep, [&sock](const boost::system::error_code &ec)
                            {
                                if (ec.value() != 0)
                                {
                                    // std::cout << ec.value() << std::endl << asio::error::operation_aborted << std::endl;
                                    if (ec.value() == asio::error::operation_aborted)
                                    {
                                        std::cout << "Operation cancelled!" << std::endl;
                                    }
                                    else
                                    {
                                        std::cout << "Error code = " << ec.value() << ", error message = " << ec.message() << std::endl;
                                    }
                                }

                                return;
                            });
        std::thread worker_thread([&ios]()
                                  {
                                      try
                                      {
                                          ios.run();
                                      }
                                      catch (const system::system_error &e)
                                      {
                                          std::cout << "Error code = " << e.code() << ", error message = " << e.what() << std::endl;
                                      }
                                  });
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        sock->cancel();
        worker_thread.join();
    }
    catch (const system::system_error &e)
    {
        std::cout << "Error code = " << e.code() << ", error message = " << e.what() << std::endl;
        return e.code().value();
    }
    return 0;
}
