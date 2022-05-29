#include <initializer_list>
#include <iostream>

double sum(const std::initializer_list<double> &li)
{
    double total = 0;
    for (auto i = li.begin(); i != li.end(); ++i) {
        total += *i;
    }
    return total;
}

double average(const std::initializer_list<double> &li)
{
    double total = sum(li);
    return li.size() == 0 ? 0.0 : total / li.size();
}

int main()
{
    std::initializer_list<double> dl{1.1, 2.2, 3.3, 4.4};
    std::cout << "SUM: " << sum(dl) << std::endl;
    std::cout << "AVRG: " << average(dl) << std::endl;
    return 0;
}
