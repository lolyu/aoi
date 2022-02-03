#ifndef _ACCTABC_H_
#define _ACCTABC_H_
#include <iostream>
#include <string>

class AcctABC
{
public:
    AcctABC(const std::string &s = "NONE", long an = -1, double bal = 0.0);
    void Deposit(double amt);
    virtual void Withdraw(double amt) = 0;
    virtual void ViewAccount() const = 0;
    double Balance() const { return mBalance; }
    virtual ~AcctABC() {}

protected:
    struct Formatting
    {
        std::ios_base::fmtflags flag;
        std::streamsize pr;
    };
    const std::string &FullName() const { return mFullName; }
    long AccountNumber() const { return mAccountNumber; }
    Formatting SetFormat() const;
    void Restore(Formatting &f) const;

private:
    std::string mFullName;
    long mAccountNumber;
    double mBalance;
};

class Brass : public AcctABC
{
public:
    Brass(const std::string &s = "NONE", long an = -1, double bal = 0.0) : AcctABC(s, an, bal) {}
    virtual void Withdraw(double amt);
    virtual void ViewAccount() const;
    virtual ~Brass() {}
};

class BrassPlus : Brass
{
public:
    BrassPlus(const std::string &s = "NONE", long an = -1, double bal = 0.0, double ml = 500, double r = 0.10);
    BrassPlus(const Brass &ba, double ml = 500, double r = 0.1);
    virtual void Withdraw(double amt);
    virtual void ViewAccount() const;
    void ResetMax(double m) { mMaxLoan = m; }
    void ResetRate(double r) { mRate = r; }
    void ResetOwes() { mOwesBank = 0; }

private:
    double mMaxLoan;
    double mRate;
    double mOwesBank;
};

#endif