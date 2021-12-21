#include <array>
#include <iostream>

using namespace std;

int main()
{
    array<int, 10> ia = {0, 1, 2, 3};
    // cout << ia.at(100) << endl;
    cout << ia.empty() << endl;
    cout << ia.size() << endl;
    cout << ia.max_size() << endl;
    cout << ia[5] << endl;
    cout << ia.data() << endl;
    return 0;
}