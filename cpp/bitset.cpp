#include <bitset>
#include <iostream>

using namespace std;

int main()
{
    bitset<4> b0;
    cout << b0.none() << endl;
    bitset<10> b1("0011000000");
    cout << b1.any() << endl;
    cout << b1[6] << endl;
    cout << b1[7] << endl;
    bitset<10> b2("XXXYYXYXYY", 10, 'X', 'Y');
    cout << b2.all() << endl;
    cout << b2 << endl;
    b2.reset();
    cout << b2 << endl;
    b2[2] = 1;
    b2[4] = 1;
    b2[6] = 1;
    b2.set(9);
    cout << b2 << endl;
    cout << b2.count() << endl;
    cout << b2.size() << endl;
    return 0;
}