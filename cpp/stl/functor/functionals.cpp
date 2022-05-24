#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

int main()
{
    vector<double> vd0{10.0, 20.0, 30.0, 40.0, 50.0, 60.0};
    vector<double> vd1{11.1, 22.2, 33.3, 44.4, 55.5, 66.6};
    vector<double> negates;
    vector<double> means;

    transform(vd0.begin(), vd0.end(), back_inserter(negates), negate<double>());
    copy(negates.begin(), negates.end(), ostream_iterator<double, char>(cout, "\t"));
    cout << endl;

    transform(vd0.begin(), vd0.end(), vd1.begin(), back_inserter(means), [](double left, double right) { return left + right / 2; });
    copy(means.begin(), means.end(), ostream_iterator<double, char>(cout, "\t"));
    cout << endl;
    return 0;
}