#ifndef _PAIR_H_
#define _PAIR_H_

#include <string>

template <typename T0, typename T1>
class PairTP {
private:
    T0 a;
    T1 b;

public:
    PairTP() = default;
    PairTP(const T0 &t0, const T1 &t1)
        : a(t0), b(t1) {}
    T0 &first();
    T1 &second();
    T0 first() const;
    T1 second() const;
};

template <typename T0, typename T1>
T0 &PairTP<T0, T1>::first()
{
    return a;
}

template <typename T0, typename T1>
T1 &PairTP<T0, T1>::second()
{
    return b;
}

template <typename T0, typename T1>
T0 PairTP<T0, T1>::first() const
{
    return a;
}

template <typename T0, typename T1>
T1 PairTP<T0, T1>::second() const
{
    return b;
}

//  explicit instantiation
template class PairTP<int, int>;

// explicit specialization
template <>
class PairTP<const char *, const char *> {
private:
    std::string a;
    std::string b;

public:
    PairTP() = default;
    PairTP(const char *a, const char *b)
        : a(a), b(b) {}
    std::string &first();
    std::string &second();
    std::string first() const;
    std::string second() const;
};

std::string &PairTP<const char *, const char *>::first()
{
    return a;
}

std::string &PairTP<const char *, const char *>::second()
{
    return b;
}

std::string PairTP<const char *, const char *>::first() const
{
    return a;
}

std::string PairTP<const char *, const char *>::second() const
{
    return b;
}

// partial specialization
template <typename T>
class PairTP<T, T>;

#endif