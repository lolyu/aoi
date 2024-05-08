#include "LogStream.h"

#include <algorithm>

const char DIGITS[] = "9876543210123456789";
const char *ZERO = DIGITS + 9;

const char DIGITS_HEX[] = "0123456789abcdef";

template <typename T>
size_t convert(char buf[], T value)
{
    char *p = buf;
    T i = value;
    do
    {
        *p++ = ZERO[i % 10];
        i /= 10;
    } while (i != 0);

    if (value < 0)
    {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);
    return p - buf;
}

size_t convert_to_hex(char buf[], uintptr_t value)
{
    uintptr_t i = value;
    char *p = buf;

    do
    {
        int lsd = i % 16;
        i /= 16;
        *p++ = DIGITS_HEX[lsd];
    } while (i != 0);

    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

template class FixedBuffer<SMALL_BUFFER_SIZE>;
template class FixedBuffer<LARGE_BUFFER_SIZE>;

template <typename T>
void LogStream::format_integer(T v)
{
    if (_buffer.avail() >= MAX_NUMERIC_SIZE)
    {
        size_t len = convert(_buffer.current(), v);
        _buffer.add(len);
    }
}

LogStream &LogStream::operator<<(short v)
{
    *this << static_cast<int>(v);
    return *this;
}

LogStream &LogStream::operator<<(unsigned short v)
{
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream &LogStream::operator<<(int v)
{
    format_integer(v);
    return *this;
}

LogStream &LogStream::operator<<(unsigned int v)
{
    format_integer(v);
    return *this;
}

LogStream &LogStream::operator<<(long v)
{
    format_integer(v);
    return *this;
}

LogStream &LogStream::operator<<(unsigned long v)
{
    format_integer(v);
    return *this;
}

LogStream &LogStream::operator<<(long long v)
{
    format_integer(v);
    return *this;
}

LogStream &LogStream::operator<<(unsigned long long v)
{
    format_integer(v);
    return *this;
}

LogStream &LogStream::operator<<(const void *p)
{
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    if (_buffer.avail() >= MAX_NUMERIC_SIZE)
    {
        char *buf = _buffer.current();
        buf[0] = '0';
        buf[1] = 'x';
        size_t len = convert_to_hex(buf + 2, v);
        _buffer.add(len + 2);
    }
    return *this;
}

LogStream& LogStream::operator<<(double v)
{
  if (_buffer.avail() >= MAX_NUMERIC_SIZE)
  {
    int len = snprintf(_buffer.current(), MAX_NUMERIC_SIZE, "%.12g", v);
    _buffer.add(len);
  }
  return *this;
}

template<typename T>
Fmt::Fmt(const char* fmt, T val)
{
  _length = snprintf(_buf, sizeof(_buf), fmt, val);
}

template Fmt::Fmt(const char* fmt, char);
template Fmt::Fmt(const char* fmt, short);
template Fmt::Fmt(const char* fmt, unsigned short);
template Fmt::Fmt(const char* fmt, int);
template Fmt::Fmt(const char* fmt, unsigned int);
template Fmt::Fmt(const char* fmt, long);
template Fmt::Fmt(const char* fmt, unsigned long);
template Fmt::Fmt(const char* fmt, long long);
template Fmt::Fmt(const char* fmt, unsigned long long);
template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);
