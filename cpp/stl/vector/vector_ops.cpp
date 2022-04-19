#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void print_double_vector(const vector<double> &vd)
{
    // with iterator, we could modify container's content
    // with const_iterator, it is an iterator that points to a const value(like a `const T *`), dereferencing it returns a reference to a constant value(const T&)
    // when you have a const reference to the container, you can only get a const_iterator
    for (vector<double>::const_iterator it = vd.begin(); it != vd.end(); ++it) {
        cout << *it << '\t';
    }
    cout << endl;
}

int main()
{
    srand(time(NULL));
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
    vd1.swap(vd0);
    print_double_vector(vd0);
    print_double_vector(vd1);

    for_each(
        vd0.begin(), vd0.end(), [](double &d) { d += 10; }
    );
    print_double_vector(vd0);

    for_each(
        vd0.begin(), vd0.end(), [](double &d) { d += rand() % 10; }
    );
    random_shuffle(vd0.begin(), vd0.end());
    print_double_vector(vd0);

    for_each(
        vd0.begin(), vd0.end(), [](double &d) { d = (d >= 15 ? d : -d); }
    );
    print_double_vector(vd0);

    sort(vd0.begin(), vd0.end());
    print_double_vector(vd0);

    sort(vd0.begin(), vd0.end(), [](const double &l, const double &r) { return abs(l) < abs(r); });
    print_double_vector(vd0);
    return 0;
}