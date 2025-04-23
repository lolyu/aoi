# cpp object alignment
* Every object type has the property called alignment requirement, which is a nonnegative integer value (of type std::size_t, and always a power of two) representing the number of bytes between successive addresses at which objects of this type can be allocated.
* each object should be placed at address that is a multiple of its alignment requirement.

## the alignment rules for POD
* For POD (plain old data), the rules are typically:
    * Each member in the structure has some size `s` and some alignment requirement `a`.
    * The compiler starts with a size `S` set to zero and an alignment requirement `A` set to one (byte).
    * The compiler processes each member in the structure in order:
        * Consider the member’s alignment requirement `a`. If `S` is not currently a multiple of `a`, then add just enough bytes to `S` so that it is a multiple of `a`.
            * This determines where the member will go; it will go at offset `S` from the beginning of the structure (for the current value of `S`).
        * Set `A` to the least common multiple of `A` and `a`.
    * Add `s` to `S`, to set aside space for the member.
    * When the above process is done for each member, consider the structure’s alignment requirement `A`. If `S` is not currently a multiple of `A`, then add just enough to `S` so that it is a multiple of `A`.
    * The size of the structure is the value of `S` when the above is done.
* Additionally:
    * if any member is an array, its size is the number of elements multiplied by the size of each element, and its alignment requirement is the alignment requirement of an element
    * if any member is a structure, its size and alignment requirement are calculated as above
    * if any member is a union, its size is the size of its largest member plus ust enough to make it a multiple of the least common multiple of the alignments of all the members.
* **NOTE**:
    * for basic types(`char`, `int`, `long`, etc), the alignment requirement is just its size.

```cpp
struct alignas(16) Bar            // S = 0, A = 1
{
    int i;                        // s = 4, a = 4 -> S = 4, A = 4 
    int n;                        // s = 4, a = 4 -> S = 8, A = 4
    alignas(4) char arr[3];       // s = 4, a = 4 -> S = 12, A = 4
    short s;                      // s = 2, a = 2 -> S = 14, A = 4
};                                // As A = 4, S = 14 + 2 = 16 NOTE: S should be a multiple of A
```

## alignas and alignof
* `alignas` sets the align requirement for the element
* `alignof` calculates the align requirement for the type
* how to disable alignment?
    * in `GCC`, use `__attribute__((packed))`
```
#include <iostream>

using namespace std;

struct alignas(16) Bar
{
    int i; // 4 bytes
    int n; // 4 bytes
    alignas(4) char arr[3];
    short s; // 2 bytes
};

struct BarNoPadding
{
    int i; // 4 bytes
    int n; // 4 bytes
    char arr[3];
    short s; // 2 bytes
} __attribute__((packed));

int main()
{
    // the align requirement of basic types is just its size
    cout << alignof(char) << endl;         // 1
    cout << alignof(short) << endl;        // 2
    cout << alignof(int) << endl;          // 4
    cout << alignof(long) << endl;         // 8
    cout << alignof(long long) << endl;    // 8
    cout << alignof(float) << endl;        // 4
    cout << alignof(double) << endl;       // 8
    cout << alignof(Bar) << endl;          // 4
    cout << sizeof(Bar) << endl;           // 16
    cout << alignof(BarNoPadding) << endl; // 1
    cout << sizeof(BarNoPadding) << endl;  // 13
    return 0;
}
```

## references
* https://en.cppreference.com/w/cpp/language/object#Alignment
* https://stackoverflow.com/questions/14510711/how-is-the-size-of-a-c-class-determined
