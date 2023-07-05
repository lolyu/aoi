#include <iostream>
#include <string>

using namespace std;

class Distance
{
public:
    static const long double km_per_mile;

    long double get_kilometers() { return m_kilometers; }

    Distance operator+(const Distance &other) { return Distance(m_kilometers + other.m_kilometers); }

private:
    friend Distance operator""_km(long double val);
    friend Distance operator""_mi(long double val);

    explicit Distance(long double val) : m_kilometers(val) {}

    long double m_kilometers{0};
};

const long double Distance::km_per_mile = 1.609344L;

Distance operator""_km(long double val)
{
    return Distance(val);
}

Distance operator""_mi(long double val)
{
    return Distance(val * Distance::km_per_mile);
}

int main()
{
    Distance d0{100.0_km};

    Distance d1{10.0_mi};

    Distance d2 = d0 + d1;

    cout << d2.get_kilometers() << endl;
    return 0;
}
