#include <math.h>

double squareRoot(const double a)
{
    double b = sqrt(a);
    if (b != b)
    {
        return -1.0;
    }
    else
    {
        return sqrt(a);
    }
}

class Time
{
public:
    Time(int h = 0, int m = 0)
    {
        hour = h;
        minute = m;
    }

    Time operator+(const Time &t)
    {
        Time ret(hour, minute);
        ret.hour += t.hour;
        ret.minute += t.minute;
        ret.hour += ret.minute / 60;
        ret.minute %= 60;
        return ret;
    }

    int getHour() { return hour; }
    int getMin() { return minute; }

private:
    int hour;
    int minute;
};
