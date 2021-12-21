#ifndef MYTIME2_H_
#define MYTIME2_H_
#include <iostream>

class Time
{
public:
    Time(int h = 0, int m = 0);
    void addMin(int m);
    void addHr(int h);
    void reset(int h = 0, int m = 0);
    Time operator+(const Time &t) const;
    Time operator-(const Time &t) const;
    Time operator*(double n) const;
    void show() const;

    friend Time operator*(double m, const Time &t) { return t * m; }
    friend std::ostream &operator<<(std::ostream &out, const Time &t);

private:
    int getTotalMins() const;
    int m_hours;
    int m_minutes;
};

#endif