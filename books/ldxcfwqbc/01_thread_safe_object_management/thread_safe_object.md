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
