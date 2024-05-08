#ifndef _LOG_FILE_H
#define _LOG_FILE_H

#include <mutex>
#include <string>
#include <memory>
#include <chrono>
#include <cstdio>
#include <iostream>

class LogFile
{
public:
    using Time = std::chrono::time_point<std::chrono::system_clock>;

public:
    LogFile(const std::string &basename, size_t rotate_size, bool thread_safe = false, int flush_interval = 3);
    LogFile(const LogFile &) = delete;
    LogFile &operator=(const LogFile &) = delete;
    ~LogFile() = default;

public:
    void append(const char *logline, int len);
    void flush();
    void rotate_file();

private:
    void append_unlocked(const char *logline, int len);
    static std::string get_log_filename(const std::string &base_name, Time &curr_time);
    void reset_file(const std::string &filename)
    {
        _file.reset(new File(filename));
        _filename = filename;
    }

private:
    const static int CHECK_TIME_ROTATE = 1024;
    const static int ROTATE_PER_SECONDS = 60 * 60 * 24;
    class File
    {
    public:
        explicit File(const std::string &filename) : _fp(fopen(filename.c_str(), "ae")), _bytes_written(0)
        {
            setbuffer(_fp, _buffer, sizeof(_buffer));
        }
        File(const File &) = delete;
        File &operator=(const File &) = delete;
        ~File()
        {
            flush();
            fclose(_fp);
        }

    public:
        void append(const char *logline, const size_t len)
        {
            size_t remain = len, written = 0;
            do
            {
                int delta = fwrite_unlocked(logline + written, sizeof(char), remain, _fp);
                if (delta == 0)
                {
                    int err = ferror(_fp);
                    if (err)
                    {
                        std::cerr << "LogFile::File::append() failed" << std::endl;
                        break;
                    }
                }
                written += delta;
                remain -= delta;
            } while (remain > 0);

            _bytes_written += len;
        }

        void flush()
        {
            fflush(_fp);
        }

        size_t bytes_written() const { return _bytes_written; }

    private:
        FILE *_fp;
        char _buffer[1024 * 64];
        size_t _bytes_written;
    };

private:
    const std::string _basename;
    const size_t _rotate_size;
    const int _flush_interval;
    int _count;

    std::string _filename;
    std::unique_ptr<std::mutex> _mutex;
    Time _last_rotate;
    Time _last_flush;
    std::time_t _start_of_period;
    std::unique_ptr<File> _file;
};

#endif
