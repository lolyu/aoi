#ifndef _WIDGET_H_
#define _WIDGET_H_
#include <experimental/propagate_const>
#include <iostream>
#include <memory>

class Widget
{
public:
    Widget();

    explicit Widget(int);

    ~Widget();

    Widget(Widget &&);

    Widget(const Widget &) = delete;

    Widget &operator=(Widget &&);

    Widget &operator=(const Widget &) = delete;

public:
    void draw() const;

    void draw();

    bool shown() const { return true; }

private:
    class WidgetImpl;
    std::experimental::propagate_const<std::unique_ptr<WidgetImpl>> pImpl;
};


#endif
