#include <iostream>

#include "arraytp.h"

using namespace std;

int main()
{
    ArrayTP<int, 10> demo(100);
    cout << demo[9] << endl;
    cout << demo[10] << endl;
    return 0;
}