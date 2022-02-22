#include <iostream>
#include <memory>

using namespace std;

class Resource
{
public:
    Resource(int i = 100) : i(i) { cout << "Allocated" << endl; }
    ~Resource() { cout << "Deallocated" << endl; }

private:
    int i;
};

int main()
{
    unique_ptr<Resource> res0(new Resource());
    unique_ptr<Resource> res1;
    cout << boolalpha;
    cout << (static_cast<bool>(res0) ? "not null" : "null") << endl;
    cout << (static_cast<bool>(res1) ? "not null" : "null") << endl;
    res1 = move(res0);
    cout << (static_cast<bool>(res0) ? "not null" : "null") << endl;
    cout << (static_cast<bool>(res1) ? "not null" : "null") << endl;
    if (res0)
    {
        cout << "res0 not null" << endl;
    }
    if (res1)
    {
        cout << "res1 not null" << endl;
    }
    // favor `make_unique` over creating unique_ptr yourself
    auto res2 = make_unique<Resource>(100);

    return 0;
}