#include <iostream>
#include <type_traits>

class Trivial0
{
public:
    Trivial0(int i = 0, int j = 0) : i(i), j(j) {}

private:
    int i;
    int j;
};

class Trivial1
{
public:
    Trivial1(int i, int j) : i(i), j(j) {}
    Trivial1() = default;

private:
    int i;
    int j;
};

class StandardLayout0
{
public:
private:
    int i;
    int j;
};

class StandardLayout1
{
public:
    int i;

private:
    int j;
};

class POD0
{
public:
    POD0() = default;
    int i;

private:
    int j;
};

class POD1
{
public:
    POD1(int i = 0, int j = 0) : i(i), j(j) {}

private:
    int i;
    int j;
};

int main()
{
    using namespace std;
    Trivial0 t0{1, 2};
    Trivial1 t1;

    cout << is_trivial<Trivial0>::value << endl;
    cout << is_trivial<Trivial1>::value << endl;
    cout << is_standard_layout<StandardLayout0>::value << endl;
    cout << is_standard_layout<StandardLayout1>::value << endl;
    cout << is_pod<POD0>::value << endl;
    cout << is_pod<POD1>::value << endl;

    return 0;
}
