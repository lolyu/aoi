#include <iostream>

namespace widget
{

class Widget
{
public:
    Widget(int i = 0) : pImpl(new WidgetImpl)
    {
        std::cout << "Widget::Widget(int i)" << ' ' << pImpl << std::endl;
        pImpl->i = i;
    }
    ~Widget()
    {
        std::cout << "Widget::~Widget()" << ' ' << pImpl->i << std::endl;
        std::cout << pImpl << std::endl;
        delete pImpl;
    }
    Widget(const Widget &w) : Widget(w.pImpl->i)
    {
        std::cout << "Widget::Widget(const Widget &w)" << std::endl;
    }

    void swap(Widget &other)
    {
        std::cout << "Widget::swap(Widget &other)" << std::endl;

        // right way to do
        // enable ADL
        using std::swap;
        swap(this->pImpl, other.pImpl);

        // bad way to do
        // this always try to use the swap in the std (including any template specialization)
        // this will not enable ADL
        // std::swap(this->pImpl, other.pImpl);
    }

    // Widget &operator=(Widget w)
    // {
    //     std::cout << "Widget::operator=" << std::endl;
    //     swap(w);
    //     return *this;
    // }
    Widget &operator=(const Widget &rhs)
    {
        *pImpl = *(rhs.pImpl);
    }

    void show() const
    {
        std::cout << "Widget: " << pImpl->i << ' ' << pImpl << std::endl;
    }

private:
    struct WidgetImpl
    {
        int i;
    };

private:
    WidgetImpl *pImpl;
};

};

namespace std
{

template <>
void swap<widget::Widget>(widget::Widget &lhs, widget::Widget &rhs)
{
    lhs.swap(rhs);
}

};


void swap_and_display(widget::Widget &a, widget::Widget &b)
{
    a.show();
    b.show();

    using std::swap;
    swap(a, b);

    a.show();
    b.show();
}



int main()
{
    widget::Widget w0(10);
    widget::Widget w1(20);
    widget::Widget w2(w0);  // w2 = 10
    w0.show();
    w1.show();
    w2.show();
    w2 = w1;                // w2 = 20, w1 = 10
    w0.show();
    w1.show();
    w2.show();
    swap_and_display(w0, w1);
    w0.show();
    w1.show();
    w2.show();
    return 0;
}
