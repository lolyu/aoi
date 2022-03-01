#include <iostream>
#include <map>
#include <typeinfo>

using namespace std;

enum Fruit {
    apple,
    banana,
    cherry
};

int main()
{
    map<string, Fruit> fruitMap;
    cout << fruitMap["apple"] << endl;
    cout << typeid(fruitMap["apple"]).name() << endl;
    return 0;
}