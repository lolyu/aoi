#include <iostream>
#include "stonewt.h"

using namespace std;

void display(const Stonewt &st, int n);
double sum(double a, double b);

int main()
{
    Stonewt incognito(275);
    Stonewt wolfe(285.7);
    Stonewt taft(21, 8);
    incognito.show_stn();
    wolfe.show_stn();
    taft.show_lbs();
    incognito = Stonewt(276.8);
    taft = Stonewt(325);
    incognito.show_stn();
    taft.show_lbs();
    display(taft, 2);
    display(Stonewt(422), 2);
    // constructor that takes one argument will act to convert the type of the argument to the
    // class object
    // this type of conversion only happens for function arguments
    // display(200.20, 2);
    // operator typeName() const will act as conversion function to convert class
    // objects to type typeName
    sum(taft, 200);
    // res0 is a double
    auto res0 = 200.20 + taft;
    // Stonewt res1 = 200.20 + taft;
    cout << res0 << endl;
    // res1.show_lbs();
    return 0;
}

void display(const Stonewt &st, int n)
{
    for (int i = 0; i < n; ++i)
    {
        cout << "Wow! ";
        st.show_stn();
    }
}

double sum(double a, double b)
{
    return a + b;
}
