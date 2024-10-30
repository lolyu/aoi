#include <iostream>
#include <iterator>
#include <vector>

template <typename Iter>
void reverse(Iter first, Iter last)
{
    typename std::iterator_traits<Iter>::difference_type n = std::distance(first, last);
    for (--n; n > 0; n -= 2)
    {
        typename std::iterator_traits<Iter>::value_type tmp = *first;
        *first++ = *--last;
        *last = tmp;
    }
}

int main()
{
    std::vector<int> vi{0, 1, 2, 3, 5};

    reverse(vi.begin(), vi.end());

    for (auto i : vi)
    {
        std::cout << i << std::endl;
    }

    return 0;
}
