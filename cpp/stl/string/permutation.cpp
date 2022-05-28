#include <algorithm>
#include <iostream>
#include <string>

int main()
{
    using namespace std;

    string s("hello");
    sort(s.begin(), s.end());
    do {
        cout << s << endl;
    } while (next_permutation(s.begin(), s.end()));

    return 0;
}