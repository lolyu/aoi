#include <iostream>
#include <vector>

using namespace std;

void print(const int ip[], int n);

int main()
{
    vector<int> vi(10, 100);
    print(vi.data(), vi.size());
    return 0;
}

void print(const int ip[], int n)
{
    for (int i = 0; i < n; ++i) {
        cout << ip[i] << ' ';
    }
    cout << endl;
}