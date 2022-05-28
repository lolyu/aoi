#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
    using namespace std;

    vector<int> vi{0, 1, 2, 3, 4, 5};
    copy(vi.cbegin(), vi.cend(), ostream_iterator<int, char>(cout, " "));
    cout << endl;

    transform(vi.cbegin(), vi.cend(), ostream_iterator<int, char>(cout, " "), [](int i) { return -i; });
    cout << endl;

    vector<double> vd{0.1, 1.1, 2.1, 3.1, 4.1, 5.1};
    transform(vd.cbegin(), vd.cend(), vi.cbegin(), ostream_iterator<double, char>(cout, " "), [](double d, int i) { return d + i; });
    cout << endl;

    replace(vi.begin(), vi.end(), 2, 200);

    vector<int> vid;
    replace_copy(vi.begin(), vi.end(), back_inserter(vid), 2, 200);
    copy(vid.cbegin(), vid.cend(), ostream_iterator<int, char>(cout, " "));
    cout << endl;

    replace_if(
        vi.begin(), vi.end(), [](int i) { return i % 2 == 0; }, 100
    );
    copy(vi.cbegin(), vi.cend(), ostream_iterator<int, char>(cout, " "));
    cout << endl;
    return 0;
}