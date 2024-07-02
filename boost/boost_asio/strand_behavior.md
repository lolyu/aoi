# strand behavior

* `strand::wrap`: create a new handler object that, once invoked, will pass the wrapped handler to `strand::dispatch`
* `strand::dispatch`: requests the `io_context` to invoke the handler
    * **the handler will be invoked directly if the caller is running with the strand** [1]
    * otherwise, it will be queued inside this strand and dispatched with `io_context::dispatch` [2]
```cpp
template <typename Executor, typename Function, typename Allocator>
void strand_executor_service::dispatch(const implementation_type& impl,
    Executor& ex, Function&& function, const Allocator& a)
{
  typedef decay_t<Function> function_type;

  // If we are already in the strand then the function can run immediately.        [1]
  if (running_in_this_thread(impl))
  {
    // Make a local, non-const copy of the function.
    function_type tmp(static_cast<Function&&>(function));

    fenced_block b(fenced_block::full);
    static_cast<function_type&&>(tmp)();
    return;
  }

  // Allocate and construct an operation to wrap the function.
  typedef executor_op<function_type, Allocator> op;
  typename op::ptr p = { detail::addressof(a), op::ptr::allocate(a), 0 };
  p.p = new (p.v) op(static_cast<Function&&>(function), a);

  BOOST_ASIO_HANDLER_CREATION((impl->service_->context(), *p.p,
        "strand_executor", impl.get(), 0, "dispatch"));

  // Add the function to the strand and schedule the strand if required.
  bool first = enqueue(impl, p.p);                                                [2]
  p.v = p.p = 0;
  if (first)
  {
    boost::asio::dispatch(ex,
        allocator_binder<invoker<Executor>, Allocator>(
          invoker<Executor>(impl, ex), a));
  }
}
```

## strand::wrap
* `io_context::strand` definition
```cpp
class io_context::strand
{
  ...
  detail::wrapped_handler<strand, Handler, detail::is_continuation_if_running>
#endif
  wrap(Handler handler)
  {
    return detail::wrapped_handler<io_context::strand, Handler,
        detail::is_continuation_if_running>(*this, handler);
  }
  ...
};
```
* `wrapped_handler` definition: simply a wrapper to call the dispatcher's `dispatch` method
```cpp
template <typename Dispatcher, typename Handler,
    typename IsContinuation = is_continuation_delegated>
class wrapped_handler
{
public:
  typedef void result_type;

  wrapped_handler(Dispatcher dispatcher, Handler& handler)
    : dispatcher_(dispatcher),
      handler_(BOOST_ASIO_MOVE_CAST(Handler)(handler))
  {
  }
  ...
  void operator()()
  {
    dispatcher_.dispatch(BOOST_ASIO_MOVE_CAST(Handler)(handler_));
  }
```
* from the above, `strand::wrap` is used to create a `wrapped_handler` object that, once invoked, will call `strand::dispatch`

### example 1
```cpp
#include <iostream>
#include <boost/asio.hpp>

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING 1

int main()
{
    boost::asio::io_service ios;
    boost::asio::io_service::strand strand(ios);

    ios.post(strand.wrap([]() { std::cout << "1" << std::endl; }));
    // boost::asio::post(strand, []() { std::cout << "2" << std::endl; });

    ios.run_one();
    // ios.run_one();

    return 0;
}
```
* output
```
$ g++ strand_behavior.cpp -lboost_system -lpthread -lboost_thread -D BOOST_ASIO_ENABLE_HANDLER_TRACKING
$ ./a.out
@asio|1719913270.728956|0*1|io_context@0x7ffc32038d20.post                    // post a `wrapped_handler` handler
@asio|1719913270.729015|>1|                                                   // redirect to strand::dispatch
@asio|1719913270.729020|1*2|strand@0x55d7043c86d0.dispatch                    // strand::dispatch
@asio|1719913270.729027|>2|
1
@asio|1719913270.729050|<2|
@asio|1719913270.729052|<1|
```
* the call sequence:
    * the `ios.run_one()` invokes the `strand::wrap` wrapped handler, which in turn call `strand::dispatch`.
    * `strand::dispatch` queues the original handler, and calls `io_context::dispatch`
    * `io_context::dispatch` calls the original handler right away as:
        * the caller is from the `io_context` itself
        * the original handler is the first && only handler in the strand's queue

### example 2
```cpp
#include <iostream>
#include <boost/asio.hpp>

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING 1


int main()
{
    boost::asio::io_service ios;
    boost::asio::io_service::strand strand(ios);

    ios.post(strand.wrap([&strand]() { std::cout << "1" << " " << strand.running_in_this_thread() << std::endl; }));     // handler #1
    boost::asio::post(strand, []() { std::cout << "2" << std::endl; });                                                  // handler #2

    ios.run_one();
    ios.run_one();
    ios.run_one();

    return 0;
}
```
* output
```
$ ./a.out
@asio|1719917225.368237|0*1|io_context@0x7ffcb107f810.post
@asio|1719917225.368290|0*2|strand@0x5648d6d4b6d0.post
@asio|1719917225.368294|>1|
@asio|1719917225.368298|1*3|strand@0x5648d6d4b6d0.dispatch
@asio|1719917225.368304|<1|
@asio|1719917225.368309|>2|
2
@asio|1719917225.368329|<2|
@asio|1719917225.368332|>3|
1 1
@asio|1719917225.368341|<3|
```
* the call sequence:
    * the first `ios.run_one()` invokes the `strand::wrap` wrapped handler, which in turn call `strand::dispatch`.
    * `strand::dispatch` queues the handler #1, and calls `io_context::dispatch`
    * `io_context::dispatch` doesn't call the handler #1 right away because 
        * the handler #2 is the first in the strand queue.
    * the second `ios.run_one()` invokes the handler #2 as it is the first handler in the strand queue.
    * the third `ios.run_one()` invokes the handler #1 as it is the first handler && only handler in the strand queue.

## references
* https://stackoverflow.com/questions/38136537/boostasiostranddispatchhandle-or-call-the-handle-directly
* https://stackoverflow.com/questions/2326588/boost-asio-io-service-dispatch-vs-post
* https://www.crazygaze.com/blog/2016/03/17/how-strands-work-and-why-you-should-use-them/
