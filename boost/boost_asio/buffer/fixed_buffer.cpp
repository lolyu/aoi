#include <iostream>
#include <boost/asio.hpp>
#include <memory>

using namespace boost;

int main()
{
    // typedef std::pair<void*, std::size_t> mutable_buffer;
    // typedef std::pair<const void*, std::size_t> const_buffer;
    // asio::buffer either returns asio::mutable_buffers_1 or asio::const_buffers_1
    // asio::mutable_buffers_1 and asio::const_buffers_1 are adapters to asio::mutable_buffer and asio::const_buffer
    // and they meet the requirements of MutableBufferSequence and ConstBufferSequence accordingly
    const std::string str0 = "helloworld";
    asio::const_buffers_1 buf0 = asio::buffer(str0);

    std::string str1 = "helloworld";
    asio::mutable_buffers_1 buf1 = asio::buffer(str1);
    asio::const_buffers_1 buf2 = static_cast<asio::const_buffers_1>(asio::buffer(str1));

    const size_t BUF_SIZE_BYTES = 20;
    std::unique_ptr<char[]> buf(new char[BUF_SIZE_BYTES]);
    asio::mutable_buffers_1 buf3 = asio::buffer(buf.get(), BUF_SIZE_BYTES);

    return 0;
}