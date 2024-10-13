#include "widget_util.h"

void test_widget()
{
    Widget w0(100);
    const Widget w1(101);
    w0.draw();
    w1.draw();
}
