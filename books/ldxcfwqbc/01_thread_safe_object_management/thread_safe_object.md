# thread safe object

## thread-safe constructor
* **RULE**: don't share `this` in the constructor
    * the object is not fully-initialized, sharing `this` in the constructor could enable other threads to access this partially-initialized object, which might cause unexpected results.
 
## thread-safe destructor
* in OOP, three relationships between objects:
    * association - has a
    * aggregation - has a + part of
    * composition - has a + part of + ownership
        * safe, because own the lifetime of the part
* `dangling pointer`: a pointer pointing to a memory location that has been deleted (or freed)
* `wild pointer`: a pointer that has not been initialized to anything (not even `NULL` or `nullptr`)
* if one thread does the deallocation of an object, and another thread is using this object, which might lead to an unexpected result:
    * in the following example, thread `A` is calling `Observable::notify()` and thread `B` is deallocating an `Observer` with `~Observer`

```cpp
class Observer
{
public:
    Observer() = delete;
    Observer(Observable *s);
    virtual ~Observer();
    virtual void update();

private:
    Observable *subject;
};

class Observable
{
public:
    Observable() = default;
    void register_observer(Observer *x);
    void unregister_observer(Observer *x);
    void notify();

private:
    unordered_map<int, Observer *> observers;
};

Observer::Observer(Observable *s) : subject(s)
{
    subject->register_observer(this);
}

Observer::~Observer()
{
    subject->unregister_observer(this);
}

void Observable::notify()
{
    for (auto &p : observers)
    {
        p.second->update();
    }
}
```

## use smart pointers

```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Observable;
class Observer;

class Observer : public enable_shared_from_this<Observer>
{
public:
    Observer() = delete;
    Observer(shared_ptr<Observable> s, int i);
    virtual ~Observer();
    virtual void update();

    int get_index() const { return i; }

private:
    int i;
    shared_ptr<Observable> subject;
};

class Observable
{
public:
    Observable() = default;
    void register_observer(const weak_ptr<Observer> &x);
    void notify();

private:
    vector<weak_ptr<Observer>> observers;
};

Observer::Observer(shared_ptr<Observable> s, int i) : subject(s), i(i)
{
    // NOTE: cannot register here, as the shared_ptr constructor is not
    // called yet, so the weak pointer member in the enable_shared_from_this
    // is not initialized yet, so weak_from_this() returns a weak_ptr that
    // points to nullptr
    // subject->register_observer(weak_from_this());
}

Observer::~Observer()
{
}

void Observer::update()
{
    cout << "Update observer " << get_index() << endl;
}

void Observable::register_observer(const weak_ptr<Observer> &x)
{
    observers.push_back(x);
}

void Observable::notify()
{
    for (auto it = observers.begin(); it != observers.end();)
    {
        auto sp = it->lock();
        if (sp)
        {
            sp->update();
            ++it;
        }
        else
        {
            it = observers.erase(it);
        }
    }
}

int main()
{
    auto observable_sp = make_shared<Observable>();
    auto obsv0 = make_shared<Observer>(observable_sp, 0);
    observable_sp->register_observer(obsv0);
    {
        auto obsv1 = make_shared<Observer>(observable_sp, 1);
        observable_sp->register_observer(obsv1);
        cout << "First notify" << endl;
        observable_sp->notify();
    }
    cout << "Second notify" << endl;
    observable_sp->notify();

    return 0;
}
```
