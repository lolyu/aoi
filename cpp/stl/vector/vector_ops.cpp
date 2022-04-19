#include <iostream>
#include <string>
#include <vector>

using namespace std;

void print_double_vector(const vector<double> &vd)
{
    for (vector<double>::const_iterator it = vd.begin(); it != vd.end(); ++it) {
        cout << *it << '\t';
    }
    cout << endl;
}

int main()
{
    vector<double> vd0(10); // zero initialization
    cout << vd0.size() << endl;
    cout << vd0.capacity() << endl;
    vector<double> vd1{0, 1, 2, 3};
    cout << vd1.size() << endl;
    cout << vd1.capacity() << endl;

    print_double_vector(vd1);
    vd1.erase(vd1.begin(), vd1.begin() + 2);
    print_double_vector(vd1);
    vd1.insert(vd1.begin(), vd0.begin(), vd0.end());
    print_double_vector(vd1);
    vd1.push_back(100.0f);
    vd1.push_back(201.10f);
    print_double_vector(vd1);
    return 0;
}