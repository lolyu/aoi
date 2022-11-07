#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

class printer
{
public:
    printer(boost::asio::io_context &io)
        : timer_(io, boost::asio::chrono::seconds(1)),
          count_(0)
    {
        timer_.async_wait(boost::bind(&printer::print, this, boost::asio::placeholders::error));
    }

    ~printer()
    {
        std::cout << "Final count is " << count_ << std::endl;
    }

    void print(const boost::system::error_code &error)
    {
        if (count_ < 5)
        {
            std::cout << count_ << std::endl;
            ++count_;

            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
            timer_.async_wait(boost::bind(&printer::print, this, boost::asio::placeholders::error));
        }
    }

private:
    boost::asio::steady_timer timer_;
    int count_;
};

int main()
{
    boost::asio::io_context io;
    printer p(io);
    io.run();

    return 0;
}
