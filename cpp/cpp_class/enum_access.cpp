#include <iostream>
#include <vector>

using namespace std;

class Parent
{
public:
    using DoubleArray = vector<double>;

    enum class Gender
    {
        Male,
        Female
    };
};

class ChinaParent : public Parent
{
public:
};

int main()
{
    cout << static_cast<int>(ChinaParent::Gender::Male) << endl;
    ChinaParent::DoubleArray da{10, 20, 30};
    return 0;
}