#include <atomic>
#include <cassert>
#include <iostream>
#include <new>
#include <thread>
#include <type_traits>
#include <chrono>

/*
Requirement:
1. SPSC

Key takeaways:
1. cache line align
2. perfect forwarding
3. atomic operations
*/


template <typename T, size_t capacity>
class RingBuffer
{
static_assert((capacity && !(capacity & (capacity - 1))), "capacity must be power of 2.");
public:
    RingBuffer() : _head(0), _tail(0) {}

    ~RingBuffer()
    {
        size_t head = _head.load(std::memory_order_acquire);
        size_t tail = _tail.load(std::memory_order_acquire);
        while (head != tail)
        {
            reinterpret_cast<T*>(&_data[head])->~T();
            head = next_pos(head);
        }
    }

    RingBuffer(const RingBuffer &) = delete;
    RingBuffer &operator=(const RingBuffer &) = delete;

    size_t size() const noexcept
    {
        size_t head = _head.load(std::memory_order_relaxed);
        size_t tail = _head.load(std::memory_order_relaxed);
        if (tail > head)
        {
            return tail - head;
        }
        else
        {
            return tail + capacity - head;
        }
    }

    bool full() const noexcept
    {
        return size() == capacity;
    }

    template <typename U>
    bool push(U &&value) noexcept
    {
        size_t tail = _tail.load(std::memory_order_relaxed);
        size_t next_tail = next_pos(tail);

        if (next_tail == _head.load(std::memory_order_acquire))
        {
            return false;
        }

        new (&_data[tail]) T(std::forward<U>(value));
        _tail.store(next_tail, std::memory_order_release);
        return true;
    }

    bool pop(T &value) noexcept
    {
        size_t tail = _tail.load(std::memory_order_acquire);
        size_t head = _head.load(std::memory_order_relaxed);
        if (head == tail)
        {
            return false;
        }

        value = std::move(*reinterpret_cast<T*>(&_data[head]));
        reinterpret_cast<T*>(&_data[head])->~T();

        _head.store(next_pos(_head), std::memory_order_release);
        return true;
    }

private:
    size_t next_pos(size_t pos)
    {
        return (pos + 1) & (capacity - 1);
    }

private:
    alignas(64) std::atomic<size_t> _head;
    alignas(64) std::atomic<size_t> _tail;
    alignas(64) std::aligned_storage_t<sizeof(T), alignof(T)> _data[capacity];
};

void test_threaded() {
    RingBuffer<int, 1024> buffer;
    const size_t num_items = 10000000;

    auto producer = [&buffer, num_items]() {
        for (size_t i = 0; i < num_items; ++i) {
            while (!buffer.push(i)) {
            }
        }
    };

    auto consumer = [&buffer, num_items]() {
        for (size_t i = 0; i < num_items; ++i) {
            int value;
            while (!buffer.pop(value)) {
            }
            assert(value == i);
        }
    };

    std::thread producer_thread(producer);
    std::thread consumer_thread(consumer);

    producer_thread.join();
    consumer_thread.join();

    std::cout << "test_threaded passed!" << std::endl;
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    test_threaded();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << duration.count() << std::endl;
    return 0;
}
