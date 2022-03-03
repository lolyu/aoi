#include <iostream>

using namespace std;

template <typename T>
class HasFriend {
private:
    T item;
    static int ct;

public:
    HasFriend(const T &t)
        : item(t) { ++ct; }
    ~HasFriend() { --ct; }
    friend void counts();
    friend void reports(HasFriend<T> &);
};

// each specialization will have its own static data member
template <typename T>
int HasFriend<T>::ct = 0;

void counts()
{
    cout << "int count: " << HasFriend<int>::ct << ";" << endl;
    cout << "double count: " << HasFriend<double>::ct << ";" << endl;
}

void reports(HasFriend<int> &hi)
{
    cout << "HasFriend<int>: " << hi.item << endl;
}

void reports(HasFriend<double> &hd)
{
    cout << "HasFriend<double>: " << hd.item << endl;
}

int main()
{
    counts();
    HasFriend<double> hd0(1.03);
    HasFriend<double> hd1(3.14);
    HasFriend<int> hi0(100);
    HasFriend<int> hi1(200);
    counts();
    reports(hd0);
    reports(hd1);
    reports(hi0);
    reports(hi1);
    return 0;
}