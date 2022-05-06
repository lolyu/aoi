#include <iostream>
#include <string>
#include <tuple>
#include <vector>

int main()
{
    using namespace std;
    pair<string, vector<int>> foo(
        piecewise_construct,
        forward_as_tuple(string("helloworld")),
        forward_as_tuple(2, 200)
    );
    cout << foo.first << endl;
    for (auto s : foo.second) {
        cout << s << endl;
    }
    return 0;
}