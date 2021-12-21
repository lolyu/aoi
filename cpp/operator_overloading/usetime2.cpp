#include <iostream>
#include "mytime2.h"

int main()
{
    using namespace std;
    Time weeding(4, 35);
    Time waxing(2, 47);
    Time total;
    Time diff;
    Time adjusted;

    cout << weeding << endl;
    cout << waxing << endl;

    total = weeding + waxing;
    cout << total << endl;

    diff = weeding - waxing;
    cout << diff << endl;

    adjusted = total * 1.5;
    cout << adjusted << endl;
    return 0;
}