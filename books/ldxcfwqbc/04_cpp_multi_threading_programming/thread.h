#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <pthread.h>

namespace thread
{

class Thread
{
public:
    using thread_func = std::function<void ()>;
public:
     explicit Thread(const thread_func &func, const std::string &name=std::string());
     ~Thread();

    void start();
    void join();

    bool started() const { return _started; }
    pid_t tid() const { return *_tid; }
    const std::string &name() const { return _name; }
    static int threads_created() { return _threads_created; }

private:
    Thread(const Thread &) = delete;
    Thread &operator=(const Thread &) = delete;

private:
    bool _started = false;
    bool _joined = false;
    pthread_t _pthread_id = 0;
    std::shared_ptr<pid_t> _tid;
    thread_func _func;
    std::string _name;
    static std::atomic<int> _threads_created;
};

namespace current_thread
{
    pid_t tid();
    const char *name();
    bool is_main_thread();
};

};
