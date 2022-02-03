#include <iostream>
#include <iomanip>
#include "acctabc.h"

AcctABC::AcctABC(const std::string &s, long an, double bal) : mFullName(s), mAccountNumber(an), mBalance(bal) {}

void AcctABC::Deposit(double amt)
{
    if (amt < 0)
    {
        std::cout << "Negative deposit not allowed; deposit is cancelled." << std::endl;
    }
    else
    {
        mBalance += amt;
    }
}

void AcctABC::Withdraw(double amt)
{
    mBalance -= amt;
}

AcctABC::Formatting AcctABC::SetFormat() const
{
    Formatting f;
    f.flag = std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
    f.pr = std::cout.precision(2);
    return f;
}

void AcctABC::Restore(AcctABC::Formatting &f) const
{
    std::cout.setf(f.flag, std::ios_base::floatfield);
    std::cout.precision(f.pr);
}

void Brass::Withdraw(double amt)
{
    if (amt < 0)
    {
        std::cout << "Withdrawal amount must be positive; withdrawal canceled." << std::endl;
    }
    else if (amt > Balance())
    {
        Formatting f = SetFormat();
        std::cout << "Withdrawal amount of $" << amt << " exceeds your balance." << std::endl
                  << "Withdrawal cancelled." << std::endl;
        Restore(f);
    }
    else
    {
        AcctABC::Withdraw(amt);
    }
}

void Brass::ViewAccount() const
{
    Formatting f = SetFormat();
    std::cout << "Client: " << FullName() << std::endl;
    std::cout << "Account Number: " << AccountNumber() << std::endl;
    std::cout << "Balance: $" << Balance() << std::endl;
    Restore(f);
}

BrassPlus::BrassPlus(const std::string &s, long an, double bal, double ml, double r) : Brass(s, an, bal), mMaxLoan(ml), mRate(r), mOwesBank(0.0) {}

BrassPlus::BrassPlus(const Brass &ba, double ml, double r) : Brass(ba), mMaxLoan(ml), mRate(r), mOwesBank(0.0) {}

void BrassPlus::ViewAccount() const
{
    Formatting f = SetFormat();
    std::cout << "BrassPlus Client: " << FullName() << std::endl;
    std::cout << "Account Number: " << AccountNumber() << std::endl;
    std::cout << "Balance: $" << Balance() << std::endl;
    std::cout << "Maximum loan: $" << mMaxLoan << std::endl;
    std::cout << "Owed to bank: $" << mOwesBank << std::endl;
    std::cout.precision(3);
    std::cout << "Loan rate: " << std::setw(4) << 100 * mRate << "%" << std::endl;
    Restore(f);
}

void BrassPlus::Withdraw(double amt)
{
    Formatting f = SetFormat();
    double bal = Balance();
    if (amt < 0)
    {
        std::cout << "Withdrawal amount must be positive; withdrawal canceled." << std::endl;
    }
    else if (amt <= bal)
    {
        AcctABC::Withdraw(amt);
    }
    else if (amt <= bal + mMaxLoan - mOwesBank)
    {
        double advance = amt - bal;
        double charge = mRate * advance;
        mOwesBank += advance + charge;
        std::cout << "Bank advance: $" << advance << std::endl;
        std::cout << "Finance charge: $" << charge << std::endl;
        AcctABC::Withdraw(bal);
    }
    else
    {
        std::cout << "Credit limit exceeded. Transaction cancelled." << std::endl;
    }
    Restore(f);
}
