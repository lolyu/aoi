#include <algorithm>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>

int main()
{
    std::multimap<int, int> mm;
    std::vector<int> numbers = {0, 1, 2, 3, 4, 5, 6};
    for (auto num : numbers) {
        mm.insert(std::make_pair(num, num));
        mm.insert(std::make_pair(num, num + 1));
        mm.insert(std::make_pair(num, num * 2));
    }
    for (auto [key, value] : mm) {
        std::cout << key << ":\t" << value << std::endl;
    }
    std::cout << std::endl;

    // multimap keeps the order of values for the same key as the insertion order
    mm.insert(std::make_pair(0, 100));
    for (auto [key, value] : mm) {
        std::cout << key << ":\t" << value << std::endl;
    }
    std::cout << std::endl;

    auto [lower, upper] = mm.equal_range(0);
    for (auto it = lower; it != upper; ++it) {
        std::cout << it->first << ":\t" << it->second << std::endl;
    }
    std::cout << std::endl;

    return 0;
}