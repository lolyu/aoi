#include <cassert>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <optional>
#include <pthread.h>


template <typename T>
class SingletonWithPthreadOnce
{
public:
    SingletonWithPthreadOnce() = delete;
    ~SingletonWithPthreadOnce() = delete;
    SingletonWithPthreadOnce(const SingletonWithPthreadOnce &) = delete;
    SingletonWithPthreadOnce &operator=(const SingletonWithPthreadOnce &) = delete;

    static T instance()
    {
        pthread_once(&_once_control, &init);
        return *_value;
    }
private:
    static void init()
    {
        _value = new T();
        std::atexit(destroy);
    }

    static void destroy()
    {
        delete _value;
    }

    static pthread_once_t _once_control;
    static T *_value;
};

template <typename T>
pthread_once_t SingletonWithPthreadOnce<T>::_once_control = PTHREAD_ONCE_INIT;
template <typename T>
T *SingletonWithPthreadOnce<T>::_value = nullptr;


template <typename T>
class SingletonWithCallOnce
{
public:
    SingletonWithCallOnce() = delete;
    ~SingletonWithCallOnce() = delete;
    SingletonWithCallOnce(const SingletonWithCallOnce &) = delete;
    SingletonWithCallOnce &operator=(const SingletonWithCallOnce &) = delete;

    static T instance()
    {
        std::call_once(_once_control, [](){ _value = T(); });
    }
private:
    static std::once_flag _once_control;
    static std::optional<T> _value = std::nullopt;
};

template <typename T>
std::once_flag SingletonWithCallOnce<T>::_once_control;

int main()
{
    return 0;
}
