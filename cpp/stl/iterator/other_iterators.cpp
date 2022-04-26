#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

void print_vector_int(const vector<int> &vi);
void print_deque_int(const deque<int> &qi);

int main()
{
    vector<int> vi{10, 20, 30, 40, 50};
    for (vector<int>::const_reverse_iterator it = vi.crbegin(); it != vi.crend(); ++it) {
        cout << *it << endl;
    }
    vector<int> dvi0(10);
    copy(vi.begin(), vi.end(), dvi0.begin());
    print_vector_int(dvi0);

    vector<int> dvi1;
    copy(vi.begin(), vi.end(), back_insert_iterator<vector<int>>(dvi1));
    print_vector_int(dvi1);

    copy(vi.begin(), vi.end(), insert_iterator<vector<int>>(dvi1, dvi1.begin() + 2));
    print_vector_int(dvi1);

    deque<int> dqi0;
    copy(vi.begin(), vi.end(), front_insert_iterator<deque<int>>(dqi0));
    print_deque_int(dqi0);

    return 0;
}

void print_vector_int(const vector<int> &vi)
{
    copy(vi.begin(), vi.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}

void print_deque_int(const deque<int> &qi)
{
    copy(qi.cbegin(), qi.cend(), ostream_iterator<int>(cout, " "));
    cout << endl;
}
