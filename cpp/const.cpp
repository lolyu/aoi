#include <iostream>

using namespace std;

int main()
{
    int a = 0;
    // we can assign non const variable address to const pointer
    // in this way, we could not modify the non const variable via this pointer
    const int *p = &a;
    // *p = 20;
    return 0;
}