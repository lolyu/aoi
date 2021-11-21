#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class AddX
{
public:
    AddX(int x) : m_x(x) {}
    int operator()(int y) const { return m_x + y; }

private:
    int m_x;
};

template <typename T>
void print_vector(const vector<T> &v)
{
    for (const T &e : v)
    {
        cout << e << '\t';
    }
    cout << endl;
}

int main()
{
    vector<int> va{10, 20, 30, 40};
    vector<int> vb(va.size());
    transform(va.begin(), va.end(), vb.begin(), AddX(10));
    print_vector<int>(va);
    print_vector<int>(vb);
    return 0;
}