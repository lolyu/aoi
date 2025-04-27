#include <iostream>
#include <queue>
#include <atomic>
#include <cassert>
#include <new>
#include <chrono>
#include <thread>

#define CACHE_LINE_SIZE 64
std::atomic<size_t> g_elem_count(0);

enum RING_SYNC_TYPE
{
    RING_SYNC_ST,
    RING_SYNC_MT,
};

template <typename T, size_t capacity, RING_SYNC_TYPE sync_type>
class RingBuffer
{
public:
    static_assert((capacity && !(capacity & (capacity - 1))), "capacity must be power of 2.");

    RingBuffer() {}

    ~RingBuffer() {}

    RingBuffer(const RingBuffer &) = delete;
    RingBuffer &operator=(const RingBuffer &) = delete;

    template <typename U>
    bool push(U &&value) noexcept
    {
        // bookkeeping
        bool success = false;
        uint32_t prod_head, prod_head_next;
        uint32_t cons_tail;
        uint32_t available = 0;

        do {
            prod_head = _prod.head.load(std::memory_order_acquire);
            cons_tail = _cons.tail.load(std::memory_order_acquire);

            available = (capacity + cons_tail - prod_head);
            if (available == 0)
            {
                return false;
            }

            prod_head_next = prod_head + 1;
            if (sync_type == RING_SYNC_ST)
            {
                _prod.head.store(prod_head_next, std::memory_order_release);
                success = true;
            } else if (sync_type == RING_SYNC_MT)
            {
                success = _prod.head.compare_exchange_strong(prod_head, prod_head_next, std::memory_order_acq_rel);
            } else {
                assert(false);
            }
        } while (!success);

        // move data element
        new (&_data[prod_head & (capacity - 1)]) T(std::forward<U>(value));

        // align producer head && tail
        success = false;
        do {
            if (sync_type == RING_SYNC_ST)
            {
                _prod.tail.store(prod_head_next, std::memory_order_release);
                success = true;
            } else if (sync_type == RING_SYNC_MT)
            {
                success = _prod.tail.compare_exchange_strong(prod_head, prod_head_next, std::memory_order_acq_rel);
            } else {
                assert(false);
            }
        } while (!success);

        return true;
    }

    bool pop(T &value) noexcept
    {
        bool success = false;
        uint32_t cons_head, cons_head_next;
        uint32_t prod_tail;
        uint32_t length = 0;

        do {
            cons_head = _cons.head.load(std::memory_order_acquire);
            prod_tail = _prod.tail.load(std::memory_order_acquire);

            length = (prod_tail - cons_head);
            if (length == 0)
            {
                return false;
            }

            cons_head_next = cons_head + 1;
            if (sync_type == RING_SYNC_ST)
            {
                _cons.head.store(cons_head_next, std::memory_order_release);
                success = true;
            } else if (sync_type == RING_SYNC_MT)
            {
                success = _cons.head.compare_exchange_strong(cons_head, cons_head_next, std::memory_order_acq_rel);
            } else {
                assert(false);
            }

        } while (!success);

        value = std::move(*reinterpret_cast<T *>(&_data[cons_head & (capacity - 1)]));
        reinterpret_cast<T *>(&_data[cons_head & (capacity - 1)])->~T();

        success = false;
        do {
            if (sync_type == RING_SYNC_ST)
            {
                _cons.tail.store(cons_head_next, std::memory_order_release);
                success = true;
            } else if (sync_type == RING_SYNC_MT)
            {
                success = _cons.tail.compare_exchange_strong(cons_head, cons_head_next, std::memory_order_acq_rel);
            } else {
                assert(false);
            }

        } while (!success);
        return true;
    }

private:
    typedef struct _head_tail
    {
        std::atomic<uint32_t> head;
        std::atomic<uint32_t> tail;

        _head_tail(uint32_t h = 0, uint32_t t = 0) : head(h), tail(t) {}
    } head_tail;

private:
    alignas(CACHE_LINE_SIZE) head_tail _prod;
    alignas(CACHE_LINE_SIZE) head_tail _cons;
    alignas(CACHE_LINE_SIZE) std::aligned_storage_t<sizeof(T), alignof(T)> _data[capacity];
};

void test_threaded_spsc() {
    RingBuffer<size_t, 1024, RING_SYNC_ST> buffer;
    g_elem_count.store(0);
    const size_t num_items = 10000000;

    auto producer = [&buffer, num_items]() {
        for (size_t i = 0; i < num_items; ++i) {
            while (!buffer.push(i)) {
            }
            g_elem_count.fetch_add(1, std::memory_order_relaxed);
        }
    };

    auto consumer = [&buffer, num_items]() {
        for (size_t i = 0; i < num_items; ++i) {
            size_t value;
            while (!buffer.pop(value)) {
            }
            assert(value == i);
            g_elem_count.fetch_sub(1, std::memory_order_relaxed);
        }
    };

    std::thread producer_thread(producer);
    std::thread consumer_thread(consumer);

    producer_thread.join();
    consumer_thread.join();

    assert(g_elem_count.load() == 0);
    std::cout << "test_threaded SPSC passed, g_elem_count = " << g_elem_count.load() << std::endl;
}

void test_threaded_mpmc()
{
    RingBuffer<size_t, 1024, RING_SYNC_MT> buffer;
    g_elem_count.store(0);
    const size_t num_items = 10000000;

    auto producer = [&buffer, num_items]() {
        for (size_t i = 0; i < num_items; ++i) {
            while (!buffer.push(i)) {
            }
            g_elem_count.fetch_add(1, std::memory_order_relaxed);
        }
    };

    auto consumer = [&buffer, num_items]() {
        for (size_t i = 0; i < num_items; ++i) {
            size_t value;
            while (!buffer.pop(value)) {
            }
            g_elem_count.fetch_sub(1, std::memory_order_relaxed);
        }
    };

    std::thread producer_thread0(producer);
    std::thread producer_thread1(producer);
    std::thread consumer_thread0(consumer);
    std::thread consumer_thread1(consumer);

    producer_thread0.join();
    producer_thread1.join();
    consumer_thread0.join();
    consumer_thread1.join();

    assert(g_elem_count.load() == 0);
    std::cout << "test_threaded MPMC passed, g_elem_count = " << g_elem_count.load() << std::endl;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    test_threaded_mpmc();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << duration.count() << std::endl;
    return 0;
};
