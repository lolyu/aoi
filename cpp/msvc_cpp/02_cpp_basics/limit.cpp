#include <limits>
#include <iostream>

using namespace std;

int main()
{
    long int_max = numeric_limits<int>::max();
    long int_min = numeric_limits<int>::min();
    cout << "INT MAX: " << int_max << endl;
    cout << "INT MIN: " << int_min << endl;
    return 0;
}
