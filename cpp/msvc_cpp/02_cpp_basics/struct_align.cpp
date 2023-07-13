#include <iostream>

// for each member, the starting address must be the multiple of its align requirement
// the struct align requirement is the least common multiple of all members' align requirement
// (gdb) x/40b &d
// 0x7fffffffdfe0: 1       0       0       0       0       0       0       0
// 0x7fffffffdfe8: 0       0       0       0       0       0       0       64
// 0x7fffffffdff0: 3       80      85      85      4       0       0       0
// 0x7fffffffdff8: 5       80      85      85      6       0       0       0
// 0x7fffffffe000: 7       8       -1      -1      -1      127     0       0
struct my_data
{
    // S = 0, A = 1
    int i;             // s = 4, a = 4, padding: S = 0, A = 1 -> fill: S = 4, A = 4
    double d;          // s = 8, a = 8 padding: S = 8, A = 4 -> fill: S = 16, A = 8
    unsigned bf1 : 10; // S = 16 + 2
    int bf2 : 25;      // S = 18 + 4
    int : 0;           // S = 24 zero bit field forces padding, so S is padded to aligned to 8
    int bf4 : 9;       // S = 24 + 2
    int i2;            // s = 4, a = 4, padding: S = 28, A = 8 -> fill: S = 32, A = 8
    char c1;           // s = 1, a = 1, padding: S = 32, A = 8 -> fill: S = 33, A = 8
    char c2;           // s = 1, a = 1, padding: S = 33, A = 8 -> fill: S = 34, A = 8
};                     // S = 40, A = 1

struct S
{
    // will usually occupy 2 bytes:
    unsigned char b1 : 3; // 1st 3 bits (in 1st byte) are b1
    unsigned char : 2;    // next 2 bits (in 1st byte) are blocked out as unused
    unsigned char b2 : 6; // 6 bits for b2 - doesn't fit into the 1st byte => starts a 2nd
    unsigned char b3 : 2; // 2 bits for b3 - next (and final) bits in the 2nd byte
};

int main()
{
    using namespace std;

    cout << alignof(int) << endl;
    cout << alignof(double) << endl;
    cout << alignof(unsigned) << endl;
    cout << alignof(char) << endl;

    cout << sizeof(my_data) << endl;
    cout << offsetof(my_data, i) << endl;
    cout << offsetof(my_data, d) << endl;
    cout << offsetof(my_data, i2) << endl;
    cout << offsetof(my_data, c1) << endl;
    cout << offsetof(my_data, c2) << endl;
    cout << sizeof(S) << endl;

    struct my_data d = {1, 2, 3, 4, 5, 6, 7, 8};

    return 0;
}
