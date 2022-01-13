#include "queue.h"

using namespace std;

int main()
{
    Queue q0 = 10;
    Item i0, i1, i2;
    q0.enqueue(i0);
    q0.enqueue(i1);
    q0.enqueue(i2);
    Queue q1 = q0;
    std::cout << q0 << std::endl;
    std::cout << q1 << std::endl;
    return 0;
}