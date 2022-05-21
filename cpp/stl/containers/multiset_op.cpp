#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <tuple>
#include <vector>

template <typename Container, typename Element>
void print(const Container &c)
{
    std::copy(c.begin(), c.end(), std::ostream_iterator<Element>(std::cout, " "));
    std::cout << std::endl;
}

int main()
{
    std::vector<int> vi0 = {0, 1, 2, 3, 43, 5, 23, 13};
    std::vector<int> vi1 = {1, 3, 4, 4, 5, 0, 0};

    std::multiset<int> ms;
    copy(vi0.begin(), vi0.end(), std::inserter(ms, ms.begin()));
    copy(vi1.begin(), vi1.end(), std::inserter(ms, ms.begin()));
    print<std::multiset<int>, int>(ms);

    std::multiset<int>::iterator lower, upper;
    std::tie(lower, upper) = ms.equal_range(0);
    for (auto it = lower; it != upper; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}