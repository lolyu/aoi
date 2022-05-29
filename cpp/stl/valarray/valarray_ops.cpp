#include <cstdlib>
#include <iostream>
#include <valarray>

using namespace std;

const int SIZE = 12;
typedef valarray<int> vint;

void show(const vint &v, int cols)
{
    for (size_t i = 0; i < v.size(); ++i) {
        cout.width(3);
        cout << v[i];
        if (i % cols == cols - 1) {
            cout << endl;
        } else {
            cout << ' ';
        }
    }
    if (v.size() % cols != 0) {
        cout << endl;
    }
}

int main()
{
    vint valint(SIZE);
    size_t i;
    for (i = 0; i < valint.size(); ++i) {
        valint[i] = rand() % 10;
    }

    show(valint, 3);
    cout << endl;

    vint vcol(valint[slice(1, 4, 3)]);
    show(vcol, 1);
    cout << endl;

    vint vrow(valint[slice(3, 3, 1)]);
    show(vrow, 3);
    cout << endl;

    valint[slice(2, 4, 3)] = 10;
    show(valint, 3);
    cout << endl;

    valint[slice(0, 4, 3)] = vint(valint[slice(1, 4, 3)]) + vint(valint[slice(2, 4, 3)]);
    show(valint, 3);
    return 0;
}
