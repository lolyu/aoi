#include <iostream>
#include "stonewt.h"

using namespace std;

Stonewt::Stonewt()
{
    stone = pds_left = pounds = 0;
}

Stonewt::Stonewt(double lbs)
{
    stone = static_cast<int>(lbs) / Lbs_per_stn;
    pds_left = lbs - stone * Lbs_per_stn;
    pounds = lbs;
}

Stonewt::Stonewt(int stn, double lbs)
{
    stone = stn;
    pds_left = lbs;
    pounds = stone * Lbs_per_stn + lbs;
}

Stonewt::~Stonewt()
{
}

void Stonewt::show_stn() const
{
    cout << stone << " stone, " << pds_left << " pounds." << endl;
}

void Stonewt::show_lbs() const
{
    cout << pounds << " pounds." << endl;
}

Stonewt::operator int() const
{
    return int(pounds + 0.5);
}

Stonewt::operator double() const
{
    return pounds;
}

Stonewt Stonewt::operator+(const Stonewt &rs) const
{
    cout << "Using Stonewt::operator+" << endl;
    return Stonewt(pounds + rs.pounds);
}
