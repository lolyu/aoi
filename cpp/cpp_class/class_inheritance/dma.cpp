#include <cstring>
#include "dma.h"

BaseDMA::BaseDMA(const char *l, int r)
{
    label = new char[std::strlen(l) + 1];
    std::strcpy(label, l);
    rating = r;
}

BaseDMA::BaseDMA(const BaseDMA &rs)
{
    label = new char[std::strlen(rs.label) + 1];
    std::strcpy(label, rs.label);
    rating = rs.rating;
}

BaseDMA::~BaseDMA()
{
    delete[] label;
}

BaseDMA &BaseDMA::operator=(const BaseDMA &rs)
{
    if (&rs == this)
    {
        return *this;
    }
    delete[] label;
    label = new char[std::strlen(rs.label) + 1];
    std::strcpy(label, rs.label);
    rating = rs.rating;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const BaseDMA &rs)
{
    os << "Label: " << rs.label << std::endl;
    os << "Rating: " << rs.rating << std::endl;
}

HasDMA::HasDMA(const char *l, const char *s, int r) : BaseDMA(l, r)
{
    style = new char[std::strlen(s) + 1];
    std::strcpy(style, s);
}

HasDMA::HasDMA(const HasDMA &rs) : BaseDMA(rs)
{
    style = new char[std::strlen(rs.style) + 1];
    std::strcpy(style, rs.style);
}

HasDMA::~HasDMA()
{
    delete[] style;
}

HasDMA &HasDMA::operator=(const HasDMA &rs)
{
    if (&rs == this)
    {
        return *this;
    }
    BaseDMA::operator=(rs);
    delete[] style;
    style = new char[std::strlen(rs.style) + 1];
    std::strcpy(style, rs.style);
    return *this;
}

std::ostream &operator<<(std::ostream &os, const HasDMA &rs)
{
    os << static_cast<const BaseDMA &>(rs);
    os << "Style: " << rs.style << std::endl;
}
