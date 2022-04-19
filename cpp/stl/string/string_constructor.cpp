#include <iostream>
#include <string>

int main()
{
    using namespace std;
    const char* s = "helloworld";
    string one(s);
    string two(20, '#');
    string three(one);
    string four;
    string five(s, 5);
    string six(s, s + 5);
    string seven(s, 0, 5);
    string eight{'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};
    cout << one << endl;
    cout << two << endl;
    cout << three << endl;
    cout << four << endl;
    cout << five << endl;
    cout << six << endl;
    cout << seven << endl;
    cout << eight << endl;
    return 0;
}