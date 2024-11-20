#include <iostream>
#include <cstring>

// https://stackoverflow.com/questions/7194127/how-should-i-write-iso-c-standard-conformant-custom-new-and-delete-operators#7194137

static const int signature = 0xDEADBEEF;
typedef unsigned char Byte;

class Base
{
public:
    static void *operator new(std::size_t size);
    static void operator delete(void *mem, std::size_t size);
private:
    int b = 10;
};

void *Base::operator new(std::size_t size)
{
    if (size != sizeof(Base))
    {
        return ::operator new(size);
    }

    std::cout << "void *Base::operator new(std::size_t size)" << std::endl;
    size_t real_size = size + 2 * sizeof(int);

    while (true)
    {
        void *mem = std::malloc(real_size);
        if (mem)
        {
            *(static_cast<int *>(mem)) = signature;
            *(reinterpret_cast<int *>(static_cast<Byte *>(mem) + sizeof(int) + size)) = signature;

            return static_cast<Byte *>(mem) + sizeof(int);
        }

        std::new_handler global_handler = std::set_new_handler(nullptr);
        std::set_new_handler(global_handler);

        if (global_handler)
        {
            (*global_handler)();
        }
        else
        {
            throw std::bad_alloc();
        }
    }
}

void Base::operator delete(void *p, std::size_t size)
{
    if (!p)
    {
        return;
    }
    if (size != sizeof(Base))
    {
        ::operator delete(p);
        return;
    }

    std::cout << "void Base::operator delete(void *, std::size_t)" << std::endl;
    void *mem = static_cast<Byte *>(p) - sizeof(int);
    std::free(mem);
    return;
}

class Derived : public Base
{
private:
    int d = 10;
};

int main()
{
    Base *bp = new Base;
    Derived *dp = new Derived;

    delete bp;
    delete dp;

    return 0;
}
