#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <valarray>
#include <vector>

using namespace std;

int main()
{
    vector<double> ved1(10), ved2(10), ved3(10);
    array<double, 10> vod1, vod2, vod3;
    valarray<double> vad1(10), vad2(10), vad3(10);

    ved1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ved2 = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    transform(ved1.begin(), ved1.end(), ved2.begin(), ved3.begin(), plus<double>());
    copy(ved3.begin(), ved3.end(), ostream_iterator<double, char>(cout, " "));
    cout << endl;

    vod1 = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10};
    vod2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    transform(vod1.begin(), vod1.end(), vod2.begin(), vod3.begin(), multiplies<double>());
    copy(vod3.begin(), vod3.end(), ostream_iterator<double, char>(cout, " "));
    cout << endl;

    vad1 = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 1.1, 1, 1, 1};
    vad2 = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    vad3 = vad1 * vad2;
    vad3 *= 10;
    copy(begin(vad3), end(vad3), ostream_iterator<double, char>(cout, " "));
    cout << endl;
    sort(begin(vad3), end(vad3));
    copy(begin(vad3), end(vad3), ostream_iterator<double, char>(cout, " "));
    cout << endl;

    return 0;
}