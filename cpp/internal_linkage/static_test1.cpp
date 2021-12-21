#include "demo.h"

// extern int b = 20;   not OK, as this is defininig an external variable b
// extern int b;        OK, referencing declaration
// static int b = 20;   OK, shadowing the one sharing across TU
int A::static_member = 10;
const std::string A::static_str_member = "helloworld";

int func()
{
    return 0;
}

int main()
{
    using namespace std;
    cout << A::static_member << endl;
    return 0;
}