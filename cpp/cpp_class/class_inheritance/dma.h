#ifndef _DMA_H_
#define _DMA_H_
#include <iostream>

class BaseDMA
{
public:
    BaseDMA(const char *l = "NULL", int r = 0);
    BaseDMA(const BaseDMA &rs);
    virtual ~BaseDMA();
    BaseDMA &operator=(const BaseDMA &rs);
    friend std::ostream &operator<<(std::ostream &os, const BaseDMA &rs);

private:
    char *label;
    int rating;
};

class HasDMA : public BaseDMA
{
public:
    HasDMA(const char *l = "NULL", const char *s = "NULL", int r = 0);
    HasDMA(const HasDMA &rs);
    virtual ~HasDMA();
    HasDMA &operator=(const HasDMA &rs);
    friend std::ostream &operator<<(std::ostream &os, const HasDMA &rs);

private:
    char *style;
};

#endif