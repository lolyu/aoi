#include "widget.h"

class Widget::WidgetImpl
{
public:
    WidgetImpl(int n) : n(n) {}

    void draw(const Widget &w)
    {
        if (w.shown())
        {
            std::cout << "draw a widget: " << n << std::endl;
        }
    }

    void draw(const Widget &w) const
    {
        if (w.shown())
        {
            std::cout << "draw a const widget: " << n << std::endl;
        }
    }
private:
    int n;
};

Widget::Widget() = default;

Widget::Widget(int n) : pImpl(std::make_unique<WidgetImpl>(n)) {}

Widget::Widget(Widget &&) = default;

Widget::~Widget() = default;

Widget &Widget::operator=(Widget &&) = default;

void Widget::draw()
{
    pImpl->draw(*this);
}

void Widget::draw() const
{
    pImpl->draw(*this);
}
