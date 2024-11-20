#include <iostream>
#include <cstring>

static const int signature = 0xDEADBEEF;
typedef unsigned char Byte;

void *operator new(std::size_t size)
{
    size_t real_size = size + 2 * sizeof(int);
    void *mem = std::malloc(real_size);
    if (!mem)
    {
        throw std::bad_alloc();
    }

    *(static_cast<int *>(mem)) = signature;
    *(reinterpret_cast<int *>(static_cast<Byte *>(mem) + sizeof(int) + size)) = signature;

    return static_cast<Byte *>(mem) + sizeof(int);
}

void operator delete(void *p)
{
    if (!p)
    {
        return;
    }

    void *mem = static_cast<Byte *>(p) - sizeof(int);
    free(mem);
}

int main()
{
    void *p = nullptr;
    p = new int(2);
    delete p;
    return 0;
}
