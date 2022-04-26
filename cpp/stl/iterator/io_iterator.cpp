#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const string DEFAULT_DELIMITER = " ";

using IntArray = vector<int>;

void print_int_array(const IntArray &a);
void print_int_array_cout(const IntArray &a);
void get_int_array(IntArray &a);

void print_int_array_to_oss(const IntArray &a, ostringstream &oss, bool reverse = false, string delimiter = DEFAULT_DELIMITER);
void get_int_array_from_iss(IntArray &a, istringstream &iss);

int main()
{
    IntArray a{10, 20, 30, 40, 50};
    print_int_array(a);
    print_int_array_cout(a);
    IntArray b(10, 0);
    // get_int_array(b);
    // print_int_array(b);

    string input("20 30 40 50");
    istringstream iss(input);
    ostringstream oss;
    IntArray c(10, 0);
    get_int_array_from_iss(c, iss);
    print_int_array_to_oss(c, oss);
    cout << oss.str() << endl;
    oss.str("");
    print_int_array_to_oss(c, oss, true);
    cout << oss.str() << endl;
    return 0;
}

void print_int_array(const IntArray &a)
{
    copy(a.begin(), a.end(), ostream_iterator<int, char>(cout, " "));
    cout << endl;
}

void print_int_array_cout(const IntArray &a)
{
    for (auto i : a) {
        cout << i << " ";
    }
    cout << endl;
}

void get_int_array(IntArray &a)
{
    copy(istream_iterator<int, char>(cin), istream_iterator<int, char>(), a.begin());
}

void print_int_array_to_oss(const IntArray &a, ostringstream &oss, bool reverse, string delimiter)
{
    if (reverse) {
        copy(a.rbegin(), a.rend(), ostream_iterator<int>(oss, delimiter.c_str()));
    } else {
        copy(a.begin(), a.end(), ostream_iterator<int>(oss, delimiter.c_str()));
    }
    cout << endl;
}

void get_int_array_from_iss(IntArray &a, istringstream &iss)
{
    copy(istream_iterator<int>(iss), istream_iterator<int>(), a.begin());
}
