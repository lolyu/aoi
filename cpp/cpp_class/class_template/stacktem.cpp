#include <cctype>
#include <iostream>
#include <string>

#include "stacktp.h"

using namespace std;

int main()
{
    Stack<string> names;
    names.push("Alice Jenson");
    names.push("Bob Dylan");
    names.push("Catherine Marley");
    names.push("Davie John");
    string name;
    while (!names.isempty()) {
        names.pop(name);
        cout << name << endl;
    }
    return 0;
}