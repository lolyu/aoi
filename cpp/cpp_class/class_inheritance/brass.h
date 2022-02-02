#ifndef _BRASS_H_
#define _BRASS_H_
#include <string>

class Brass
{
public:
    Brass(const std::string &s = "NONE", long an = -1, double bal = 0.0);
    void Deposit(double amt);
    double Balance() const;
    virtual void Withdraw(double amt);
    virtual void ViewAccount() const;
    // always need a virtual destructor, when delete is called to a pointer that points to a base class, the delete will
    // call the corresponding destructor based on the type of the pointed object instead of the pointer type.
    virtual ~Brass() {}

private:
    std::string mFullName;
    long mAccountNumber;
    double mBalance;
};

class BrassPlus : public Brass
{
public:
    BrassPlus(const std::string &s = "NONE", long an = -1, double bal = 0.0, double ml = 500, double r = 0.11125);
    BrassPlus(const Brass &ba, double ml = 500, double r = 0.11125);

    void ResetMax(double m) { mMaxLoan = m; }
    void ResetRate(double r) { mRate = r; }
    void ResetOwes() { mOwesBank = 0; }

    virtual void Withdraw(double amt);
    virtual void ViewAccount() const;

private:
    double mMaxLoan;
    double mRate;
    double mOwesBank;
};

#endif