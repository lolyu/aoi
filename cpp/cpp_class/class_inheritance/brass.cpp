#include <iostream>
#include "brass.h"

using std::cout;
using std::endl;
using std::string;

typedef std::ios_base::fmtflags format;
typedef std::streamsize precis;

format setFormat();
void restore(format f, precis p);

Brass::Brass(const std::string &s = "NONE", long an = -1, double bal = 0.0) : mFullName(s), mAccountNumber(an), mBalance(bal) {}

void Brass::Deposit(double amt)
{
    if (amt < 0)
    {
        cout << "Negative deposit not allowed; deposit is cancelled." << endl;
    }
    else
    {
        mBalance += amt;
    }
}

double Brass::Balance() const
{
    return mBalance;
}

void Brass::Withdraw(double amt)
{
    if (amt < 0)
    {
        cout << "Withdrawal amount must be positive; withdrawal canceled." << endl;
    }
    else if (amt > mBalance)
    {
        format initialState = setFormat();
        precis prec = cout.precision(2);
        cout << "Withdrawal amount of $" << amt << " exceeds your balance." << endl
             << "Withdrawal cancelled." << endl;
        restore(initialState, prec);
    }
    else
    {
        mBalance -= amt;
    }
}

void Brass::ViewAccount() const
{
    format initialState = setFormat();
    precis prec = cout.precision(2);
    cout << "Client: " << mFullName << endl;
    cout << "Account Number: " << mAccountNumber << endl;
    cout << "Balance: $" << mBalance << endl;
    restore(initialState, prec);
}

BrassPlus::BrassPlus(const std::string &s, long an, double bal, double ml, double r) : Brass(s, an, bal), mMaxLoan(ml), mRate(r), mOwesBank(0.0) {}

BrassPlus::BrassPlus(const Brass &ba, double ml, double r) : Brass(ba), mMaxLoan(ml), mRate(r), mOwesBank(0.0) {}

void BrassPlus::Withdraw(double amt)
{
    format initialState = setFormat();
    precis prec = cout.precision(2);

    double bal = Balance();
    if (amt <= bal)
    {
        Brass::Withdraw(amt);
    }
    else if (amt <= bal + mMaxLoan - mOwesBank)
    {
        format initialState = setFormat();
        precis prec = cout.precision(2);
        double advance = amt - bal;
        mOwesBank += (1 + mRate) * advance;
        cout << "Bank advance: $" << advance << endl;
        cout << "Finance charge: $" << advance * mRate << endl;
        Brass::Withdraw(bal);
        restore(initialState, prec);
    }
    else
    {
        cout << "Credit limit exceeded. Transaction cancelled." << endl;
    }
}

void BrassPlus::ViewAccount() const
{
    ;
}

format setFormat()
{
    return cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
}

void restore(format f, precis p)
{
    cout.setf(f, std::ios_base::floatfield);
    cout.precision(p);
}
