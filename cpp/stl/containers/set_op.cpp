#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <tuple>

int main()
{
    const int N = 6;
    std::string s1[N] = {"buffoon", "thinkers", "for", "heavy", "can", "for"};
    std::string s2[N] = {"metal", "any", "food", "elegant", "deliver", "for"};

    std::set<std::string> A(s1, s1 + N);
    std::set<std::string> B(s2, s2 + N);

    std::ostream_iterator<std::string> osi(std::cout, " ");

    std::copy(A.begin(), A.end(), osi);
    std::cout << std::endl;

    std::copy(B.begin(), B.end(), osi);
    std::cout << std::endl;

    std::set_union(A.begin(), A.end(), B.begin(), B.end(), osi);
    std::cout << std::endl;

    std::set_difference(A.begin(), A.end(), B.begin(), B.end(), osi);
    std::cout << std::endl;

    std::set_intersection(A.begin(), A.end(), B.begin(), B.end(), osi);
    std::cout << std::endl;

    std::set<std::string> C;
    std::set_union(A.begin(), A.end(), B.begin(), B.end(), inserter(C, C.begin()));

    std::copy(C.begin(), C.end(), osi);
    std::cout << std::endl;

    std::set<std::string>::iterator pos;
    std::tie(pos, std::ignore) = C.insert("hello");
    std::copy(pos, C.end(), osi);
    std::cout << std::endl;

    auto lower = C.lower_bound("can");
    auto upper = C.upper_bound("for");
    std::copy(lower, upper, osi);
    std::cout << std::endl;

    return 0;
}