#include "dma.h"

using namespace std;

int main()
{
    BaseDMA bd("helloworld", 20);
    HasDMA hd("helloworld", "demo", 20);
    cout << bd;
    cout << hd;
    HasDMA pd = hd;
    cout << pd;
    return 0;
}