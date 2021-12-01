#include <boost/asio.hpp>
#include <iostream>

class SyncTCPClient
{
public:
    SyncTCPClient(const std::string &raw_ip_address, unsigned short port_num) : m_ep(boost::asio::ip::address::from_string(raw_ip_address), port_num), m_sock(m_ios)
    {
        m_sock.open(m_ep.protocol());
    }

    void connect()
    {
        m_sock.connect(m_ep);
    }

    void close()
    {
        m_sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        m_sock.close();
    }

    std::string emulateLongComputationOp(unsigned int duration_sec)
    {
        std::ostringstream oss;
        oss << "EMULATE_LONG_COMP_OP " << std::to_string(duration_sec) << "\n";
        sendRequest(oss.str());
        return receiveResponse();
    }

private:
    void sendRequest(const std::string &request)
    {
        boost::asio::write(m_sock, boost::asio::buffer(request));
    }

    std::string receiveResponse()
    {
        boost::asio::streambuf buf;
        boost::asio::read_until(m_sock, buf, '\n');
        std::istream input(&buf);
        std::string response;
        std::getline(input, response);
        return response;
    }

    boost::asio::io_service m_ios;
    boost::asio::ip::tcp::endpoint m_ep;
    boost::asio::ip::tcp::socket m_sock;
};

int main()
{
    const std::string raw_ip_address = "127.0.0.1";
    const unsigned short port_num = 9000;

    try
    {
        SyncTCPClient client(raw_ip_address, port_num);
        client.connect();
        std::cout << "Sending request to the server..." << std::endl;
        std::string response = client.emulateLongComputationOp(10);
        std::cout << "Response received: " << response << std::endl;
        client.close();
    }
    catch (const boost::system::system_error &ec)
    {
        std::cerr << "Error code = " << ec.code().value() << ", error message = " << ec.what() << std::endl;
        return ec.code().value();
    }

    return 0;
}
