#include "tabtenn0.h"
#include <iostream>

TableTennisPlayer::TableTennisPlayer(const std::string &fn, const std::string &ln, bool ht) : mFirstName(fn), mLastName(ln), mHasTable(ht)
{
    // member initializer list will call its member's constructor directly
    mFirstName = fn;
    mLastName = ln;
    mHasTable = ht;
}

void TableTennisPlayer::Name() const
{
    std::cout << mLastName << ", " << mFirstName;
}

RatedPlayer::RatedPlayer(unsigned int r, const std::string &fn, const std::string &ln, bool ht) : TableTennisPlayer(fn, ln, ht), mRating(r) {}

RatedPlayer::RatedPlayer(unsigned int r, const TableTennisPlayer &tp) : TableTennisPlayer(tp), mRating(r) {}
