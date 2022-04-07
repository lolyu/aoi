#include <iostream>

using namespace std;

/*
If a friend declaration in a non-local class first declares a class or function
(this implies that the name of the class or function is unqualified) the friend class or function is a member of the innermost enclosing namespace.
https://stackoverflow.com/questions/4492062/why-does-a-c-friend-class-need-a-forward-declaration-only-in-other-namespaces
*/

class TV
{
    friend class Remote;
};

class Remote
{
    friend class TV;
};

int main()
{
    return 0;
}