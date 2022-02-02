#include <iostream>
#include <string>
#include "brass.h"

using namespace std;

const int CLIENTS = 4;

int main()
{
    Brass *clients[CLIENTS];
    string temp;
    long tempnum;
    double tempbal;
    char kind;

    for (int i = 0; i < CLIENTS; ++i)
    {
        cout << "Enter client's name: ";
        getline(cin, temp);
        cout << "Enter client's account number: ";
        cin >> tempnum;
        cout << "Enter opening balance: $";
        cin >> tempbal;
        cout << "Enter 1 for Brass account or 2 for BrassPlus Account: ";
        while (cin >> kind && (kind != '1' && kind != '2'))
        {
            cout << "Enter either 1 or 2: ";
        }
        if (kind == '1')
        {
            clients[i] = new Brass(temp, tempnum, tempbal);
        }
        else
        {
            double tmax, trate;
            cout << "Enter the overdraft limit: $";
            cin >> tmax;
            cout << "Enter the interest rate as a decimal fraction: ";
            cin >> trate;
            clients[i] = new BrassPlus(temp, tempnum, tempbal, tmax, trate);
        }
        while (cin.get() != '\n')
        {
            continue;
        }
    }
    cout << endl;
    for (int i = 0; i < CLIENTS; ++i)
    {
        clients[i]->ViewAccount();
        cout << endl;
    }

    for (int i = 0; i < CLIENTS; ++i)
    {
        delete clients[i];
    }
    cout << "Done." << endl;

    return 0;
}