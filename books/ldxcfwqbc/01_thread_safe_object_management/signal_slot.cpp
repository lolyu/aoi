#include <iostream>
#include <memory>
#include <vector>
#include <functional>

template <typename RET, typename... ARGS>
class Signal;

template <typename RET, typename... ARGS>
class Signal<RET(ARGS...)>
{
public:
    typedef std::function<RET(ARGS...)> Functor;

    void connect(Functor &&functor)
    {
        // the std::forward is to embrace the move semantics
        // functor is a rvalue reference, it is a lvalue inside
        // connect function body.
        // and there are two overloads of push_back:
        // void push_back(const T& value)       [1]
        // void push_back(T &&value)            [2]
        // in order to invoke [2], use std::forward to pass functor
        // to push_back as rvalue reference
        _functors.push_back(std::forward<Functor>(functor));
    }

    void call(ARGS&& ... args)
    {
        for (const Functor &functor : _functors)
        {
            functor(args...);
        }
    }

private:
    std::vector<Functor> _functors;
};

void print(int i)
{
    std::cout << i << std::endl;
}

void inc_print(int i)
{
    std::cout << ++i << std::endl;
}

void dec_print(int i)
{
    std::cout << --i << std::endl;
}


int main()
{
    Signal<void()> signal;
    int i = 10;

    signal.connect(std::bind(&print, i));
    signal.connect(std::bind(&inc_print, i));
    signal.connect(std::bind(&dec_print, i));

    signal.call();

    return 0;
}
