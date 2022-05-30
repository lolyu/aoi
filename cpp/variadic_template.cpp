#include <iostream>
#include <string>

using namespace std;

void show_list() {}

template <typename T>
void show_list(const T &val)
{
    cout << "here";
    cout << val << endl;
}

template <typename T, typename... Args>
void show_list(const T &val, const Args &...args)
{
    cout << val << ", ";
    cout << "here d" << endl;
    show_list(args...);
}

void print()
{
    cout << endl;
}

template <typename T>
void print(const T &t)
{
    cout << t << endl;
}

template <typename First, typename... Rest>
void print(const First &first, const Rest &...rest)
{
    cout << first << ", ";
    print(rest...); // recursive call using pack expansion syntax
}

int main()
{
    int n = 14;
    double d = 3.1415;
    string s = "helloworld";
    // show_list(n, d, s, 1000);
    print("first", 2, "third", 3.14159);
    return 0;
}