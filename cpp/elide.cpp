#include <iostream>

// in C++ 17, return value is always elided (in-place constructed), else moved, else copied
// $ g++ elide.cpp
// $ ./a.out      
// Widget()
// ~Widget()
// $ g++ -fno-elide-constructors elide.cpp                                                                                                             1 ↵
// $ ./a.out                              
// 0 Widget()
// 0 Widget(Widget &&)
// 0 ~Widget()
// 0 ~Widget()

template <typename T, size_t N>
class Widget
{
public:
    Widget() : _data(new T[N])
    {
        _self_index = _index++;
        std::cout << _self_index << " Widget()" << std::endl;
    }

    Widget(const Widget &other) : Widget()
    {
        std::cout << _self_index << " Widget(const Widget &)" << std::endl;
        std::copy(other._data, other._data + N, _data);
    }

    Widget(Widget &&other) noexcept : _data(other._data)
    {
        _self_index = _index++;
         std::cout << _self_index << " Widget(Widget &&)" << std::endl;
        other._data = nullptr;
    }

    Widget &operator=(Widget other)
    {
         std::cout << _self_index << " Widget &operator=(const Widget &)" << std::endl;
        swap(other);
        return *this;
    }

    ~Widget()
    {
         std::cout << _self_index << " ~Widget()" << std::endl;
        if (_data != nullptr)
            delete[] _data;
    }

private:
    void swap(Widget &other)
    {
        std::swap(_data, other._data);
    }

private:
    T *_data;
    size_t _self_index;
    static size_t _index;
};

template <typename T, size_t N>
size_t Widget<T, N>::_index = 0;

auto make_int10()
{
    Widget<int, 10> w;
    return w;
}

int main()
{
    auto w = make_int10();
    return 0;
}
