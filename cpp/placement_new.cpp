#include <iostream>
#include <string>
#include <new>

using namespace std;
const int BUFSIZE = 512;

class JustTesting
{
public:
    JustTesting(const string &s = "Just Testing", int n = 0)
    {
        words = s;
        number = n;
        cout << words << " constructed" << endl;
    }

    ~JustTesting()
    {
        cout << words << " destroyed" << endl;
    }

    void show() const
    {
        cout << words << ", " << number << endl;
    }

private:
    string words;
    int number;
};

int main()
{
    char *buffer = new char[BUFSIZE];
    JustTesting *pc1, *pc2;
    pc1 = new (buffer) JustTesting;
    pc2 = new JustTesting("Heap1", 20);

    cout << "Memory block addresses:" << endl;
    cout << "buffer: " << static_cast<void *>(buffer) << " heap: " << pc2 << endl;
    cout << "Memory contents:" << endl;
    cout << pc1 << ": ";
    pc1->show();
    cout << pc2 << ": ";
    pc2->show();

    JustTesting *pc3, *pc4;
    pc3 = new (buffer + sizeof(*pc1)) JustTesting("Better Idea", 6);
    pc4 = new JustTesting("Heap2", 20);
    cout << "Memory contents:" << endl;
    cout << pc3 << ": ";
    pc3->show();
    cout << pc4 << ": ";
    pc4->show();

    delete pc2;
    delete pc4;
    // explicitly call destructor for objects created by placement new
    pc3->~JustTesting();
    pc1->~JustTesting();
    delete[] buffer;
    cout << "Done" << endl;

    return 0;
}