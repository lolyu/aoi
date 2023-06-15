#include <iostream>

using namespace std;

// https://learn.microsoft.com/en-us/cpp/cpp/ellipses-and-variadic-templates?view=msvc-170
// An ellipsis is used in two ways:
// to the left of the parameter name: parameter pack
// to the right of the parameter name: pack expansion

// the parameter pack must be the final parameter in the template parameter list for class
template <typename... Types>
struct Tuple
{
};

struct Tuple<> t0;
struct Tuple<int> t1;
struct Tuple<int, float> t2;

template <class... Types>
void f(Types... args);

template <typename... Arguments>
int function(Arguments... args);

template <typename... Arguments>
int function(Arguments &&...args);

template <typename... Arguments>
int function(Arguments *...args);

template <typename... Arguments>
int function(const Arguments *...args);

void print()
{
    cout << endl;
}

template <typename T>
void print(const T &t)
{
    cout << t << endl;
}

template <typename T, typename... Types>
void print(const T &t, const Types &...types)
{
    cout << t << ", ";
    print(types...);
}

int main()
{
    print("helloworld", 1, 2, 3);
    return 0;
}
