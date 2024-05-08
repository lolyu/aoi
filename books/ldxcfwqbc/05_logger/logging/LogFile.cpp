#include "LogFile.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <unistd.h>

LogFile::LogFile(const std::string &basename,
                 size_t rotate_size,
                 bool thread_safe,
                 int flush_interval)
    : _basename(basename),
      _rotate_size(rotate_size),
      _flush_interval(flush_interval),
      _count(0),
      _mutex(thread_safe ? new std::mutex : NULL),
      _last_rotate(),
      _last_flush(),
      _start_of_period()
{
    assert(basename.find('/') == std::string::npos);
    rotate_file();
}

void LogFile::append(const char *logline, int len)
{
    if (_mutex)
    {
        std::lock_guard<std::mutex> lock(*_mutex);
        append_unlocked(logline, len);
    }
    else
    {
        append_unlocked(logline, len);
    }
}

void LogFile::flush()
{
    if (_mutex)
    {
        std::lock_guard<std::mutex> lock(*_mutex);
        _file->flush();
    }
    else
    {
        _file->flush();
    }
}

void LogFile::append_unlocked(const char *logline, int len)
{
    _file->append(logline, len);

    if (_file->bytes_written() > _rotate_size)
    {
        std::cout << "Rotate due to bytes written " << _file->bytes_written() << std::endl;
        rotate_file();
    }
    else
    {
        if (_count > CHECK_TIME_ROTATE)
        {
            _count = 0;
            Time now = std::chrono::system_clock::now();
            time_t this_period = std::chrono::system_clock::to_time_t(now) / ROTATE_PER_SECONDS * ROTATE_PER_SECONDS;
            if (this_period != _start_of_period)
            {
                std::cout << "Rotate due to new period" << std::endl;
                rotate_file();
            }
            else if (std::chrono::system_clock::to_time_t(now) - std::chrono::system_clock::to_time_t(_last_flush) > _flush_interval)
            {
                _last_flush = now;
                _file->flush();
            }
        }
        else
        {
            ++_count;
        }
    }
}

void LogFile::rotate_file()
{
    Time now;
    std::string filename = get_log_filename(_basename, now);
    std::time_t start = std::chrono::system_clock::to_time_t(now) / ROTATE_PER_SECONDS * ROTATE_PER_SECONDS;

    if (now > _last_rotate && filename != _filename)
    {
        _last_rotate = now;
        _last_flush = now;
        _start_of_period = start;
        reset_file(filename);
        std::cout << "Rotate log file to " << filename << std::endl;
    }
}

std::string LogFile::get_log_filename(const std::string &basename, Time &now)
{
    std::ostringstream ss;
    ss << basename;
    now = std::chrono::system_clock::now();
    std::time_t now_tt = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::gmtime(&now_tt);
    ss << std::put_time(&tm, ".%Y%m%d-%H%M%S");
    ss << '.' << getpid() << ".log";
    return ss.str();
}