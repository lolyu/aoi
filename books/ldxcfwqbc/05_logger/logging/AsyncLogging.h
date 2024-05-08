#ifndef _ASYNC_LOGGING
#define _ASYNC_LOGGING

#include "LogFile.h"
#include "LogStream.h"

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class AsyncLogging
{
public:
    using Buffer = FixedBuffer<LARGE_BUFFER_SIZE>;
    using BufferVector = std::vector<std::unique_ptr<Buffer>>;
    using BufferPtr = BufferVector::value_type;

public:
    AsyncLogging(const std::string &basename, size_t rotate_size, int flush_interval = 3) :
        _flush_interval(flush_interval),
        _running(false),
        _basename(basename),
        _rotate_size(rotate_size),
        _curr_buff(new Buffer()),
        _next_buff(new Buffer())
    {
        _curr_buff->bzero();
        _next_buff->bzero();
        _buffers.reserve(16);
    }
    AsyncLogging(const AsyncLogging &) = delete;
    AsyncLogging &operator=(const AsyncLogging &) = delete;
    ~AsyncLogging()
    {
        if (_running)
        {
            stop();
        }
    }

    void append(const char *logline, int len)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_curr_buff->avail() > len)
        {
            _curr_buff->append(logline, len);
        }
        else
        {
            _buffers.push_back(std::move(_curr_buff));
            if (_next_buff)
            {
                _curr_buff = std::move(_next_buff);
            }
            else
            {
                _curr_buff.reset(new Buffer());
            }
            _curr_buff->append(logline, len);
            _log_avail_cond.notify_all();
        }
    }

    void start()
    {
        _running = true;
        _thread = std::thread(std::bind(&AsyncLogging::thread_func, this));
    }

    void stop()
    {
        _running = false;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _log_avail_cond.notify_all();
        }
        _thread.join();
    }

private:
    void thread_func()
    {
        assert(_running == true);
        LogFile output(_basename, _rotate_size, false);
        BufferPtr buffer1 = std::make_unique<Buffer>();
        BufferPtr buffer2 = std::make_unique<Buffer>();
        buffer1->bzero();
        buffer2->bzero();

        BufferVector buffers_to_write;
        buffers_to_write.reserve(16);
        while (_running)
        {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                if (_buffers.empty())
                {
                    _log_avail_cond.wait_for(lock, std::chrono::seconds(_flush_interval));
                }
                _buffers.push_back(std::move(_curr_buff));
                _curr_buff = std::move(buffer1);
                // place buffers into buffers_to_write
                std::move(_buffers.begin(), _buffers.end(), std::back_inserter(buffers_to_write));
                _buffers.clear();
                if (!_next_buff)
                {
                    _next_buff = std::move(buffer2);
                }
            }
            if (buffers_to_write.size() > 25)
            {
                buffers_to_write.erase(buffers_to_write.begin(), buffers_to_write.end() - 2);
            }
            for (auto &buffer : buffers_to_write)
            {
                output.append(buffer->data(), buffer->length());
            }
            if (buffers_to_write.size() > 2)
            {
                buffers_to_write.resize(2);
            }
            if (!buffer1)
            {
                assert(!buffers_to_write.empty());
                buffer1 = std::move(buffers_to_write.back());
                buffers_to_write.pop_back();
                buffer1->reset();
            }
            if (!buffer2)
            {
                assert(!buffers_to_write.empty());
                buffer2 = std::move(buffers_to_write.back());
                buffers_to_write.pop_back();
                buffer2->reset();
            }
            buffers_to_write.clear();
            output.flush();
        }
        output.flush();
    }

private:
    const int _flush_interval;
    bool _running;
    std::string _basename;
    size_t _rotate_size;
    std::thread _thread;
    std::mutex _mutex;
    std::condition_variable _started;
    std::condition_variable _log_avail_cond;
    BufferPtr _curr_buff;
    BufferPtr _next_buff;
    BufferVector _buffers;
};

#endif