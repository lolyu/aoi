#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

int main()
{
    vector<string> strings = {"hello", "world", "myfriend"};
    queue<string> q;
    for (const auto &s : strings) {
        q.push(s);
    }
    while (q.empty()) {
        cout << q.front() << endl;
        q.pop();
    }
    return 0;
}