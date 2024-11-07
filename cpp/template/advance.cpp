#include <iostream>
#include <iterator>
#include <vector>


template <typename IterT, typename DistT>
void do_advance(IterT &iter, DistT d, std::input_iterator_tag)
{
    if (d < 0)
    {
        throw std::out_of_range("Negative distance");
    }
    for (; d >= 0; ++iter, --d);
}

template <typename IterT, typename DistT>
void do_advance(IterT &iter, DistT d, std::bidirectional_iterator_tag)
{
    if (d >= 0)
    {
        for (; d != 0; ++iter, --d);
    }
    else
    {
        for (; d != 0; ++iter, ++d);
    }
}

template <typename IterT, typename DistT>
void do_advance(IterT &iter, DistT d, std::random_access_iterator_tag)
{
    iter += d;
}

template <typename IterT, typename DistT>
void my_advance(IterT &iter, DistT d)
{
    typedef typename IterT::iterator_category tag;
    do_advance(iter, d, tag());
}

int main()
{
    std::vector<int> vi{0, 1, 2, 3, 4};
    auto iter = vi.begin();
    std::cout << *iter << std::endl;
    my_advance(iter, 2);
    std::cout << *iter << std::endl;
    return 0;
}
