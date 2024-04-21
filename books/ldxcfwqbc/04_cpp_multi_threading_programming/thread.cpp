#include "thread.h"

#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <linux/unistd.h>

namespace thread
{

namespace current_thread
{
    thread_local const char *_thread_name = "UnnamedThread";
};

namespace
{
thread_local pid_t _thread_id = 0;

pid_t get_tid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

void after_fork()
{
  _thread_id = get_tid();
  current_thread::_thread_name = "main";
}

class ThreadNameInitializer
{
public:
    ThreadNameInitializer()
    {
        current_thread::_thread_name = "main";
        pthread_atfork(NULL, NULL, &after_fork);
    }
};

ThreadNameInitializer init;

struct ThreadData
{
    Thread::thread_func _func;
    std::string _name;
    std::weak_ptr<pid_t> _pid;

    ThreadData(const Thread::thread_func &func, const std::string &name, const std::shared_ptr<pid_t> &pid) : _func(func), _name(name), _pid(pid) {}

    void run()
    {
        std::shared_ptr<pid_t> ptid = _pid.lock();
        if (ptid)
        {
            *ptid = current_thread::tid();
        }

        if (!_name.empty())
        {
            current_thread::_thread_name = _name.c_str();
        }
        ::prctl(PR_SET_NAME, current_thread::_thread_name);
        _func();
        current_thread::_thread_name = "finished";
    }
};

void *start_thread(void *obj)
{
    ThreadData *data = static_cast<ThreadData*>(obj);
    data->run();
    delete data;
    return NULL;
}
}

pid_t current_thread::tid()
{
  if (_thread_id == 0)
  {
    _thread_id = get_tid();
  }
  return _thread_id;
}

const char* current_thread::name()
{
  return _thread_name;
}

bool current_thread::is_main_thread()
{
  return tid() == ::getpid();
}

std::atomic<int> Thread::_threads_created{0};

Thread::Thread(const thread_func &func, const std::string &name) :
    _tid(std::make_shared<pid_t>(0)),
    _func(func),
    _name(name)
{
    ++_threads_created;
}

Thread::~Thread()
{
    if (_started && !_joined)
    {
        pthread_detach(_pthread_id);
    }
}

void Thread::start()
{
    assert(!_started);
    _started = true;

    ThreadData *data = new ThreadData(_func, _name, _tid);
    if (pthread_create(&_pthread_id, NULL, &start_thread, data))
    {
        _started = false;
        delete data;
        abort();
    }
}

void Thread::join()
{
  assert(_started);
  assert(!_joined);
  _joined = true;
  pthread_join(_pthread_id, NULL);
}

};
