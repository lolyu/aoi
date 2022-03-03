#include <iostream>
#include <typeinfo>

using namespace std;

// bound template friends, means the friend class/function shares the same template parameter as the class template
// the friend class/function defined inside the class template is actually a specialization of the its own template

// step1: define friend function as function template
template <typename T>
void counts();

template <typename T>
void reports(T &);

template <typename T>
class HasFriend {
private:
    T item;
    static int ct;

public:
    HasFriend(const T &t)
        : item(t) { ++ct; }
    ~HasFriend() { --ct; }
    // step2: define friend function specialization inside the class template
    friend void counts<HasFriend<T>>();
    friend void reports<HasFriend<T>>(HasFriend<T> &);
};

// each specialization will have its own static data member
template <typename T>
int HasFriend<T>::ct = 0;

// step3: provide friend function template definitions
template <typename T>
void counts()
{
    cout << typeid(T).name() << " count: " << T::ct << ";" << endl;
}

template <typename T>
void reports(T &hf)
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