#include <iostream>
#include <typeinfo>

class Base
{
public:
    int x = 10;
};
class Derived : public Base {};
class DerivedDerived : public Derived {};

template <typename T>
class Wrapper
{
public:
    Wrapper(T *tp) : tp_(tp)
    {
        std::cout << "Init " << typeid(T).name() << " wrapper with " << typeid(tp).name() << std::endl;
    }

    template <typename U>
    Wrapper(U *tp) : tp_(tp)
    {
        std::cout << "Init " << typeid(T).name() << " wrapper with " << typeid(tp).name() << std::endl;
    }

    template <typename U>
    Wrapper(const Wrapper<U> &other) : tp_(other.get())
    {
        std::cout << typeid(T).name() << " wrapper copy constructor from " << typeid(other).name() << std::endl;
    }

    template <typename U>
    Wrapper<T> &operator=(const Wrapper<U> &other)
    {
        std::cout << typeid(T).name() << " wrapper assignment from " << typeid(other).name() << std::endl;
        tp_ = other.get();
        return *this;
    }

    ~Wrapper()
    {
        std::cout << "Destroy wrapper" << std::endl;
    }

    T* get() const
    {
        return tp_;
    }

private:
    T *tp_;
};

int main()
{
    Base *bp = new Base();
    Derived *dp = new Derived();

    std::cout << bp << std::endl;
    std::cout << dp << std::endl;

    Wrapper<Base> bpw0(bp);
    Wrapper<Base> bpw1(dp);
    Wrapper<Derived> dpw(dp);
    Wrapper<Base> bpw2(dpw);
    bpw0 = dpw;

    std::cout << bpw0.get() << std::endl;
    std::cout << bpw1.get() << std::endl;
    std::cout << dpw.get() << std::endl;
    std::cout << bpw2.get() << std::endl;

    delete bp;
    delete dp;
    return 0;
}
