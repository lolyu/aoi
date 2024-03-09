#include <memory>
#include <mutex>
#include <iostream>
#include <vector>
#include <cassert>


class Foo
{
public:
    void doit() const;
};

typedef std::vector<Foo> FooList;
typedef std::shared_ptr<FooList> FooListPtr;
std::mutex g_foolist_mutex;
FooListPtr g_foos;

void post(const Foo &f)
{
    std::lock_guard<std::mutex> lock(g_foolist_mutex);
    if (!g_foos.unique())
    {
        g_foos.reset(new FooList(*g_foos));
    }
    g_foos->push_back(f);
}

void traverse()
{
    FooListPtr foos_view;
    {
        std::lock_guard<std::mutex> lock(g_foolist_mutex);
        foos_view = g_foos;
        assert(!g_foos.unique());
    }

    // assert(!g_foos.unique());
    for (auto it = foos_view->cbegin(); it != foos_view->cend(); ++it)
    {
        it->doit();
    }
}

void Foo::doit() const
{
    Foo f;
    post(f);
}

int main()
{
    g_foos = std::make_shared<FooList>();
    Foo f;
    post(f);
    traverse();
    return 0;
}
