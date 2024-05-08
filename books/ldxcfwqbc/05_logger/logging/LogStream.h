#ifndef _LOG_STREAM_H
#define _LOG_STREAM_H

#include <cassert>
#include <cstring>
#include <limits>
#include <string>

const int SMALL_BUFFER_SIZE = 4000;
const int LARGE_BUFFER_SIZE = 4000 * 1000;

template <int SIZE>
class FixedBuffer
{
public:
    FixedBuffer() : _curr(_data) {}
    FixedBuffer(FixedBuffer &) = delete;
    FixedBuffer &operator=(const FixedBuffer &) = delete;
    ~FixedBuffer() {}
    const char *data() const { return _data; }
    int length() const { return _curr - _data; }
    char *current() { return _curr; }
    int avail() const { return static_cast<int>(end() - _curr); }
    void append(const char *buf, int len)
    {
        if (avail() > len)
        {
            memcpy(_curr, buf, len);
            _curr += len;
        }
    }
    void add(size_t len) { _curr += len; }
    void reset() { _curr = _data; }
    void bzero() { memset(_data, 0, SIZE); }
    std::string to_string() const { return std::string(_data, length()); }

private:
    const char *end() const { return _data + SIZE; }

private:
    char _data[SIZE];
    char *_curr;
};

class T
{
public:
    T(const char *str, int len)
        : _str(str),
          _len(len)
    {
        assert(strlen(_str) == _len);
    }

    const char *_str;
    const size_t _len;
};

class LogStream
{
public:
    using Buffer = FixedBuffer<SMALL_BUFFER_SIZE>;

public:
    LogStream()
    {
        static_assert(MAX_NUMERIC_SIZE - 10 > std::numeric_limits<double>::digits10, "numeric size");
        static_assert(MAX_NUMERIC_SIZE - 10 > std::numeric_limits<long double>::digits10, "numeric size");
        static_assert(MAX_NUMERIC_SIZE - 10 > std::numeric_limits<long>::digits10, "numeric size");
        static_assert(MAX_NUMERIC_SIZE - 10 > std::numeric_limits<long long>::digits10, "numeric size");
    }
    LogStream(const LogStream &) = delete;
    LogStream &operator=(const LogStream &) = delete;
    ~LogStream() = default;

    LogStream &operator<<(bool v)
    {
        _buffer.append(v ? "1" : "0", 1);
        return *this;
    }

    LogStream &operator<<(short);
    LogStream &operator<<(unsigned short);
    LogStream &operator<<(int);
    LogStream &operator<<(unsigned int);
    LogStream &operator<<(long);
    LogStream &operator<<(unsigned long);
    LogStream &operator<<(long long);
    LogStream &operator<<(unsigned long long);

    LogStream &operator<<(const void *);

    LogStream &operator<<(float v)
    {
        *this << static_cast<double>(v);
        return *this;
    }
    LogStream &operator<<(double);

    LogStream &operator<<(char v)
    {
        _buffer.append(&v, 1);
        return *this;
    }

    LogStream &operator<<(const char *v)
    {
        _buffer.append(v, strlen(v));
        return *this;
    }

    LogStream &operator<<(const T &v)
    {
        _buffer.append(v._str, v._len);
        return *this;
    }

    LogStream &operator<<(const std::string &v) // FIXME: StringPiece
    {
        _buffer.append(v.c_str(), v.size());
        return *this;
    }

    void append(const char *data, int len) { _buffer.append(data, len); }
    const Buffer &buffer() const { return _buffer; }
    void reset_buffer() { _buffer.reset(); }

private:
    template <typename T>
    void format_integer(T);

private:
    Buffer _buffer;
    static const int MAX_NUMERIC_SIZE = 64;
};

class Fmt
{
public:
    template <typename T>
    Fmt(const char *fmt, T val);

    const char *data() const { return _buf; }
    int length() const { return _length; }

private:
    char _buf[32];
    int _length;
};

inline LogStream &operator<<(LogStream &s, const Fmt &fmt)
{
    s.append(fmt.data(), fmt.length());
    return s;
}

#endif