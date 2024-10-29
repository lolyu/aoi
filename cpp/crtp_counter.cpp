#include <iostream>

template <typename T>
class Counter
{
public:
    Counter()
    {
        ++mCreated;
        ++mAlive;
    }

    ~Counter()
    {
        --mAlive;
    }

    static size_t mCreated;
    static size_t mAlive;
};

template <typename T>
size_t Counter<T>::mCreated = 0;

template <typename T>
size_t Counter<T>::mAlive = 0;

class A : public Counter<A> {};
class B : public Counter<B> {};

int main()
{
    A a0, a1, a2;
    B b0, b1;

    {
        A a3;
        std::cout << A::mCreated << std::endl;
        std::cout << A::mAlive << std::endl;
    }

    std::cout << A::mCreated << std::endl;
    std::cout << A::mAlive << std::endl;
    std::cout << B::mCreated << std::endl;
    std::cout << B::mAlive << std::endl;

    return 0;
}
