#ifndef _STUDENT_H_
#define _STUDENT_H_

#include <iostream>
#include <string>
#include <valarray>

class Student
{

private:
    typedef std::valarray<double> ArrayDb;

public:
    Student() : mName("NONE"), mScores() {}

    explicit Student(const std::string &s) : mName(s), mScores() {}

    explicit Student(int n) : mName("NONE"), mScores(n) {}

    Student(const std::string &s, int n) : mName(s), mScores(n) {}

    Student(const std::string &s, const ArrayDb &a) : mName(s), mScores(a) {}

    Student(const char *str, const double *pd, int n) : mName(str), mScores(pd, n) {}

    ~Student() = default;

    double Average() const;

    const std::string &Name() const;

    double &operator[](int i);

    double operator[](int i) const;

    friend std::istream &operator>>(std::istream &is, Student &stu);

    friend std::ostream &operator<<(std::ostream &os, const Student &stu);

    friend std::istream &getline(std::istream &is, Student &stu);

private:
    std::ostream &arr_out(std::ostream &os) const;

private:
    std::string mName;
    ArrayDb mScores;
};

#endif