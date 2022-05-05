#include <iostream>
#include <map>

using namespace std;

int main()
{
    multimap<int, char> dict = {
        {1, 'A'},
        {2, 'B'},
        {2, 'C'},
        {2, 'D'},
        {4, 'E'},
        {3, 'F'}};
    auto range = dict.equal_range(2);
    for (auto it = range.first; it != range.second; ++it) {
        cout << it->first << ": " << it->second << endl;
    }
    return 0;
}