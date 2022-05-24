#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>

template <typename T>
class TooBig
{
public:
    TooBig(const T &t)
        : cutoff(t) {}
    bool operator()(const T &t) { return t > cutoff; }

private:
    T cutoff;
};

void output(int n)
{
    std::cout << n << std::endl;
}

int main()
{
    TooBig<int> f100(100);
    std::list<int> yadayada{50, 100, 90, 180, 60, 210, 415, 88, 188, 201};
    std::list<int> etcetera{50, 100, 90, 180, 60, 210, 415, 88, 188, 201};
    std::cout << "Original lists:" << std::endl;

    for_each(yadayada.begin(), yadayada.end(), output);
    std::cout << std::endl;

    for_each(etcetera.begin(), etcetera.end(), output);
    std::cout << std::endl;

    yadayada.remove_if(f100);
    etcetera.remove_if(TooBig<int>(200));

    std::cout << "Trimmed lists:" << std::endl;

    for_each(yadayada.begin(), yadayada.end(), output);
    std::cout << std::endl;

    for_each(etcetera.begin(), etcetera.end(), output);
    std::cout << std::endl;

    return 0;
}
