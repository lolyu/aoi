#include "student.h"

double Student::Average() const
{
    return mScores.size() == 0 ? 0 : mScores.sum() / mScores.size();
}

const std::string &Student::Name() const
{
    return mName;
}

double &Student::operator[](int i)
{
    return mScores[i];
}

double Student::operator[](int i) const
{
    return mScores[i];
}

std::ostream &Student::arr_out(std::ostream &os) const
{
    int i;
    int lim = mScores.size();
    if (lim > 0)
    {
        for (i = 0; i < lim; ++i)
        {
            os << mScores[i] << " ";
            if (i % 5 == 4)
            {
                os << std::endl;
            }
        }
    }
    else
    {
        os << " empty array ";
    }
    return os;
}

std::istream &operator>>(std::istream &is, Student &stu)
{
    is >> stu.mName;
    return is;
}

std::istream &getline(std::istream &is, Student &stu)
{
    getline(is, stu.mName);
    return is;
}

std::ostream &operator<<(std::ostream &os, const Student &stu)
{
    os << "Scores for " << stu.mName << ":" << std::endl;
    stu.arr_out(os);
    return os;
}

const std::string &StudentPriv::Name() const
{
    return static_cast<const std::string &>(*this);
}

double StudentPriv::Average() const
{
    if (ArrayDb::size() == 0)
    {
        return 0;
    }
    else
    {
        return ArrayDb::sum() / ArrayDb::size();
    }
}

double &StudentPriv::operator[](int i)
{
    return ArrayDb::operator[](i);
}

double StudentPriv::operator[](int i) const
{
    return ArrayDb::operator[](i);
}

std::ostream &StudentPriv::arr_out(std::ostream &os) const
{
    int i;
    int lim = ArrayDb::size();
    if (lim > 0)
    {
        for (i = 0; i < lim; ++i)
        {
            os << ArrayDb::operator[](i) << " ";
            if (i % 5 == 4)
            {
                os << std::endl;
            }
        }
    }
    else
    {
        os << " empty array ";
    }
    return os;
}

std::istream &operator>>(std::istream &is, StudentPriv &stu)
{
    is >> static_cast<std::string &>(stu);
    return is;
}

std::istream &getline(std::istream &is, StudentPriv &stu)
{
    getline(is, static_cast<std::string &>(stu));
    return is;
}

std::ostream &operator<<(std::ostream &os, const StudentPriv &stu)
{
    os << "Scores for " << static_cast<const std::string &>(stu) << ":" << std::endl;
    stu.arr_out(os);
    return os;
}
