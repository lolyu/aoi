#include <iostream>
#include "mytime2.h"

Time::Time(int h, int m) : m_hours(h), m_minutes(m) {}

void Time::addMin(int m)
{
    m_minutes += m;
    m_hours += m_minutes / 60;
    m_minutes %= 60;
}

void Time::addHr(int h)
{
    m_hours += h;
}

void Time::reset(int h, int m)
{
    m_hours = h;
    m_minutes = m;
}

int Time::getTotalMins() const
{
    int totalMins;
    totalMins = m_minutes + m_hours * 60;
}

Time Time::operator+(const Time &t) const
{
    Time sum = *this;
    sum.addMin(t.m_minutes);
    sum.addHr(t.m_hours);
    return sum;
}

Time Time::operator-(const Time &t) const
{
    int diffMins = this->getTotalMins() - t.getTotalMins();
    return Time(diffMins / 60, diffMins % 60);
}

Time Time::operator*(double mult) const
{
    long totalMins = this->getTotalMins() * mult;
    return Time(totalMins / 60, totalMins % 60);
}

void Time::show() const
{
    std::cout << m_hours << " hours, " << m_minutes << " minutes.";
}

std::ostream &operator<<(std::ostream &out, const Time &t)
{
    out << t.m_hours << " hours, " << t.m_minutes << " minutes.";
    return out;
}
