#include <iostream>

using namespace std;

struct job
{
    char name[40];
    double salary;
    int floor;
};

template <typename T>
void Swap(T &a, T &b);

template <typename T>
void Swap(T *a, T *b, int n);

// explicit specialization
template <>
void Swap<job>(job &a, job &b);

// explicit instantiation
template void Swap<long>(long &a, long &b);

template <typename T>
void show(const T *a, int n);

void show(const job &j);

template <typename T1, typename T2>
auto Add(T1 a, T2 b) -> decltype(a + b);

int main()
{
    // non-template function > explicit specialization > template function
    int a = 10;
    int b = 20;
    Swap<int>(a, b);

    int aa[5] = {1, 2, 3, 4, 5};
    int bb[5] = {11, 22, 33, 44, 55};
    show(aa, 5);
    show(bb, 5);
    Swap<int>(aa, bb, sizeof(aa) / sizeof(int));
    show(aa, 5);
    show(bb, 5);

    job ja = {"Alice", 24339.0f, 10};
    job jb = {"Bob", 19223.4f, 3};
    show(ja);
    show(jb);
    Swap(ja, jb);
    show(ja);
    show(jb);
    // use <> to explicitly use template function
    Swap<>(ja, jb);
    show(ja);
    show(jb);

    cout << sizeof(Add(10L, 20)) << endl;
    return 0;
}

template <typename T>
void Swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
void Swap(T *a, T *b, int n)
{
    T temp;
    for (int i = 0; i < n; ++i)
    {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}

template <>
void Swap<job>(job &a, job &b)
{
    double dt;
    int it;
    dt = a.salary;
    a.salary = b.salary;
    b.salary = dt;
    it = a.floor;
    a.floor = b.floor;
    b.floor = it;
}

template <typename T>
void show(const T *a, int n)
{
    for (int i = 0; i < n; ++i)
    {
        cout << a[i] << '\t';
    }
    cout << endl;
}

void show(const job &j)
{
    cout << j.name << " with salary as " << j.salary << ", floor " << j.floor << endl;
}

// trailing return type
template <typename T1, typename T2>
auto Add(T1 a, T2 b) -> decltype(a + b)
{
    return a + b;
}
