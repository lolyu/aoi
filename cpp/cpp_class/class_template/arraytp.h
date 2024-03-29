#ifndef _ARRAYTP_H_
#define _ARRAYTP_H_

#include <cstdlib>
#include <iostream>
#include <sstream>

template <typename T, int n>
class ArrayTP {
private:
    T ar[n];

public:
    ArrayTP(){};
    explicit ArrayTP(const T &v);
    virtual T &operator[](int i);
    virtual T operator[](int i) const;
};

template <typename T, int n>
ArrayTP<T, n>::ArrayTP(const T &v)
{
    for (int i = 0; i < n; ++i) {
        ar[i] = v;
    }
}

template <typename T, int n>
T &ArrayTP<T, n>::operator[](int i)
{
    if (i < 0 || i >= n) {
        std::ostringstream oss;
        oss << "Index " << i << " out of range.";
        throw std::out_of_range(oss.str());
    } else {
        return ar[i];
    }
}

template <typename T, int n>
T ArrayTP<T, n>::operator[](int i) const
{
    if (i < 0 || i >= n) {
        std::ostringstream oss;
        oss << "Index " << i << " out of range.";
        throw std::out_of_range(oss.str());
    } else {
        return ar[i];
    }
}

#endif