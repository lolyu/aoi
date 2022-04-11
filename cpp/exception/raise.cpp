#include <iostream>

using namespace std;

int main()
{
    try {
        throw "divided by zero";
    } catch (const char *s) {
        cout << s << endl;
    }
    return 0;
}