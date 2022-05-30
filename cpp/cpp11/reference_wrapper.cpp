#include <functional>
#include <iostream>

void f(int &n1, int &n2, const int &n3)
{
    std::cout << n1 << " " << n2 << " " << n3 << std::endl;
    ++n1;
    ++n2;
}

int main()
{
    int n1 = 1, n2 = 2, n3 = 3;
    std::function<void()> bound_f = std::bind(f, n1, std::ref(n2), std::cref(n3));
    n1 = 10, n2 = 11, n3 = 13;
    std::cout << n1 << " " << n2 << " " << n3 << std::endl;
    bound_f();
    std::cout << n1 << " " << n2 << " " << n3 << std::endl;
    return 0;
}