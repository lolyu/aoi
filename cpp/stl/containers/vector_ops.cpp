#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

template <typename T>
void print(const vector<T> &list)
{
    copy(list.cbegin(), list.cend(), ostream_iterator<T>(cout, " "));
    cout << endl;
}

int main()
{
    vector<int> list0 = {0, 1, 2, 3, 4, 5};
    vector<int> list1(10, 100);
    vector<int> list2(list0.cbegin(), list0.cend());
    print(list0);
    print(list1);
    print(list2);
    // insert inserts the element into the position before the iterator
    list2.insert(list2.begin(), list1[2]);
    list2.insert(list2.begin(), 2, 1);
    list2.insert(list2.end(), list0.cbegin(), list0.cend());
    print(list2);
    // erase removes the element pointed by the iterator
    list2.erase(list2.begin() + 2);
    list2.erase(list2.begin(), list2.begin() + 4);
    print(list2);

    cout << list2.front() << " " << list2.back() << endl;
    list2.push_back(10000);
    print(list2);
    cout << list2[100] << endl;
    // compared to [] operator, at will do a boundary check, out_of_range will be throw if the index is out of range
    cout << list2.at(100) << endl;
    return 0;
}