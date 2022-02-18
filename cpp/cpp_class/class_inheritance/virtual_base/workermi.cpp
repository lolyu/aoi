#include "workermi.h"
#include <iostream>

using namespace std;

Worker::~Worker() = default;

void Worker::Get()
{
    getline(cin, fullname);
    cout << "Enter worker's ID: ";
    cin >> id;
    while (cin && cin.get() != '\n')
    {
        continue;
    }
}

void Worker::Data() const
{
    cout << "Name: " << fullname << endl;
    cout << "Employee ID: " << id << endl;
}

void Waiter::Get()
{
    cout << "Enter waiter's panache rating: ";
    cin >> panache;
    while (cin && cin.get() != '\n')
    {
        continue;
    }
}

void Waiter::Data() const
{
    cout << "Panache rating" << panache << endl;
}

void Waiter::Set()
{
    cout << "Enter waiter's name: ";
    Worker::Get();
    Get();
}

void Waiter::Show() const
{
    cout << "Category: waiter" << endl;
    Worker::Data();
    Data();
}

const char *Singer::pv[vtypes] = {"other", "alto", "contralto", "soprano", "bass", "baritone", "tenor"};

void Singer::Get()
{
    cout << "Enter number for singer's vocal range: " << endl;
    int i;
    for (i = 0; i < vtypes; ++i)
    {
        cout << i << ": " << pv[i] << " ";
        if (i % 4 == 3)
        {
            cout << endl;
        }
    }
    if (i % 4 != 0)
    {
        cout << endl;
    }
    cin >> voice;
    while (cin && cin.get() != '\n')
    {
        continue;
    }
}

void Singer::Data() const
{
    cout << "Vocal range: " << pv[voice] << endl;
}

void Singer::Set()
{
    cout << "Enter singer's name: ";
    Worker::Get();
    Get();
}

void Singer::Show() const
{
    cout << "Category: singer" << endl;
    Worker::Data();
    Data();
}

void SingingWaiter::Get()
{
    Waiter::Get();
    Singer::Get();
}

void SingingWaiter::Data() const
{
    Waiter::Data();
    Singer::Data();
}

void SingingWaiter::Set()
{
    cout << "Enter singer waiter's name: ";
    Worker::Get();
    Get();
}

void SingingWaiter::Show() const
{
    cout << "Category: singing waiter" << endl;
    Worker::Data();
    Data();
}
