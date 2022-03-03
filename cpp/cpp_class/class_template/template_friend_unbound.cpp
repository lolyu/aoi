#include <iostream>
#include <typeinfo>

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

    template <typename TT>
    friend void counts();
    template <typename TT>
    friend void reports(TT &);
};

// each specialization will have its own static data member
template <typename T>
int HasFriend<T>::ct = 0;

template <typename TT>
void counts()
{
    cout << typeid(TT).name() << " count: " << TT::ct << ";" << endl;
}

template <typename TT>
void reports(TT &hf)
{
    cout << hf.item << endl;
}

int main()
{
    counts<HasFriend<int>>();
    HasFriend<int> hf1(10);
    HasFriend<int> hf2(20);
    HasFriend<double> hd(3.14);
    counts<HasFriend<int>>();
    counts<HasFriend<double>>();
    reports<HasFriend<int>>(hf1);
    reports<HasFriend<int>>(hf2);
    reports<HasFriend<double>>(hd);

    return 0;
}