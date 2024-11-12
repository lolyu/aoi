#include <iostream>

class NewHandlerHolder
{
public:
    explicit NewHandlerHolder(std::new_handler curr_handler) : curr_handler(curr_handler), prev_handler(std::set_new_handler(curr_handler))
    {}

    NewHandlerHolder(const NewHandlerHolder &) = delete;
    NewHandlerHolder &operator=(const NewHandlerHolder &) = delete;

    ~NewHandlerHolder()
    {
        std::set_new_handler(prev_handler);
    }

private:
    std::new_handler curr_handler = nullptr;
    std::new_handler prev_handler = nullptr;
};

template <typename T>
class PerClassHandlerMixIn
{
public:
    static std::new_handler set_new_handler(std::new_handler p);
    static void *operator new(std::size_t size);

private:
    static std::new_handler curr_handler;
};

template <typename T>
std::new_handler PerClassHandlerMixIn<T>::curr_handler = nullptr;

template <typename T>
std::new_handler PerClassHandlerMixIn<T>::set_new_handler(std::new_handler p)
{
    std::new_handler prev_handler = curr_handler;
    curr_handler = p;
    return prev_handler;
}

template <typename T>
void *PerClassHandlerMixIn<T>::operator new(std::size_t size)
{
    std::cout << "PerClassHandler::operator new" << std::endl;
    NewHandlerHolder nhh(curr_handler);
    return ::operator new(size);
}

class Base {};

class Derived : public Base, public PerClassHandlerMixIn<Derived>
{
private:
    double matrix[10000000000];
};

void out_of_mem()
{
    std::cerr << "Unable to allocate memory." << std::endl << std::flush;
    std::abort();
}

int main()
{
    Derived::set_new_handler(&out_of_mem);
    Derived *dp = new Derived();
    return 0;
}
