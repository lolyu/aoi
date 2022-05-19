#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>

using namespace std;

void output(int n)
{
    cout << n << " ";
}

int main()
{
    list<int> one(5, 2);
    list<int> two;
    int stuff[5] = {1, 2, 4, 8, 6};
    two.insert(two.begin(), stuff, stuff + 5);
    list<int> three(two);
    int more[6] = {6, 4, 2, 4, 6, 5};
    three.insert(three.end(), more, more + 6);

    for_each(one.begin(), one.end(), output);
    cout << endl;

    for_each(two.begin(), two.end(), output);
    cout << endl;

    for_each(three.begin(), three.end(), output);
    cout << endl;

    three.remove(2);
    for_each(three.begin(), three.end(), output);
    cout << endl;

    three.splice(three.begin(), one);
    for_each(three.begin(), three.end(), output);
    cout << endl;

    // unique remove continuous duplicates
    three.unique();
    for_each(three.begin(), three.end(), output);
    cout << endl;

    three.sort();
    three.unique();
    for_each(three.begin(), three.end(), output);
    cout << endl;

    two.sort();
    three.merge(two);
    for_each(three.begin(), three.end(), output);
    cout << endl;

    three.unique();
    for_each(three.begin(), three.end(), output);
    cout << endl;

    for (auto it = three.begin(); it != three.end(); ++it) {
        if (*it % 2 == 0) {
            list<int> tmp(2, 10);
            three.splice(it, tmp);
        }
    }
    for_each(three.begin(), three.end(), output);
    cout << endl;

    return 0;
}