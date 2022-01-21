#ifndef _TABTENN0_H_
#define _TABTENN0_H_
#include <string>

class TableTennisPlayer
{
public:
    TableTennisPlayer(const std::string &fn = "none", const std::string &ln = "none", bool ht = false);

    void Name() const;
    bool hasTable() const { return mHasTable; }
    void resetTable(bool v) { mHasTable = v; }

private:
    std::string mFirstName;
    std::string mLastName;
    bool mHasTable;
};

class RatedPlayer : public TableTennisPlayer
{
public:
    RatedPlayer(unsigned int r, const std::string &fn = "none", const std::string &ln = "none", bool ht = false);
    RatedPlayer(unsigned int r, const TableTennisPlayer &tp);
    unsigned int Rating() const { return mRating; }
    void ResetRating(unsigned int r) { mRating = r; }

private:
    unsigned int mRating;
};

#endif